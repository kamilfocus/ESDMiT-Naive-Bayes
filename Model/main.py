import csv
import logging

from qrs_data import QrsData

input_path = '../ReferencyjneDane/100/ConvertedQRSRawData.txt'
log_file   = 'bayes_logger.txt'

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

def raw_csv_data_load(input_path, fieldnames=None):
    data   = []
    kwargs = {'delimiter' : ' ', 'quotechar' :'\n', 'skipinitialspace' : True}
    with open(input_path, 'r') as csvfile:
        if fieldnames:
            csv_reader = csv.DictReader(csvfile, fieldnames, **kwargs)
        else:
            csv_reader = csv.reader(csvfile, **kwargs)
        for row in csv_reader:
            qrs_row = QrsData(row)
            data.append(qrs_row)
    return data

def named_csv_data_load(input_path):
    fieldnames = ['r_peak', 'r_peak_value', 'rr_pre_interval', 'rr_post_interval', \
                  'p_onset', 'p_onset_val', 'p_peak', 'p_peak_val', 'p_end', \
                  'p_end_val', 'qrs_onset', 'qrs_onset_val', 'qrs_end', 'qrs_end_val', \
                  't_peak', 't_peak_val', 't_end', 't_end_val']
    QrsData.fields_set(fieldnames)
    return raw_csv_data_load(input_path, fieldnames)

if __name__ == "__main__":
    logger_setup(log_level = logging.INFO, log_to_file = True)
    logging.info('Naive Bayes Classification')

    data = named_csv_data_load(input_path)

    sample_data = data[0]
    logging.info(sample_data)
