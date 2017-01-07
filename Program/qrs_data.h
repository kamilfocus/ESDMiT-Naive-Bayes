#ifndef QRS_DATA_H
#define QRS_DATA_H

#include <Eigen/Dense>
using Eigen::VectorXd;

class Qrs
{
    public:
        Qrs(size_t class_id, std::vector<double> & data)
        {
            this->class_id = class_id;
            this->data     = VectorXd::Map(data.data(), data.size());
        }

        size_t get_class_id(){
            return class_id;
        }

        VectorXd & get_data(){
            return data;
        }

        std::string to_string()
        {

            std::ostringstream oss;
            oss << class_id << ": ";
            for(int i = 0; i<data.size(); i++)
            {
                oss << data[i] << " ";
            }
            oss << std::endl;
            return oss.str();
        }

    private:
        size_t     class_id;
        VectorXd   data;
};

#endif // QRS_DATA_H
