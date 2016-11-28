import logging
import random

class QrsDataSplitter(object):

    def __init__(self, split_ratio):
        self.split_ratio = split_ratio

    def __dict_prepare(self, max_id):
        classes_dict = dict()
        for i in range(1, max_id + 1):
            classes_dict[i] = 0
        return classes_dict

    def __classes_count(self, qrs_dataset, classes_size):
        for qrs_data in qrs_dataset:
            if qrs_data.class_id in classes_size.keys():
                classes_size[qrs_data.class_id] += 1
            else:
                classes_size[qrs_data.class_id] = 1

    def __training_set_too_small(self, train_set_classes, classes_thr):
        for key, value in classes_thr.iteritems():
            if train_set_classes[key] < value:
                return True
        return False

    def split(self, qrs_dataset):
        max_id = max(qrs_data.class_id for qrs_data in qrs_dataset)
        classes_size_thr = self.__dict_prepare(max_id)
        self.__classes_count(qrs_dataset, classes_size_thr)
        for key, value in classes_size_thr.iteritems():
            classes_size_thr[key] = int(value*self.split_ratio)

        train_set_classes = self.__dict_prepare(max_id)
        training_set      = []
        test_set          = list(qrs_dataset)
        while self.__training_set_too_small(train_set_classes, classes_size_thr):
            index = random.randrange(len(test_set))
            train_item = test_set.pop(index)
            train_set_classes[train_item.class_id] += 1
            training_set.append(train_item)

        return training_set, test_set
