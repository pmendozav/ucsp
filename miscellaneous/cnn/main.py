import matplotlib.pyplot as plt
import tensorflow as tf
import numpy as np
from sklearn.metrics import confusion_matrix
import time
from datetime import timedelta
import math
import os

import source.helper as ch
import source.nn as nn

from auxiliary.cifar10 import img_size, num_channels, num_classes

##
filter_size1 = 5
filter_size2 = 5
num_filters1 = 64
num_filters2 = 64
fc_size1 = 256
fc_size2 = 128

data = ch.load_data_cifar10()
##

##
#TODO: add strides and pooling size to layers (build_nn function)
layers = [
    {
        'type' : 1,
        'input_pos' : -1,
        'layer_name' : 'layer_conv1',
        'channels' : num_channels,
        'filter_size' : filter_size1,
        'num_filters' : num_filters1,
        'use_pooling' : True,
    },
    {
        'type' : 1,
        'input_pos' : 0,
        'layer_name' : 'layer_conv2',
        'channels' : num_filters1,
        'filter_size' : filter_size2,
        'num_filters' : num_filters2,
        'use_pooling' : True,
    },
    {
        'type' : 0,
        'input_pos' : -1,
        'layer_name' : 'layer_fc1',
        'num_inputs' : 0, #the first is not important
        'num_outputs' : fc_size1,
        'use_relu' : True,
    },
    {
        'type' : 0,
        'input_pos' : 0,
        'layer_name' : 'layer_fc2',
        'num_inputs' : fc_size1,
        'num_outputs' : fc_size2,
        'use_relu' : True,
    },
    {
        'type' : 0,
        'input_pos' : 1,
        'layer_name' : 'layer_fc3',
        'num_inputs' : fc_size2,
        'num_outputs' : num_classes,
        'use_relu' : False,
    },
    ]

cfg = {
    'img_size' : img_size,
    'num_channels' : num_channels,
    'num_classes' : num_classes,
    'learning_rate' : 1e-4,
    'folder' : 'checkpoints/',
    'file_name' : 'cifar10_cnn',
    'session' : tf.Session(),
    }

train_batch_size = 64
test_batch_size = 256
num_iterations = 1000
##

ojala = nn.Ojala()

ojala.build_nn(layers, cfg)

ojala.optimize(data, num_iterations = num_iterations, train_batch_size = train_batch_size)

ojala.print_test_accuracy(data, test_batch_size= test_batch_size)

ojala.shutdown()