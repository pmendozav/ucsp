import matplotlib.pyplot as plt
import tensorflow as tf
import numpy as np
from sklearn.metrics import confusion_matrix
import time
from datetime import timedelta
import math
import os
import cifar10
from cifar10 import img_size, num_channels, num_classes


def random_batch(data, train_batch_size):
    # Number of images in the training-set.
    num_images = len(data.images_train)

    # Create a random index.
    idx = np.random.choice(num_images,
                           size=train_batch_size,
                           replace=False)

    # Use the random index to select random images and labels.
    x_batch = data.images_train[idx, :, :, :]
    y_batch = data.labels_train[idx, :]

    return x_batch, y_batch

class Cnn:
    session = None
    def __init__(self, X, channels, y_true, y_true_cls, save_dir = 'checkpoints/'):
        self.session = None
        
        self.conv_layers = []
        self.conv_weights = []

        self.fc_layers = []
        #self.fc_weights = []
        
        self.X = X
        self.channels = channels

        self.y_true = y_true
        self.y_true_cls = y_true_cls

        self.layer_flat = None
        self.num_features = None

        self.total_iterations = 0
        self.global_step = tf.Variable(initial_value=0,name='global_step', trainable=False)

        if not os.path.exists(save_dir):
            os.makedirs(save_dir)
        self.save_dir = save_dir
        self.save_path = os.path.join(save_dir, 'best_validation')
        
    def new_weights(self, shape, name='weights'):
        return tf.Variable(tf.truncated_normal(shape, stddev=0.05), name=name)

    def new_biases(self, length, name='biases'):
        return tf.Variable(tf.constant(0.05, shape=[length]), name=name)

    def add_conv_layer(self, filter_size, num_filters, scope, use_pooling=True, batch_normalize=False):
        with tf.variable_scope(scope):
            if len(self.conv_layers) is 0:
                _input = self.X
                channels = self.channels
            else:
                _input = self.conv_layers[-1]
                channels = self.conv_weights[-1].get_shape()[3].value
        
            shape = [filter_size, filter_size, channels, num_filters]

            with tf.name_scope('weights'):
                kernel = self.new_weights(shape=shape)
                #self.variable_summaries(kernel)
            with tf.name_scope('biases'):
                bias = self.new_biases(length=num_filters)
                #self.variable_summaries(bias)

            layer = tf.nn.conv2d(input=_input, filter=kernel, strides=[1,1,1,1], padding='SAME', name='layer')
            layer += bias

            if batch_normalize:
                mean, variance = tf.nn.moments(layer, axes=[0])
                layer = tf.nn.batch_normalization(layer, mean, variance, 0, 1.0, 0.000001)

            if use_pooling:
                layer = tf.nn.max_pool(value=layer, ksize=[1,2,2,1], strides=[1,2,2,1], padding='SAME')
        
            layer = tf.nn.relu(layer)
            
            self.conv_layers.append(layer)
            self.conv_weights.append(kernel)

    def flatten_layer(self, scope='flatten_layer'):
        with tf.name_scope(scope):
            layer = self.conv_layers[-1]
            shape = layer.get_shape()
            num_features = shape[1:4].num_elements()

            layer_flat = tf.reshape(layer, [-1, num_features])
            self.layer_flat = layer_flat
            self.num_features = num_features

    # new_fc_layer(input=layer_flat, num_inputs=num_features, num_outputs=fc_size)
    def add_fc_layer(self, num_outputs, scope, use_relu=True):
        with tf.name_scope(scope):
            if len(self.fc_layers) is 0:
                print(scope)
                input = self.layer_flat
                print(input)
                num_inputs = self.num_features
            else:
                #self.fc_layers[-1].get_shape()[1].value
                input = self.fc_layers[-1]
                num_inputs = input.get_shape()[1].value
                print("segunda capa fully: ",input)

            with tf.name_scope('weights'):
                weights = self.new_weights(shape=[num_inputs, num_outputs])
            with tf.name_scope('biases'):
                bias = self.new_biases(length=num_outputs)
            with tf.name_scope('Wx_plus_b'):
                layer = tf.matmul(input, weights) + bias
                if use_relu:
                    layer = tf.nn.relu(layer)
                #tf.summary.histogram('pre_activations', layer)

            self.fc_layers.append(layer)

    def config(self, train_batch_size, test_batch_size):
        with tf.name_scope('prediction'):
            layer = self.fc_layers[-1]
            self.y_pred = tf.nn.softmax(layer) # solo pa ver q es asi
            self.y_pred_cls = tf.argmax(self.y_pred, dimension=1) # solo pa ver q es asi

        ##
        with tf.name_scope('cross_entropy'):
            self.entropy = tf.nn.softmax_cross_entropy_with_logits(logits=layer, labels=self.y_true)
            with tf.name_scope('total'):
                self.cost = tf.reduce_mean(self.entropy)
            #tf.summary.scalar('cost', self.cost)

        with tf.name_scope('train'):
            self.optimizer = tf.train.AdamOptimizer(learning_rate=1e-4).minimize(self.cost, global_step=self.global_step) # Adam, Gradient, Stochastic
            #self.variable_summaries(self.cost)
        ##
        with tf.name_scope('accuracy'):
            with tf.name_scope('correct_prediction'):
                self.correct_prediction = tf.equal(self.y_pred_cls, self.y_true_cls)
            with tf.name_scope('accuracy'):
                self.accuracy = tf.reduce_mean(tf.cast(self.correct_prediction, tf.float32))
        #tf.summary.scalar('accuracy', self.accuracy)

        ##
        self.test_batch_size = test_batch_size
        self.train_batch_size = train_batch_size
        #self.init = tf.global_variables_initializer()

    def start(self, session):
        self.session = session
        self.saver = tf.train.Saver()
        
        #if os.path.exists(self.saver_dir):
        #    self.saver.restore(sess=self.session, save_path=self.save_path)
        #self.merged = tf.summary.merge_all()
        #self.train_writer = tf.train.SummaryWriter(self.save_dir + '/train', self.session.graph)
        try:
            print("Trying to restore last checkpoint...")

            # Use TensorFlow to find the latest checkpoint - if any.
            last_chk_path = tf.train.latest_checkpoint(checkpoint_dir=self.save_dir)

            # Try and load the data in the checkpoint.
            self.saver.restore(self.session, save_path=last_chk_path)

            # If we get to this point, the checkpoint was successfully loaded.
            print("Restored checkpoint from:", last_chk_path)
            print("Last global_step value: ", cnn.session.run(self.global_step))
        except:
            # If the above failed for some reason, simply
            # initialize all the variables for the TensorFlow graph.
            print("Failed to restore checkpoint. Initializing variables instead.")
            #session.run(tf.global_variables_initializer())        
            #self.session.run(self.init)
            
            #self.train_writer = tf.train.SummaryWriter(self.save_dir + '/train', self.session.graph)
            self.session.run(tf.global_variables_initializer())
             

    def optimize(self, data, num_iterations, steps_to_save):
        msg = "Iteration: {0:>6}. Accuracy: {1:>6.1%}"

        start_time = time.time()
        #with tf.name_scope('accuracy'):
        for i in range(self.total_iterations, self.total_iterations + num_iterations):
            x_batch, y_true_batch = random_batch(data, self.train_batch_size)
            feed = {X:x_batch, y_true:y_true_batch}
            self.session.run(self.optimizer, feed_dict=feed)
            #summary, acc = self.session.run([self.merged, self.optimizer], feed_dict=feed)
            #self.train_writer.add_summary(summary, i)#self.session.run(self.global_step)

            if i % steps_to_save == 0:
                #tf.train.write_graph(self.session.graph_def, self.save_path, "test.pb", False)
                self.saver.save(sess=self.session, save_path=self.save_path)
                acc = self.session.run(self.accuracy, feed)
                print(msg.format(i + 1, acc))
        end_time = time.time()
        
        self.total_iterations += num_iterations
        time_dif = end_time - start_time

        print("Time usage: " + str(timedelta(seconds=int(round(time_dif)))))

    def test_accuracy(self, data, show_sample_errors=False, show_confusion_matrix=False, draw=False):
        n = len(data.images)
        cls_pred = np.zeros(shape=n, dtype=np.int)

        i = 0
        while i < n:
            j = min(i + self.test_batch_size, n)

            images = data.images[i:j, :]
            labels = data.labels[i:j, :]

            feed = {X:images, y_true:labels}
            cls_pred[i:j] = self.session.run(self.y_pred_cls, feed_dict=feed)

            i = j
        cls_true = data.cls

        correct = (cls_true == cls_pred)
        correct_sum = correct.sum()
        acc = float(correct_sum) / n

        msg = "Accuracy on Test-Set: {0:.1%}"
        print(msg.format(acc))

        if show_sample_errors:
            print("Sample errors:")
            #plot_example_errors(cls_pred=cls_pred, correct=correct)

        if show_confusion_matrix:
            print("Confusion matrix:")
            num_classes = self.fc_layers[-1].get_shape()[1].value
            plot_confusion_matrix(data=data, cls_pred=cls_pred, num_classes=num_classes)

    def save_model(self):
        saver = tf.train.Saver()
        save_path = os.path.join(self.save_dir, 'best_validation')

    def restore(self):
        self.saver.restore(sess=self.session, save_path=self.save_path)
        
    #def variable_summaries(self, var):
    #    """Attach a lot of summaries to a Tensor (for TensorBoard visualization)."""
    #    with tf.name_scope('summaries'):
    #        mean = tf.reduce_mean(var)
    #        tf.summary.scalar('mean', mean)
    #        with tf.name_scope('stddev'):
    #            stddev = tf.sqrt(tf.reduce_mean(tf.square(var - mean)))
    #            tf.summary.scalar('stddev', stddev)
    #            tf.summary.scalar('max', tf.reduce_max(var))
    #            tf.summary.scalar('min', tf.reduce_min(var))
    #            tf.summary.histogram('histogram', var)

