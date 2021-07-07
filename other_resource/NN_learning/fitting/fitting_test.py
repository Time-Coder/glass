import sys
sys.path.append('../')
from tfnn import *

P, T = sin_train_data()

trainParam = TrainParam()
trainParam.epochs = 20000
trainParam.learning_rate = 0.1
trainParam.plot_flag = True
trainParam.optimizer = 'adam'
trainParam.learning_rate_func = 'exp_decay'
trainParam.learning_rate_decay = 0.99
trainParam.discrete = False

net = BPNN([1, 10, 1], trainParam)
net.train(P, T)
# net.show()