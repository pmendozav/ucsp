import tensorflow as tf
import numpy as np
import time
from datetime import timedelta

def new_weights(shape, name = 'weights'):
    return tf.get_variable(name, initializer=tf.truncated_normal(shape, stddev=0.05))

def new_biases(length, name = 'biases'):
    return tf.Variable(tf.constant(0.05, shape=[length]), 
                       name = name)

def new_conv_layer(input,              # The previous layer.
                   num_input_channels, # Num. channels in prev. layer.
                   filter_size,        # Width and height of each filter.
                   num_filters,        # Number of filters.
                   layer_name,
                   strides_conv = [1, 1, 1, 1],
                   use_pooling=True,
                   ksize_pooling = [1, 2, 2, 1],
                   strides_pooling = [1, 2, 2, 1]):  # Use 2x2 max-pooling.
    with tf.variable_scope(layer_name, reuse=False):
        shape = [filter_size, filter_size, num_input_channels, num_filters]

        weights = new_weights(shape = shape)

        biases = new_biases(length=num_filters)

        layer = tf.nn.conv2d(input = input,
                             filter = weights,
                             strides = strides_conv,
                             padding = 'SAME',
                             name = 'layer')
        layer += biases

        if use_pooling:
            layer = tf.nn.max_pool(value = layer,
                                   ksize = ksize_pooling,
                                   strides = strides_pooling,
                                   padding = 'SAME',
                                   name = 'layer')
        layer = tf.nn.relu(layer, name = 'layer')

    return layer


def flatten_layer(layer, layer_name = 'flatten_layer'):
    with tf.variable_scope(layer_name, reuse=False):
        layer_shape = layer.get_shape()

        num_features = layer_shape[1:4].num_elements()
        layer_flat = tf.reshape(layer, [-1, num_features])

    return layer_flat, num_features

def new_fc_layer(input, 
                 num_inputs,
                 num_outputs,
                 layer_name,
                 use_relu=True):
    with tf.variable_scope(layer_name, reuse=False):
        weights = new_weights(shape=[num_inputs, num_outputs])
        biases = new_biases(length=num_outputs)

        #layer = tf.matmul(input, weights) + biases
        if use_relu:
            layer = tf.nn.relu(tf.matmul(input, weights) + biases, name='layer')
        else:
            layer = tf.nn.bias_add(tf.matmul(input, weights), biases, data_format=None, name='layer')

        #layer = tf.get_variable('xxx', initializer=_layer)
    return layer