##
def pre_process_image(image, img_size_cropped, num_channels, training):
    # This function takes a single image as input,
    # and a boolean whether to build the training or testing graph.
    
    if training:
        # For training, add the following to the TensorFlow graph.

        # Randomly crop the input image.
        image = tf.random_crop(image, size=[img_size_cropped, img_size_cropped, num_channels])

        # Randomly flip the image horizontally.
        image = tf.image.random_flip_left_right(image)
        
        # Randomly adjust hue, contrast and saturation.
        image = tf.image.random_hue(image, max_delta=0.05)
        image = tf.image.random_contrast(image, lower=0.3, upper=1.0)
        image = tf.image.random_brightness(image, max_delta=0.2)
        image = tf.image.random_saturation(image, lower=0.0, upper=2.0)

        # Some of these functions may overflow and result in pixel
        # values beyond the [0, 1] range. It is unclear from the
        # documentation of TensorFlow 0.10.0rc0 whether this is
        # intended. A simple solution is to limit the range.

        # Limit the image pixels between [0, 1] in case of overflow.
        image = tf.minimum(image, 1.0)
        image = tf.maximum(image, 0.0)
    else:
        # For training, add the following to the TensorFlow graph.

        # Crop the input image around the centre so it is the same
        # size as images that are randomly cropped during training.
        image = tf.image.resize_image_with_crop_or_pad(image,
                                                       target_height=img_size_cropped,
                                                       target_width=img_size_cropped)

    return image


