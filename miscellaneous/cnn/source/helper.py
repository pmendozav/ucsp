import matplotlib.pyplot as plt
import numpy as np
import math
from tensorflow.examples.tutorials.mnist import input_data

import auxiliary.cifar10
from auxiliary.cifar10 import img_size, num_channels, num_classes

def random_batch(data, train_batch_size):
    num_images = len(data.train_data)#

    idx = np.random.choice(num_images,
                           size=train_batch_size,
                           replace=False)

    x_batch = data.train_data[idx, :, :, :]
    y_batch = data.train_labels[idx, :]

    #x_batch = np.reshape(x_batch, [x_batch.shape[0], -1])

    return x_batch, y_batch

def load_data():
    data = input_data.read_data_sets('MNIST_data/', one_hot=True);
    data.test.cls = np.argmax(data.test.labels, axis=1)
    return data

def load_data_cifar10():
    auxiliary.cifar10.maybe_download_and_extract()
    images_train, cls_train, labels_train = auxiliary.cifar10.load_training_data()
    images_test, cls_test, labels_test = auxiliary.cifar10.load_test_data()

    #data = []
    #data.images_train = images_train
    #data.cls_train = cls_train
    #data.labels_train = labels_train

    #data.images_test = images_test
    #data.cls_test = cls_test
    #data.labels_test = labels_test

    data = type('obj', (object,), {
    'train_data' : images_train, 
    'train_cls' : cls_train,
    'train_labels' : labels_train,

    'test_data' : images_test,
    'test_cls' : cls_test,
    'test_labels' : labels_test,
    })

    return data


def plot_images(images, cls_true, img_shape, cls_pred=None):
    assert len(images) == len(cls_true) == 9
    
    # Create figure with 3x3 sub-plots.
    fig, axes = plt.subplots(3, 3)
    fig.subplots_adjust(hspace=0.3, wspace=0.3)

    for i, ax in enumerate(axes.flat):
        # Plot image.
        ax.imshow(images[i].reshape(img_shape), cmap='binary')

        # Show true and predicted classes.
        if cls_pred is None:
            xlabel = "True: {0}".format(cls_true[i])
        else:
            xlabel = "True: {0}, Pred: {1}".format(cls_true[i], cls_pred[i])

        # Show the classes as the label on the x-axis.
        ax.set_xlabel(xlabel)
        
        # Remove ticks from the plot.
        ax.set_xticks([])
        ax.set_yticks([])
    
    # Ensure the plot is shown correctly with multiple plots
    # in a single Notebook cell.
    plt.show()

def plot_conv_weights(session, weights, input_channel=0):
    # Assume weights are TensorFlow ops for 4-dim variables
    # e.g. weights_conv1 or weights_conv2.
    
    # Retrieve the values of the weight-variables from TensorFlow.
    # A feed-dict is not necessary because nothing is calculated.
    w = session.run(weights)

    # Get the lowest and highest values for the weights.
    # This is used to correct the colour intensity across
    # the images so they can be compared with each other.
    w_min = np.min(w)
    w_max = np.max(w)

    # Number of filters used in the conv. layer.
    num_filters = w.shape[3]

    # Number of grids to plot.
    # Rounded-up, square-root of the number of filters.
    num_grids = math.ceil(math.sqrt(num_filters))
    
    # Create figure with a grid of sub-plots.
    fig, axes = plt.subplots(num_grids, num_grids)

    # Plot all the filter-weights.
    for i, ax in enumerate(axes.flat):
        # Only plot the valid filter-weights.
        if i<num_filters:
            # Get the weights for the i'th filter of the input channel.
            # See new_conv_layer() for details on the format
            # of this 4-dim tensor.
            img = w[:, :, input_channel, i]

            # Plot image.
            ax.imshow(img, vmin=w_min, vmax=w_max,
                      interpolation='nearest', cmap='seismic')
        
        # Remove ticks from the plot.
        ax.set_xticks([])
        ax.set_yticks([])
    
    # Ensure the plot is shown correctly with multiple plots
    # in a single Notebook cell.
    plt.show()
