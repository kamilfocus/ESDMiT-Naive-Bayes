import logging
from pdf import *

class NaiveBayesPredictor(object):

    def __init__(self, test_set, class_descriptors):
        self.correct = 0
        self.tp = 0
        self.tn = 0
        self.fp = 0
        self.fn = 0
        
        self.test_set_len = len(test_set)
        self.descriptors = class_descriptors
        
        for input in test_set:
            prediction = self.__make_prediction(input.tolist())
            if prediction == input.class_id:
                self.correct += 1
                if input.class_id == 1:
                    self.tp += 1
                else:
                    self.tn += 1
            else:
                if input.class_id == 1:
                    self.fn += 1
                else:
                    self.fp += 1

    def __calculate_class_probability(self, input, descriptor):
        probability = 1
        idx = 0
        for mean, var_sum in descriptor.params:
            x = input[idx]
            stdev = descriptor.get_stdev(var_sum)
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
        return 100.0*self.correct/float(self.test_set_len)
        
    def __str__(self):
        output_str = str('Prediction Summary: \n')
        output_str += str('Test set length: %d\n' % self.test_set_len)
        output_str += str('Number of correct classifications: %d\n' % self.correct)
        output_str += str('Classification accuracy: %f%%\n' % self.get_accuracy())
        try:
            output_str += str('Sensitivity: %f%%\n' % (float(self.tp)/(self.tp + self.fn)*100))
        except:
            output_str += str('Sensitivity: 1.#IND%\n')
        try:
            output_str += str('Specificity: %f%%\n' % (float(self.tn)/(self.tn + self.fp)*100))
        except:
            output_str += str('Specificity: 1.#IND%\n')
        return output_str
