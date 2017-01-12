import csv
import logging
import time
import pickle
import os.path

from qrs_data import QrsData
from qrs_data_splitter import QrsDataSplitter
from class_splitter import ClassSplitter
from class_descriptor import ClassDescriptor
from naive_bayes import NaiveBayesPredictor

data_input_path  = '../ReferencyjneDane/data_unique_labels/100/test_data.txt'
class_input_path = '../ReferencyjneDane/data_unique_labels/100/test_label.txt'
log_file         = 'bayes_logger.txt'
dump_file        = 'bayes.dump'

fields2skip = []

def logger_setup(log_level = logging.INFO, log_to_file = False):
    FORMAT = "%(filename)s: %(message)s"
    logging.basicConfig(format=FORMAT, level=log_level)

    if log_to_file:
        #Clean file
        open(log_file, 'w').close()

        #Get root logger and configure an additional file sink for logs
        logger = logging.getLogger()
        fh = logging.FileHandler(log_file)
        fh.setLevel(log_level)
        fh.setFormatter(logging.Formatter(FORMAT))
        logger.addHandler(fh)

def raw_csv_data_load(data_input_path, class_input_path, fieldnames=None):
    res   = []
    kwargs = {'delimiter' : ' ', 'quotechar' :'\n', 'skipinitialspace' : True}
    with open(data_input_path, 'r') as csvfile:
        csv_reader = csv.reader(csvfile, **kwargs)
        if fieldnames:
            csv_reader = csv.DictReader(csvfile, fieldnames, **kwargs)
        with open(class_input_path, 'r') as class_file:
            class_reader = csv.reader(class_file, quotechar='\n')
            for data, class_id in zip(csv_reader, class_reader):
                if len(class_id) != 1:
                    msg = 'Class ID should contain one element: len(class_id) != %d'
                    raise StandardError(msg % len(class_id))
                
                if fields2skip:
                    for field_name in fields2skip:
                        del data[field_name]
                
                qrs_record = QrsData(int(class_id[0]), data)
                res.append(qrs_record)
    return res

def named_csv_data_load(data_input_path, class_input_path):
    fieldnames = ['r_peak', 'r_peak_value', 'rr_pre_interval', 'rr_post_interval', \
                  'p_onset', 'p_onset_val', 'p_peak', 'p_peak_val', 'p_end', \
                  'p_end_val', 'qrs_onset', 'qrs_onset_val', 'qrs_end', 'qrs_end_val', \
                  't_peak', 't_peak_val', 't_end', 't_end_val']
    QrsData.fields_set([item for item in fieldnames if item not in fields2skip])
    return raw_csv_data_load(data_input_path, class_input_path, fieldnames)

def qrs_data_log(qrs_data, class_id):
    records_num = 0
    for qrs_record in qrs_data:
        if qrs_record.class_id == class_id:
            logging.debug(qrs_record)
            records_num += 1
    logging.debug('Total number of %d-class elements: %d' % (class_id, records_num))

def qrs_data_all_classes_log(qrs_data, max_class_id):
    for i in range(1, max_class_id + 1):
        qrs_data_log(qrs_data, i)

if __name__ == "__main__":

    reset = False
    test = True
    learn = not test

    logger_setup(log_level = logging.INFO, log_to_file = True)
    logging.info('Naive Bayes Classification')

    qrs_dataset = named_csv_data_load(data_input_path, class_input_path)
    
    start_time = time.time()
    
    max_class_id = max(qrs_data.class_id for qrs_data in qrs_dataset)
    
    training_set = None
    test_set = None
    if not test and not learn:
        training_set, test_set = QrsDataSplitter(0.67).split(qrs_dataset)

        logging.debug('Training Set\n')
        qrs_data_all_classes_log(training_set, max_class_id)
        logging.debug('\nTest Set\n')
        qrs_data_all_classes_log(test_set, max_class_id)
    elif learn:
        training_set = list(qrs_dataset)
    elif test:
        test_set = list(qrs_dataset)

    class_descriptors = []
    if os.path.exists(dump_file):
        with open(dump_file, 'rb') as f:
            class_descriptors = pickle.load(f)

    class_id_list = []
    for cd in class_descriptors:
        class_id_list.append(cd.class_id)
        
    if training_set:
        class_datasets = ClassSplitter(training_set).class_datasets
    
        for key, value in class_datasets.iteritems():
            try:
                qrs_class = class_descriptors.pop(class_ids_list.index(key))
            except Exception:
                qrs_class = ClassDescriptor(key)

            for class_instance in value:
                qrs_class.learn(class_instance)
            class_descriptors.append(qrs_class)
    

    if test_set:
        naive_bayes = NaiveBayesPredictor(test_set, class_descriptors)
        logging.info(naive_bayes)
    
    logging.info('Execution time: %f [s]' % (time.time() - start_time))
    
    with open(dump_file, 'wb') as f:
        pickle.dump(class_descriptors, f)

