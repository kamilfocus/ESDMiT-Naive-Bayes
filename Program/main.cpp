// Standard libraries
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>
#include <set>
#include <map>
#include <utility>

// Eigen and other frameworks
#include <Eigen/Dense>

using Eigen::MatrixXd;

// Project files
#include "logger.h"
#include "qrs_data.h"
#include "csv_reader.h"
#include "split.h"
#include "qrs_class_manager.h"

std::string data_input_path  = "../ReferencyjneDane/101/ConvertedQRSRawData.txt";
std::string class_input_path = "../ReferencyjneDane/101/Class_IDs.txt";

std::string results_summary(size_t correct, size_t size){
    std::ostringstream oss;

    oss << "*** Prediction Test Summary:\n";
    oss << "    Test set length: " << size << std::endl;
    oss << "    Number of correct classifications: " << correct << std::endl;
    oss << "    Classification accuracy: " << (correct / (double) size) << "%\n";

    return oss.str();
}

int main() {
    LOG("*** Naive Bayes Classification. Loading data...\n");

    std::list<Qrs> qrs_list = csv_read(data_input_path, class_input_path);
    LOG("*** Data has been successfully loaded.\n");

    LOG("*** Splitting data set into training and test set.\n");
    std::list<Qrs> training_set = split_sets(qrs_list, 0.67);
    std::map<size_t, std::list<Qrs>> training_data = aggregate_classes(training_set);
    //log_all_classes(training_data);

    LOG("*** Learning has started...\n");
    QrsClassManager qrs_class_manager;
    qrs_class_manager.learn(training_data);

    LOG("*** Classes Summary:\n");
    LOG(qrs_class_manager.to_string());

    std::list<Qrs> & test_set = qrs_list;
    size_t correct_answers    = qrs_class_manager.test(test_set);
    LOG(results_summary(correct_answers, test_set.size()));
}
