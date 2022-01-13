import pickle
import numpy as np
import matplotlib.pyplot as plt
import skimage.transform
from random import random
import skimage.transform
import tensorflow as tf
from tensorflow.keras.models import Sequential
import keras



def unpickle(file):
    with open(file, 'rb') as f:
        dict = pickle.load(f, encoding="latin1")
    return dict


def cifar10_classifier_random(x):
    return np.random.randint(0, 10)


def cifar10_color(X):

    Xp = []
    for image in X:
        resized_img = skimage.transform.resize(image, (1, 1, 3)) # 1x1 image with three channels
        Xp.append(resized_img)

    Xp = np.asarray(Xp)
    Xp.transpose()
    return Xp

def cifar10_naivebayes_learn(Xp, Y):
    classes = []
    for i in range(0, 10): # for dividing the images into classes
        c = []
        classes.append(c)
    index = 0
    Y = list(Y)

    for image in Xp:
        label = Y[index] # finding the corresponding label for the image
        classes[label].append(image) # and storing the image in the correct class
        index += 1

    means = []
    vars = []
    priors = []
    for c in classes:
        rgb_means = np.mean(c, axis = 0)
        means.append(rgb_means)

        variance = np.var(c, axis = 0)
        vars.append(variance)

        p = len(c)/len(Y)
        priors.append(p)

    return means, vars, priors


def cifar10_classifier_naivebayes(x, mu, sigma, p):
    # everything looks bad here so i'd assume something has gone wrong
    # i don't know what the probability output is supposed to look like

    probabilities = []
    for i in range(0, 10): # probability for each class
        Gauss_1 = normal_dist(x, mu[i][:,:,0], sigma[i][:,:,0]) # red
        Gauss_2 = normal_dist(x, mu[i][:,:,1], sigma[i][:,:,1]) # green
        Gauss_3 = normal_dist(x, mu[i][:,:,2], sigma[i][:,:,2]) # blue

        prob = Gauss_1*Gauss_2*Gauss_3*p[i]
        probabilities.append(prob)

    probabilities = np.asarray(probabilities)
    highest_prob = np.argmax(probabilities, axis = 0)

    c = highest_prob[0][0][0]  # suspiciously many indices, it's okay it's fine let's just run with it
    return c

def normal_dist(x, mu, sigma):
    prob_density = 1/np.sqrt(2*np.pi*sigma)*np.exp(-1/(2*sigma)*(x-mu)**2)
    return prob_density


def neural_network(X, onehot):

    model = Sequential([tf.keras.layers.Flatten(input_shape=(32, 32, 3)),
                        tf.keras.layers.Dense(100, activation='sigmoid'),
                        tf.keras.layers.Dense(10, activation='sigmoid')])
    # input layer
    #model.add(Dense(5, input_shape=(32, 32, 3), activation='sigmoid'))
    # output layer
    #model.add(Dense(10, activation='sigmoid'))

    opt = keras.optimizers.SGD(learning_rate=1.3)
    model.compile(optimizer=opt, loss='mse', metrics=['accuracy'])

    num_of_epochs = 100
    tr_hist = model.fit(X, onehot, epochs=num_of_epochs, verbose=1)

    plt.plot(tr_hist.history['loss'])
    plt.ylabel('loss')
    plt.xlabel('epoch')
    plt.show()

    return

def one_hot(Y):
    indices = Y
    ones = np.max(indices)+1
    onehot = np.eye(ones)[indices]
    return onehot

# Main
datadict = unpickle('/home/gifu/tty/machine_learning/cifar-10-batches-py/test_batch')

X = datadict["data"]
Y = datadict["labels"]

labeldict = unpickle('/home/gifu/tty/machine_learning/cifar-10-batches-py/batches.meta')
label_names = labeldict["label_names"]

X = X.reshape(10000, 3, 32, 32).transpose(0,2,3,1).astype("uint8")
Y = np.array(Y)

# EX 2 CODE:
#class_acc(Y, Y)

def cifar10_classifier_1nn(x, trdata, trlabels):

    distances = []
    NN = [] # as a list in case of multiple nearest neighbours
    for index in range(0, 100):
        for data in trdata[index]:
            dist = np.square(np.subtract(data, x)**2)
            distances.append(dist)


    label_index = np.argmin(distances)
    return trlabels[label_index]

def class_acc(pred, gt):

    correct = 0  # how many correct labels are found
    for label in pred:
        index = list(pred).index(label) # finding the index of the label for comparison
        if list(pred)[index] == list(gt)[index]:
            correct += 1

    accuracy = correct/len(gt) # comparing how many accurate labels were found
    print("Accuracy: {:.3f}".format(accuracy))

nn_labels = []
for image in X:
   nn_labels.append(cifar10_classifier_1nn(image, X, Y))
class_acc(nn_labels, Y)



#EX 3 CODE:
Xp = cifar10_color(X)
mu, sigma, p = cifar10_naivebayes_learn(Xp, Y)

naivebayes_labels = []
for image in Xp:
    label = cifar10_classifier_naivebayes(image, mu, sigma, p)
    naivebayes_labels.append(label)

class_acc(naivebayes_labels, Y)

onehot = one_hot(Y)
X = X/255
neural_network(X, onehot)

for i in range(X.shape[0]):
    # Show some images randomly
    if random() > 0.999:
        plt.figure(1);
        plt.clf()
        plt.imshow(X[i])
        plt.title(f"Image {i} label={label_names[Y[i]]} (num {Y[i]})")
        plt.pause(1)
