# -*- coding: utf-8 -*-
"""
Created on Mon Oct 26 17:45:37 2020

@author: Emre
"""

import matplotlib.pyplot as plt

class Neuron:
    def __init__(self, inputAmount, bias):
        self.inputAmount = inputAmount
        self.bias = bias
        self.weights = []
        
        for i in range(inputAmount):
            self.weights.append(0)
        self.weights.append(0)

class Perceptron:
    def __init__(self, inputAmount, outputAmount, bias, threshold, learningRate):
        self.inputAmount = inputAmount
        self.outputAmount = outputAmount
        self.bias = bias
        self.threshold = threshold
        self.learningRate = learningRate
        self.neurons = []
        self.accuracies = [[], []]
        
        for i in range(outputAmount):
            self.neurons.append(Neuron(self.inputAmount, self.bias))

    def takeInputs(self, inputs, outputs):
        self.inputs = inputs
        self.outputs = outputs
        
    def learn(self, epoch):
        print("#LEARN#")
        for i in range(epoch):
            for l in range(len(self.inputs)):
                #print(l)
                for j in range(self.outputAmount):
                    sum_ = 0
                    for k in range(self.inputAmount):
                        sum_ += self.inputs[l][k] * self.neurons[j].weights[k]
                    sum_ += self.bias * self.neurons[j].weights[self.inputAmount]
                    o = sum_
                    if o >= self.threshold:
                        o = 1
                    #elif o <= -self.threshold:
                    #   o = -1
                    else:
                        o = 0
                    #print("sum:", sum_, "target:", self.outputs[l][j], "output:", o)
                    if o != self.outputs[l][j]:
                        #print("WRONG! WEIGHTS WILL CHANGE")
                        for k in range(self.inputAmount):
                            self.neurons[j].weights[k] += self.learningRate * self.outputs[l][j] * self.inputs[l][k]
                        self.neurons[j].weights[self.inputAmount] += self.learningRate * self.outputs[l][j] * self.bias
                        #print("WEIGHTS HAS BEEN CHANGED:")
                    #else:
                    #    print("CORRECT!")
            print("epoch: ", i+1)
            self.accuracies[0].append(i+1)
            self.accuracies[1].append(self.printAccuracy(self.inputs, self.outputs))
                        
    def guess(self, inputs, outputs):
        #print("#GUESS#")
        result = []
        for j in range(self.outputAmount):
            sum_ = 0
            for k in range(self.inputAmount):
                sum_ += inputs[k] * self.neurons[j].weights[k]
            sum_ += self.bias * self.neurons[j].weights[self.inputAmount]
            o = sum_
            if o >= self.threshold:
                o = 1
            #elif o <= -self.threshold:
            #    o = -1
            else:
                o = 0
            result.append(o)
            #print("sum:", sum_, "target:", outputs[j], "output:", o)
        return result
            
    def printWeights(self):
        for neuron in self.neurons:
            print("_-_-_-_-_")
            print(neuron.weights)
            
    def printAccuracy(self, inputs, outputs):
        correct = 0
        total = 0
        for i in range(len(inputs)):
            total += 1
            if (self.guess(inputs[i], outputs[i]) == outputs[i]):
                correct += 1
        print("Accuracy: ", correct/total)
        return correct/total
       
    def plotGraph(self):
        x = self.accuracies[0]
        y = self.accuracies[1]
        plt.plot(x, y)
        plt.xlabel('epoch')
        plt.ylabel('accuracy')
        plt.title('Accuracy Graph for Learning Rate = '+ str(self.learningRate))
        plt.show()
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            