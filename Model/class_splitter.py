import logging

from class_descriptor import ClassDescriptor

class ClassSplitter(object):

    def __init__(self, dataset):
        self.class_descriptors = []
        classes_datasets = self.__classes_separate(dataset)
        for key, value in classes_datasets.iteritems():
            self.class_descriptors.append(ClassDescriptor(value))

    def __classes_separate(self, dataset):
        separated_classes = {}
        for i in range(len(dataset)):
            class_item = dataset[i]
            if (class_item.class_id not in separated_classes):
                separated_classes[class_item.class_id] = []
            separated_classes[class_item.class_id].append(class_item)
        return separated_classes
    
    def __str__(self):
        output_str = str('Classes Summary: \n')
        for class_summary in self.class_descriptors:
            output_str += str(class_summary)
            output_str += '\n'
        return output_str
