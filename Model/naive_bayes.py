import logging
from pdf import *

class NaiveBayesPredictor(object):

    def __init__(self, test_set, seperated_classes):
        self.correct = 0
        self.test_set_len = len(test_set)
        self.descriptors = seperated_classes.class_descriptors
        
        for input in test_set:
            prediction = self.__make_prediction(input.tolist())
            if prediction == input.class_id:
                self.correct += 1

    def __calculate_class_probability(self, input, descriptor):
        probability = 1
        idx = 0
        for mean, stdev in descriptor.params:
            x = input[idx]
            probability *= gaussian_pdf(x, mean, stdev)
            idx += 1
        return probability 

    def __make_prediction(self, input):
        best_prob = None
        best_id = None
        for descriptor in self.descriptors:
            prob = self.__calculate_class_probability(input, descriptor)
            if best_prob is None or prob > best_prob:
                best_prob = prob
                best_id = descriptor.class_id
    
        return best_id        
            
    def get_accuracy(self):
        return self.correct/float(self.test_set_len)
        
    def __str__(self):
        output_str = str('Prediction Summary: \n')
        output_str += str('Test set length: %d\n' % self.test_set_len)
        output_str += str('Number of correct classifications: %d\n' % self.correct)
        output_str += str('Classification accuracy: %f\n' % self.get_accuracy())
        return output_str
