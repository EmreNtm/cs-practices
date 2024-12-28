# -*- coding: utf-8 -*-
"""
Created on Sun Oct 25 13:50:05 2020

@author: Emre

outputu ve inputu çokla

"""            

import NeuralNetwork as NN

aLetter = [0, 0, 1, 1, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0,
         0, 0, 1, 0, 1, 0, 0,
         0, 0, 1, 0, 1, 0, 0,
         0, 1, 1, 1, 1, 1, 0,
         0, 1, 0, 0, 0, 1, 0,
         0, 1, 0, 0, 0, 1, 0,
         1, 1, 1, 0, 1, 1, 1]

bLetter = [1, 1, 1, 1, 1, 1, 0,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 1, 1, 1, 1, 0,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 1,
         1, 1, 1, 1, 1, 1, 0]

cLetter = [0, 0, 1, 1, 1, 1, 1,
         0, 1, 0, 0, 0, 0, 1,
         1, 0, 0, 0, 0, 0, 0,
         1, 0, 0, 0, 0, 0, 0,
         1, 0, 0, 0, 0, 0, 0,
         1, 0, 0, 0, 0, 0, 0,
         1, 0, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 1,
         0, 0, 1, 1, 1, 1, 0]

dLetter = [1, 1, 1, 1, 1, 0, 0,
         0, 1, 0, 0, 0, 1, 0,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 1, 0,
         1, 1, 1, 1, 1, 0, 0]

eLetter = [1, 1, 1, 1, 1, 1, 1,
         0, 1, 0, 0, 0, 0, 1,
         0, 1, 0, 0, 0, 0, 0,
         0, 1, 0, 1, 0, 0, 0,
         0, 1, 1, 1, 0, 0, 0,
         0, 1, 0, 1, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 1,
         1, 1, 1, 1, 1, 1, 1]

jLetter = [0, 0, 0, 1, 1, 1, 1,
         0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 1, 0,
         0, 1, 0, 0, 0, 1, 0,
         0, 1, 0, 0, 0, 1, 0,
         0, 0, 1, 1, 1, 0, 0]

kLetter = [1, 1, 1, 0, 0, 1, 1,
         0, 1, 0, 0, 1, 0, 0,
         0, 1, 0, 1, 0, 0, 0,
         0, 1, 1, 0, 0, 0, 0,
         0, 1, 1, 0, 0, 0, 0,
         0, 1, 0, 1, 0, 0, 0,
         0, 1, 0, 0, 1, 0, 0,
         0, 1, 0, 0, 0, 1, 0,
         1, 1, 1, 0, 0, 1, 1]

##########################################################

a1Letter = [0, 0, 0, 1, 0, 0, 0,
          0, 0, 0, 1, 0, 0, 0,
          0, 0, 0, 1, 0, 0, 0,
          0, 0, 1, 0, 1, 0, 0,
          0, 0, 1, 0, 1, 0, 0,
          0, 1, 0, 0, 0, 1, 0,
          0, 1, 1, 1, 1, 1, 0,
          0, 1, 0, 0, 0, 1, 0,
          0, 1, 0, 0, 0, 1, 0]

b1Letter = [1, 1, 1, 1, 1, 1, 0,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 1, 1, 1, 1, 1, 0,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 1, 1, 1, 1, 1, 0]

c1Letter = [0, 0, 1, 1, 1, 0, 0,
          0, 1, 0, 0, 0, 1, 0,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 1, 0,
          0, 0, 1, 1, 1, 0, 0]

d1Letter = [1, 1, 1, 1, 1, 0, 0,
          1, 0, 0, 0, 0, 1, 0,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 1, 0,
          1, 1, 1, 1, 1, 0, 0]

e1Letter = [1, 1, 1, 1, 1, 1, 1,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 1, 1, 1, 1, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 1, 1, 1, 1, 1, 1]

j1Letter = [0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 1, 0, 0, 0, 1, 0,
          0, 1, 0, 0, 0, 1, 0,
          0, 0, 1, 1, 1, 0, 0,]

k1Letter = [1, 0, 0, 0, 0, 1, 0,
          1, 0, 0, 0, 1, 0, 0,
          1, 0, 0, 1, 0, 0, 0,
          1, 0, 1, 0, 0, 0, 0,
          1, 1, 0, 0, 0, 0, 0,
          1, 0, 1, 0, 0, 0, 0,
          1, 0, 0, 1, 0, 0, 0,
          1, 0, 0, 0, 1, 0, 0,
          1, 0, 0, 0, 0, 1, 0]

############################################################

a2Letter = [0, 0, 0, 1, 0, 0, 0,
          0, 0, 0, 1, 0, 0, 0,
          0, 0, 1, 0, 1, 0, 0,
          0, 0, 1, 0, 1, 0, 0,
          0, 1, 0, 0, 0, 1, 0,
          0, 1, 1, 1, 1, 1, 0,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 1, 0, 0, 0, 1, 1]

b2Letter = [1, 1, 1, 1, 1, 1, 0,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 1, 1, 1, 1, 0,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 0, 1,
          1, 1, 1, 1, 1, 1, 0]

c2Letter = [0, 0, 1, 1, 1, 0, 1,
          0, 1, 0, 0, 0, 1, 1,
          1, 0, 0, 0, 0, 0, 1,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 1, 0,
          0, 0, 1, 1, 1, 0, 0]

