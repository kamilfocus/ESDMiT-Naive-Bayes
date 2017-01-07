#ifndef QRS_CLASS_MANAGER_H
#define QRS_CLASS_MANAGER_H

#include <cmath>
#include <Eigen/Dense>
using Eigen::VectorXd;

#define M_PI        3.14159265358979323846

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
            oss << class_id << ": ";
            VectorXd std_dev = get_std_dev();
            for(int i = 0; i < mean.size(); i++)
            {
                oss << "(" << mean[i] << ", " << std_dev[i] << ") ";
            }
            oss << std::endl;
            return oss.str();
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

            amp          = VectorXd::Constant(1.0/(sqrt(2.0*M_PI)), mean.size());
            amp.array() /= std_dev.array();

            power = qrs.get_data() - mean;
            power = -power.array().pow(2);
            power.array() /= (2*std_dev.array().pow(2));

            return amp.array()*power.array().exp();
        }
};

class QrsClassManager
{
    public:
        QrsClassManager(void)
        {
            class_data.clear();
        }

        void learn(std::map<size_t, std::list<Qrs>> & training_data){
            std::map<size_t, std::list<Qrs>>::iterator  training_map_it;
            for (training_map_it = training_data.begin(); training_map_it != training_data.end(); ++training_map_it)
            {
                size_t class_id = training_map_it->first;
                class_data.insert( std::make_pair(class_id, QrsClass(class_id)) );

                QrsClass & qrs_class = class_data.at(class_id);
                std::list<Qrs> &            qrs_data = training_map_it->second;
                std::list<Qrs>::iterator    qrs_data_it;
                for (qrs_data_it = qrs_data.begin(); qrs_data_it != qrs_data.end(); ++qrs_data_it)
                    qrs_class.learn(*qrs_data_it);
            }
        }

        size_t test(std::list<Qrs> & test_data){
            size_t correct_answers = 0;

            std::list<Qrs>::iterator test_data_it;
            for (test_data_it = test_data.begin(); test_data_it != test_data.end(); ++test_data_it){
                double     best_probability;
                QrsClass * best_qrs_class;

                std::map<size_t, QrsClass>::iterator  class_data_it;
                for (class_data_it = class_data.begin(); class_data_it != class_data.end(); ++class_data_it)
                {
                    QrsClass & qrs_class = class_data_it->second;
                    double probability   = qrs_class.count_probability(*test_data_it);
                    if(probability > best_probability || class_data_it == class_data.begin()){
                        best_qrs_class   = &qrs_class;
                        best_probability = probability;
                    }
                }

                if( (*test_data_it).get_class_id() == (*best_qrs_class).get_class_id() )
                    correct_answers++;
            }
            return correct_answers;
        }

        std::string to_string(){
            std::ostringstream oss;

            std::map<size_t, QrsClass>::iterator  class_data_it;
            for (class_data_it = class_data.begin(); class_data_it != class_data.end(); ++class_data_it)
            {
                QrsClass & qrs_class = class_data_it->second;
                oss << "    " << qrs_class.to_string();
            }
            return oss.str();
        }

    private:
        std::map<size_t, QrsClass> class_data;
};

#endif // QRS_CLASS_MANAGER_H
