// Standard libraries
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <list>
#include <set>
#include <map>
#include <utility>

// Eigen and other frameworks
#include <Eigen/Dense>
#include "tclap/CmdLine.h"

using namespace TCLAP;
using Eigen::MatrixXd;

// Project files
#include "logger.h"
#include "qrs_data.h"
#include "csv_reader.h"
#include "split.h"
#include "qrs_class_manager.h"


std::string data_input_path  = "../ReferencyjneDane/101/ConvertedQRSRawData.txt";
std::string class_input_path = "../ReferencyjneDane/101/Class_IDs.txt";


std::string results_summary(TestResult_t result, size_t size){
    std::ostringstream oss;

    oss << "*** Prediction Test Summary:\n";
    oss << "    Test set length: " << size << std::endl;
    oss << "    Number of correct classifications: " << result.correct_answers << std::endl;
    oss << "    Classification accuracy: " << (result.correct_answers / (double) size) * 100 << "%\n";
    oss << "    Sensitivity: " << (double)result.tp/(result.tp + result.fn)*100 << "%\n";
    oss << "    Specificity: " << (double)result.tn/(result.tn + result.fp)*100 << "%\n";

    return oss.str();
}

int main(int argc, char** argv) {

    // CLI Logic

    CmdLine cmd("Naive Bayes Classification CLI", ' ', "1.0");

    SwitchArg reset_switch("r","reset","Resets Bayes Classificator", false);
    cmd.add(reset_switch);

    SwitchArg verbose_switch("v","verbose","Enable verbose logging", false);
    cmd.add(verbose_switch);

    SwitchArg learn_switch("l","learn","Use data only for learning", false);
    cmd.add(learn_switch);

    SwitchArg test_switch("t","test","Use data only for testing", false);
    cmd.add(test_switch);

    SwitchArg time_switch("m","measure","Measure execution time", false);
    cmd.add(time_switch);

    ValueArg<std::string> features_mask("f","features_mask",
    		"Define which features are considered in classification process (the least significant bit corresponds to the first feature",
			false, "0x3ffff", "double in hex format");
    cmd.add(features_mask);

    MultiArg<std::string> data_source("d","data","Paths to the datasets (Qrs data and Class IDs)", false, "string");
    cmd.add(data_source);

    cmd.parse(argc, argv);

    uint32_t fmask;
    std::istringstream(features_mask.getValue()) >> std::hex >> fmask;
    CSVRow::set_features_mask(fmask);

    std::vector<std::string> data_paths = data_source.getValue();
    if (data_source.isSet()){
        if (data_paths.size() != 2){
            LOG("Incorrect number of paths for: -d (--data)\n");
            return 0;
        }
        data_input_path  = data_paths[0];
        class_input_path = data_paths[1];
    }

    if (learn_switch.isSet() && test_switch.isSet()){
        LOG("Incorrect combination. -t -l cannot be used together");
        return 0;
    }

    //Bayes Logic
    clock_t begin = clock();

    LOG("*** Naive Bayes Classification. Loading data...\n");
    std::list<Qrs> qrs_list = csv_read(data_input_path, class_input_path);
    LOG("*** Data has been successfully loaded.\n");

    QrsClassManager qrs_class_manager(reset_switch.getValue(), fmask);
    if (!test_switch.getValue()){

        std::map<size_t, std::list<Qrs>> training_data;
        if (learn_switch.getValue()){
            training_data = aggregate_classes(qrs_list);
        }
        else{
            LOG("*** Splitting data set into training and test set.\n");
            std::list<Qrs> training_set = split_sets(qrs_list, 0.67);
            training_data = aggregate_classes(training_set);
        }

        LOG("*** Learning has started...\n");
        qrs_class_manager.learn(training_data);
        LOG("*** Learning has finished successfully.\n");

    }

    if(verbose_switch.getValue()){
        LOG("*** Classes Summary:\n");
        LOG(qrs_class_manager.to_string());
    }

    if (!learn_switch.getValue()){
    	TestResult_t result;
        std::list<Qrs> & test_set = qrs_list;
        qrs_class_manager.test(test_set, result);
        LOG(results_summary(result, test_set.size()));
    }

    if (time_switch.getValue()){
        clock_t end = clock();
        double  elapsed_time = double(end - begin) / CLOCKS_PER_SEC;
        LOG("*** Elapsed time: ");
        LOG(elapsed_time);
        LOG("[s]\n");
    }
}
