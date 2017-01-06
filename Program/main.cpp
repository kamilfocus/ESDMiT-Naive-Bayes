// Standard libraries
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>

// Eigen and other frameworks
#include <Eigen/Dense>

using Eigen::MatrixXd;

// Project files
#include "logger.h"
#include "qrs_data.h"
#include "csv_reader.h"

std::string data_input_path  = "../ReferencyjneDane/101/ConvertedQRSRawData.txt";
std::string class_input_path = "../ReferencyjneDane/101/Class_IDs.txt";

int main() {
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    LOG(m);
    LOG(std::endl);

    std::list<Qrs>::iterator it1;
    std::list<Qrs> qrs_list = csv_read(data_input_path, class_input_path);
    for (it1=qrs_list.begin(); it1!=qrs_list.end(); ++it1)
        LOG((*it1).to_string());
}
