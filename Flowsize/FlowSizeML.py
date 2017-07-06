#This File is machine learning part for flow size
#Input Format: accurate value,minimum counter value,second minimum counter value,maximum counter value(splitted by ",")
#Output Format: mean 1 | mean 2 | .....scale 1 | scale 2 | ......coefficient 1 | coefficient 2 ......(splitted by "\n")
import sys
import numpy as np
import scipy as sp
from sklearn import preprocessing
from sklearn import linear_model

train_mat = np.loadtxt(open(sys.argv[1], "rb"),delimiter=" ",skiprows=0)
f = open(sys.argv[2], "wb")

train_Y = train_mat[:, 0].copy()
train_X = train_mat[:, 1:2].copy() # only use two counters
feature_scaler = preprocessing.StandardScaler().fit(train_X)
train_X = feature_scaler.transform(train_X)

reg = linear_model.Ridge(alpha=.5)
reg.fit(train_X, train_Y)

for i in range(train_X.shape[1]):
	print >> f, feature_scaler.mean_[i]
print >> f, 0
print >> f, 0
print >> f, 0
for i in range(train_X.shape[1]):
	print >> f, feature_scaler.scale_[i]
print >> f, 1
print >> f, 1
print >> f, 1
for i in range(train_X.shape[1]):
	print >> f, reg.coef_[i]
print >> f, 0
print >> f, 0
print >> f, 0

# test_mat = np.loadtxt(open("cm_test_sort.dat", "rb"),delimiter=",",skiprows=0)

# test_Y = test_mat[:, 0].copy()
# test_X = test_mat[:, 1:5].copy()
# min_Y = test_X[:, 0].copy()
# dif = test_X[:, 1]-test_X[:, 0]
# for i in range(test_X.shape[1]):
# 	test_X[:, i] = test_X[:, i]*dif
# test_X = np.column_stack((test_X, min_Y))
# test_X = feature_scaler.transform(test_X)

# pre_Y = reg.predict(test_X)

# f = open("cmp.dat", 'w')
# for i in range(test_Y.shape[0]):
#     print >> f, test_Y[i], min_Y[i], pre_Y[i]
