import logging
import math

class ClassDescriptor(object):

    def __init__(self, class_id):
        self.class_id = class_id
        self.samples  = 0
        #class_lists   = []
        #for class_instance in class_data:
        #    class_lists.append(class_instance.tolist())
        #self.params = [(self.__mean(attr), self.__stdev(attr)) for attr in zip(*class_lists)]

    def learn(self, class_instance):
        class_values = class_instance.tolist()
        self.samples += 1
        if self.samples == 1:
            self.params = [(attr, 0) for attr in class_values]
        else:
            mean    = [attr_mean[0] for attr_mean in self.params]
            var_sum = [attr_var_sum[1] for attr_var_sum in self.params]
            delta   = list(class_values)

            index = 0
            for class_item in class_values:
                delta[index]   -= mean[index]
                mean[index]    += (class_item - mean[index])/self.samples
                delta[index]   *= (class_item - mean[index])
                var_sum[index] += delta[index]
                index += 1
            self.params = zip(mean, var_sum)
            
    
    def get_stdev(self, var_sum):
        if self.samples > 1:
            var_sum /= (self.samples -1)
        return math.sqrt(var_sum)
    
    
    def __str__(self):
        output_str = str('Class ID = %d summary: ' % self.class_id)
        output_str += str(self.params)
        return output_str
