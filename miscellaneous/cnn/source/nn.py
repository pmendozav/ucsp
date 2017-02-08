import tensorflow as tf
import numpy as np
import time
from datetime import timedelta
import os

import source.helper as hp
import source.nn_common as nnh

class Ojala:
    def __init__(self):
        self.x = None
        self.y_true = None
        self.y_true_cls = None
        self.session = None
        self.optimizer = None
        self.accuracy = None
        self.correct_prediction = None
        self.global_step = None
        self.saver = None
        self.save_path = None
        self.y_pred_cls = None
        self.cost = None
        self.save_dir = None
        self.layers = []

    #TODO: add 100 (print count) and 500 (save count) for cfg
    def optimize(self,
                 data,
                 num_iterations,
                 train_batch_size):

        start_time = time.time()

        for i in range(num_iterations):

            x_batch, y_true_batch = hp.random_batch(data, train_batch_size)
        
            feed_dict_train = {self.x: x_batch,
                               self.y_true: y_true_batch}

            i_global, _ = self.session.run([self.global_step, self.optimizer], feed_dict=feed_dict_train)

            if (i_global % 100 == 0) or (i == num_iterations - 1):
                acc = self.session.run(self.accuracy, feed_dict=feed_dict_train)
                msg = "Optimization Iteration: {0:>6}, Training Accuracy: {1:>6.1%}"
                print(msg.format(i_global, acc))

            if (i_global % 500 == 0) or (i == num_iterations - 1):
                self.saver.save(self.session,
                           save_path=self.save_path,
                           global_step=self.global_step)
                print("Saved checkpoint..")

        end_time = time.time()
        time_dif = end_time - start_time
        print("Time usage: " + str(timedelta(seconds=int(round(time_dif)))))

    def print_test_accuracy(self,
                            data,
                            test_batch_size):

        num_test = len(data.test_data)

        cls_pred = np.zeros(shape=num_test, dtype=np.int)

        i = 0
        while i < num_test:
            # The ending index for the next batch is denoted j.
            j = min(i + test_batch_size, num_test)

            # Get the images from the test-set between index i and j.
            images = data.test_data[i:j, :]

            # Get the associated labels.
            labels = data.test_labels[i:j, :]

            # Create a feed-dict with these images and labels.
            feed_dict = {self.x: images,
                         self.y_true: labels}

            # Calculate the predicted class using TensorFlow.
            cls_pred[i:j] = self.session.run(self.y_pred_cls, feed_dict=feed_dict)

            # Set the start-index for the next batch to the
            # end-index of the current batch.
            i = j

        # Convenience variable for the true class-numbers of the test-set.
        #cls_true = data.test.cls
        cls_true = data.test_cls

        # Create a boolean array whether each image is correctly classified.
        correct = (cls_true == cls_pred)

        # Calculate the number of correctly classified images.
        # When summing a boolean array, False means 0 and True means 1.
        correct_sum = correct.sum()

        # Classification accuracy is the number of correctly classified
        # images divided by the total number of images in the test-set.
        acc = float(correct_sum) / num_test

        # Print the accuracy.
        msg = "Accuracy on Test-Set: {0:.1%} ({1} / {2})"
        print(msg.format(acc, correct_sum, num_test))

    def shutdown(self):
        self.session.close()

    def get_weights_variable(self,
                             layer_name):
        variable = None
        with tf.variable_scope(layer_name, reuse=True):
            try:
                variable = tf.get_variable('weights')#layer_conv1/weights:0
            except ValueError:
                print('weights variable does not exist')
        return variable

    def get_layer_output(self,
                         layer_name):
        variable = None
        with tf.variable_scope(layer_name, reuse=True):
            try:
                variable = tf.get_variable('layer')#layer_conv1/weights:0
            except ValueError:
                print('layer variable does not exist')
        return variable

    def build_nn(self, layers, cfg):
        ##
        img_size = cfg['img_size']
        num_channels = cfg['num_channels']
        num_classes = cfg['num_classes']
        self.x = tf.placeholder(tf.float32, 
                           shape=[None, img_size, img_size, num_channels], 
                           name='x')
        self.y_true = tf.placeholder(tf.float32, shape=[None, num_classes], name='y_true')
        self.y_true_cls = tf.argmax(self.y_true, dimension=1)
        ##
        last = None
        for layer in layers:
            #print("layer={}".format(layer))

            input_pos = layer['input_pos']
            layer_name = layer['layer_name']

            if layer['type'] is 1: # is conv
                channels = layer['channels']
                filter_size = layer['filter_size']
                num_filters = layer['num_filters']
                use_pooling = layer['use_pooling']

                if input_pos is -1: #first conv-layout
                    input = self.x
                else:
                    input = self.layers[-1];

                layer = nnh.new_conv_layer(input,
                                           channels,
                                           filter_size,
                                           num_filters,
                                           layer_name)
            else: #is fc
                num_inputs = layer['num_inputs']
                num_outputs = layer['num_outputs']
                use_relu = layer['use_relu']

                if input_pos is -1: #first fc-layout
                    input, num_inputs = nnh.flatten_layer(self.layers[-1])
                else:
                    input = self.layers[-1]

                layer = nnh.new_fc_layer(input,
                                         num_inputs,
                                         num_outputs,
                                         layer_name,
                                         use_relu)
            self.layers.append(layer)
        ##
        last = self.layers[-1]
        self.y_pred = tf.nn.softmax(last)
        self.y_pred_cls = tf.argmax(self.y_pred, dimension=1)

        self.global_step = tf.Variable(initial_value=0,
                                       name='global_step', trainable=False)

        self.cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=last,
                                                                     labels=self.y_true)
        self.cost = tf.reduce_mean(self.cross_entropy)
        rate = cfg['learning_rate']
        self.optimizer = tf.train.AdamOptimizer(learning_rate=rate).minimize(self.cost,
                                                                             global_step=self.global_step)

        self.correct_prediction = tf.equal(self.y_pred_cls, self.y_true_cls)
        self.accuracy = tf.reduce_mean(tf.cast(self.correct_prediction, tf.float32))

        self.saver = tf.train.Saver()
        self.save_dir = cfg['folder']
        if not os.path.exists(self.save_dir):
            os.makedirs(self.save_dir)
        self.save_path = os.path.join(self.save_dir, cfg['file_name'])

        self.session = cfg['session']

        try:
            print("Trying to restore last checkpoint..")
            last_chk_path = tf.train.latest_checkpoint(checkpoint_dir = self.save_dir)
            self.saver.restore(self.session, save_path = last_chk_path)
            print("Restored checkpoint from:", last_chk_path)
        except:
            print("Failed to restore checkpoint. Initializing variables instead.")
            self.session.run(tf.global_variables_initializer())

#for e in dict:
#    print(dict[e])

#r = [
#    {'a':4, 'b':15, },
#    {'d':5, 'e':8, }
#    ]

#for e in r:
#    print(e)