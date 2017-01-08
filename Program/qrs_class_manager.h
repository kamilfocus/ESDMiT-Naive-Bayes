#ifndef QRS_CLASS_MANAGER_H
#define QRS_CLASS_MANAGER_H

#include <cmath>
#include <fstream>
#include "qrs_class.h"

static const std::string bayes_dump = "bayes.dump";

class QrsClassManager
{

    public:
        QrsClassManager(bool reset, uint32_t fmask)
        {
        	this->fmask = fmask;

            if (reset){
                remove(bayes_dump.c_str());
            }

            std::ifstream qrs_class_file(bayes_dump.c_str());
            if(qrs_class_file.good()){
                from_dump(qrs_class_file);
            }
            qrs_class_file.close();

        }

        ~QrsClassManager(void)
        {
            std::ofstream qrs_class_file(bayes_dump);
            qrs_class_file << (this->to_dump());
            qrs_class_file.close();
        }

        void learn(std::map<size_t, std::list<Qrs>> & training_data){
            std::map<size_t, std::list<Qrs>>::iterator  training_map_it;
            for (training_map_it = training_data.begin(); training_map_it != training_data.end(); ++training_map_it)
            {
                size_t class_id = training_map_it->first;
                class_data.insert( std::make_pair(class_id, QrsClass(class_id)) );

                QrsClass &                  qrs_class = class_data.at(class_id);
                std::list<Qrs> &            qrs_data  = training_map_it->second;
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

        std::string to_dump(){
            std::ostringstream oss;

            oss << std::hex << fmask << std::endl;
            oss << std::dec;

            std::map<size_t, QrsClass>::iterator  class_data_it;
            for (class_data_it = class_data.begin(); class_data_it != class_data.end(); ++class_data_it)
            {
                QrsClass & qrs_class = class_data_it->second;
                oss << qrs_class.to_dump();
            }
            return oss.str();
        }

        void from_dump(std::ifstream & qrs_class_file){
        	if (!qrs_class_file.eof())
        	{
        		uint32_t prev_fmask;
        		qrs_class_file >> std::hex >> prev_fmask;
        		qrs_class_file >> std::dec;
        		if (prev_fmask != fmask)
        			return;
        	}

            while(!qrs_class_file.eof()){
                QrsClass new_qrs_class(0);

                if(!new_qrs_class.from_dump(qrs_class_file))
                    break;

                class_data.insert( std::make_pair(new_qrs_class.get_class_id(), new_qrs_class) );
            }
        }

    private:
        uint32_t fmask;
        std::map<size_t, QrsClass> class_data;
};

#endif // QRS_CLASS_MANAGER_H