d2Letter = [1, 1, 1, 1, 1, 0, 0,
          0, 1, 0, 0, 0, 1, 0,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 0, 1, 0,
          1, 1, 1, 1, 1, 0, 0]

e2Letter = [1, 1, 1, 1, 1, 1, 1,
          0, 1, 0, 0, 0, 0, 1,
          0, 1, 0, 0, 1, 0, 0,
          0, 1, 1, 1, 1, 0, 0,
          0, 1, 0, 0, 1, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 0,
          1, 0, 0, 0, 0, 0, 1,
          1, 1, 1, 1, 1, 1, 1]

j2Letter = [0, 0, 0, 0, 1, 1, 1,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 0, 0, 0, 0, 1, 0,
          0, 1, 0, 0, 0, 1, 0,
          0, 0, 1, 1, 1, 0, 0]

k2Letter = [1, 1, 1, 0, 0, 1, 1,
          0, 1, 0, 0, 0, 1, 0,
          0, 1, 0, 0, 1, 0, 0,
          0, 1, 0, 1, 0, 0, 0,
          0, 1, 1, 0, 0, 0, 0,
          0, 1, 0, 1, 0, 0, 0,
          0, 1, 0, 0, 1, 0, 0,
          0, 1, 0, 0, 0, 1, 0,
          1, 1, 1, 0, 0, 1, 1]

inputLength = len(aLetter)
outputAmount = 7
bias = 1
threshold = 1
learningRate = 0.0015
p1 = NN.Perceptron(inputLength, outputAmount, bias, threshold, learningRate)

inputs = [aLetter, bLetter, cLetter, dLetter, eLetter, jLetter, kLetter,
          a1Letter, b1Letter, c1Letter, d1Letter, e1Letter, j1Letter, k1Letter,
          a2Letter, b2Letter, c2Letter, d2Letter, e2Letter, j2Letter, k2Letter]
outputs = [[1, 0, 0, 0, 0, 0, 0],
           [0, 1, 0, 0, 0, 0, 0],
           [0, 0, 1, 0, 0, 0, 0],
           [0, 0, 0, 1, 0, 0, 0],
           [0, 0, 0, 0, 1, 0, 0],
           [0, 0, 0, 0, 0, 1, 0],
           [0, 0, 0, 0, 0, 0, 1],
           [1, 0, 0, 0, 0, 0, 0],
           [0, 1, 0, 0, 0, 0, 0],
           [0, 0, 1, 0, 0, 0, 0],
           [0, 0, 0, 1, 0, 0, 0],
           [0, 0, 0, 0, 1, 0, 0],
           [0, 0, 0, 0, 0, 1, 0],
           [0, 0, 0, 0, 0, 0, 1],
           [1, 0, 0, 0, 0, 0, 0],
           [0, 1, 0, 0, 0, 0, 0],
           [0, 0, 1, 0, 0, 0, 0],
           [0, 0, 0, 1, 0, 0, 0],
           [0, 0, 0, 0, 1, 0, 0],
           [0, 0, 0, 0, 0, 1, 0],
           [0, 0, 0, 0, 0, 0, 1]]

p1.takeInputs(inputs, outputs)

#p1.printWeights()
print("_________________\n")
epoch = 50
p1.learn(epoch)
print("_________________\n")
#p1.printWeights()
#p1.printAccuracy(inputs, outputs)
p1.plotGraph()
"""
p1.guess(aLetter, [1, 0, 0, 0, 0, 0, 0])
p1.guess(bLetter, [0, 1, 0, 0, 0, 0, 0])
p1.guess(cLetter, [0, 0, 1, 0, 0, 0, 0])
p1.guess(dLetter, [0, 0, 0, 1, 0, 0, 0])
p1.guess(eLetter, [0, 0, 0, 0, 1, 0, 0])
p1.guess(jLetter, [0, 0, 0, 0, 0, 1, 0])
p1.guess(kLetter, [0, 0, 0, 0, 0, 0, 1])
print("_________________\n")
p1.guess(a1Letter, [1, 0, 0, 0, 0, 0, 0])
p1.guess(b1Letter, [0, 1, 0, 0, 0, 0, 0])
p1.guess(c1Letter, [0, 0, 1, 0, 0, 0, 0])
p1.guess(d1Letter, [0, 0, 0, 1, 0, 0, 0])
p1.guess(e1Letter, [0, 0, 0, 0, 1, 0, 0])
p1.guess(j1Letter, [0, 0, 0, 0, 0, 1, 0])
p1.guess(k1Letter, [0, 0, 0, 0, 0, 0, 1])
print("_________________\n")
p1.guess(a2Letter, [1, 0, 0, 0, 0, 0, 0])
p1.guess(b2Letter, [0, 1, 0, 0, 0, 0, 0])
p1.guess(c2Letter, [0, 0, 1, 0, 0, 0, 0])
p1.guess(d2Letter, [0, 0, 0, 1, 0, 0, 0])
p1.guess(e2Letter, [0, 0, 0, 0, 1, 0, 0])
p1.guess(j2Letter, [0, 0, 0, 0, 0, 1, 0])
p1.guess(k2Letter, [0, 0, 0, 0, 0, 0, 1])
"""