def pre_process(images, img_size_cropped, num_channels, training):
    # Use TensorFlow to loop over all the input images and call
    # the function above which takes a single image as input.
    images = tf.map_fn(lambda image: pre_process_image(image, img_size_cropped, num_channels, training), images)
    return images
##

#load data
cifar10.maybe_download_and_extract()


class_names = cifar10.load_class_names()
print(class_names)

images_train, cls_train, labels_train = cifar10.load_training_data()
images_test, cls_test, labels_test = cifar10.load_test_data()

print("Size of:")
print("- Training-set:\t\t{}".format(len(images_train)))
print("- Test-set:\t\t{}".format(len(images_test)))

img_size_cropped = 24

##
X = tf.placeholder(tf.float32, shape=[None, img_size, img_size, num_channels], name='X')
images = pre_process(images=X, img_size_cropped=img_size_cropped, num_channels=num_channels,training=True)

#distorted_images = pre_process(images=images, img_size_cropped=img_size_cropped,num_channels=num_channels,training=True) #no es necesario

y_true = tf.placeholder(tf.float32, shape=[None, num_classes], name='y_true')
y_true_cls = tf.argmax(y_true, dimension=1)
##

##

cnn = Cnn(images, num_channels, y_true, y_true_cls)

cnn.add_conv_layer(filter_size=5, num_filters=64, scope='conv1', batch_normalize=True)
cnn.add_conv_layer(filter_size=5, num_filters=64, scope='conv2')
cnn.flatten_layer()
cnn.add_fc_layer(num_outputs=256, scope='fully1')
cnn.add_fc_layer(num_outputs=num_classes, scope='fully2', use_relu=False)

cnn.config(train_batch_size=64, test_batch_size=256)
cnn.start(tf.Session())
##
#images_train = images_train.astype(np.float32)
data_train = type('obj', (object,), {'images_train' : images_train, 'labels_train' : labels_train})


#cnn.restore()
cnn.optimize(data=data_train, num_iterations=750000, steps_to_save = 10000)

##
cls = np.argmax(labels_test, axis=1)
data_test = type('obj', (object,), {
    'images' : images_test, 
    'labels' : labels_test,
    'cls' : cls,})
cnn.test_accuracy(data_test)

#global_steps no se como usarlo.. lo reseteo a cada rato
#print(cnn.session.run(cnn.global_step))


#test_writer = tf.train.SummaryWriter(FLAGS.summaries_dir + '/test')

