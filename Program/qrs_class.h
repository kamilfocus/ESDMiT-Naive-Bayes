#ifndef QRS_CLASS_H
#define QRS_CLASS_H

#include <cmath>
#include <fstream>

#include <Eigen/Dense>
using Eigen::VectorXd;

#define M_PI    3.14159265358979323846
#define COL_NUM 18

class QrsClass
{
    public:
        QrsClass(size_t class_id)
        {
            this->class_id = class_id;
            this->active   = false;
            this->samples  = 0;
        }

        size_t get_class_id(){
            return class_id;
        }

        size_t get_samples(){
            return samples;
        }


        VectorXd get_std_dev(){
            VectorXd res = VectorXd(var_sum);
            if (samples > 1){
                res /= (samples - 1);
                return res.array().sqrt();
            }
            return res;
        }

        void learn(Qrs & qrs)
        {
            samples++;
            if(active){
                VectorXd delta = VectorXd(qrs.get_data() - mean);
                mean    += (qrs.get_data() - mean)/(samples);
                delta   =  delta.array() * (qrs.get_data() - mean).array();
                var_sum += delta;
            }
            else{
                mean    = VectorXd(qrs.get_data());
                var_sum = VectorXd::Zero(mean.size());
                active  = true;
            }
        }

        double count_probability(Qrs & qrs){
            if(active){
                return gaussian_pdf(qrs).prod();
            }
            return 0.0;
        }

        std::string to_string(){
            std::ostringstream oss;
            oss << class_id << "(" << samples << "): ";
            VectorXd std_dev = get_std_dev();
            for(int i = 0; i < mean.size(); i++)
            {
                oss << "(" << mean[i] << ", " << std_dev[i] << ") ";
            }
            oss << std::endl;
            return oss.str();
        }

        std::string to_dump(){
            std::ostringstream oss;
            oss << class_id << " " << samples << std::endl;
            for(int i = 0; i < mean.size(); i++)
            {
                oss << mean[i] << " ";
            }
            oss << std::endl;
            for(int i = 0; i < var_sum.size(); i++)
            {
                oss << var_sum[i] << " ";
            }
            oss << std::endl;
            return oss.str();
        }

        void from_dump(std::ifstream & qrs_class_file){
            this->active   = true;
            qrs_class_file >> (this->class_id);
            qrs_class_file >> (this->samples);

            double              x;
            std::vector<double> input;
            for(int i = 0; i < COL_NUM; ++i){
                qrs_class_file >> x;
                input.push_back(x);
            }
            mean = VectorXd::Map(input.data(), input.size());
            input.clear();

            for(int i = 0; i < COL_NUM; ++i){
                qrs_class_file >> x;
                input.push_back(x);
            }
            var_sum = VectorXd::Map(input.data(), input.size());
        }

    private:
        bool    active;
        size_t  samples;
        size_t  class_id;

        VectorXd mean;
        VectorXd var_sum;

        VectorXd gaussian_pdf(Qrs & qrs){
            VectorXd std_dev, amp, power;

            std_dev = get_std_dev();

            amp = VectorXd::Ones(mean.size());
            amp /= sqrt(2.0*M_PI);
            amp.array() /= std_dev.array();

            power = qrs.get_data() - mean;
            power = -power.array().pow(2);
            power.array() /= (2*std_dev.array().pow(2));

            return amp.array()*power.array().exp();
        }
};

#endif // QRS_CLASS_H
