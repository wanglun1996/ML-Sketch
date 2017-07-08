import numpy as np
import scipy as sp
from sklearn import preprocessing
from sklearn import linear_model
from sklearn import svm
import sys
import warnings 

warnings.filterwarnings("ignore", category=DeprecationWarning)

f = open("ML", "a")

train_mat = np.loadtxt(open("./TrainSet/"+sys.argv[1], "rb"), delimiter=",", skiprows=0)

train_Y = train_mat[:, 0].copy()
train_X = train_mat[:, 1:].copy()

clf = linear_model.LinearRegression()
clf.fit(train_X, train_Y)

test_mat = np.loadtxt(open("./TestSet/"+sys.argv[1], "rb"), delimiter=",", skiprows=0);

test_X = test_mat[1:].copy();

res1 = (2**(clf.predict(test_X)))

print res1
#method with constant
ones = np.zeros((train_X.shape[0], 1))
for i in range(ones.shape[0]):
	ones[i] = 1
print train_X.shape, ones.shape
train_X = np.column_stack((train_X, ones))

print train_X

test_X = np.append(test_X, 1)
print test_X

clf.fit(train_X, train_Y)

res2 = (2**(clf.predict(test_X)))

print clf.coef_

print res2

#method with low and high seperated with constant

train_X = np.zeros((train_mat.shape[0], 3))

for i in range(0, 19):
	train_X[:, 0] += train_mat[:, 2*i+1]
	train_X[:, 1] += train_mat[:, 2*i+2]
	train_X[:, 2] = 1

train_X[:, :1] /= (train_mat.shape[1]-1)/2
train_X[:, :1] = 2**train_X[:, :1]

train_Y = 2**train_Y

clf.fit(train_X, train_Y)
print clf.coef_

test_X = np.zeros(3)

for i in range(0, 19):
	test_X[0] += test_mat[2*i+1]
	test_X[1] += test_mat[2*i+2]
	test_X[2] = 1

test_X[:1] /= (test_mat.shape[0]-1)/2
test_X[:1] = 2**test_X[:1]

res3 = (clf.predict(test_X))

print res3

#method with low and high seperated without constant

train_X = np.zeros((train_mat.shape[0], 2))

for i in range(0, 19):
	train_X[:, 0] += train_mat[:, 2*i+1]
	train_X[:, 1] += train_mat[:, 2*i+2]

train_X[:, :1] /= (train_mat.shape[1]-1)/2
train_X[:, :1] = 2**train_X[:, :1]

clf.fit(train_X, train_Y)
print clf.coef_

test_X = np.zeros(2)

for i in range(0, 19):
	test_X[0] += test_mat[2*i+1]
	test_X[1] += test_mat[2*i+2]

test_X[:1] /= (test_mat.shape[0]-1)/2
test_X[:1] = 2**test_X[:1]

res3 = (clf.predict(test_X))

print res3
