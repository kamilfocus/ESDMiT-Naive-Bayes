import logging
import math

class ClassDescriptor(object):

    def __init__(self, class_data):
        self.class_id = class_data[0].class_id
        class_lists   = []
        for class_instance in class_data:
            class_lists.append(class_instance.tolist())
        self.params = [(self.__mean(attr), self.__stdev(attr)) for attr in zip(*class_lists)]

    def __mean(self, numbers):
        return sum(numbers)/float(len(numbers))
    
    def __stdev(self, numbers):
        avg = self.__mean(numbers)
        variance = sum([pow(x-avg,2) for x in numbers])/float(len(numbers)-1)
        return math.sqrt(variance)
        
    def __str__(self):
        output_str = str('Class ID = %d summary: ' % self.class_id)
        output_str += str(self.params)
        return output_str
