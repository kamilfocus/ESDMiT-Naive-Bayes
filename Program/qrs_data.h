#ifndef QRS_DATA_H
#define QRS_DATA_H

class Qrs
{
    public:
        Qrs(size_t class_id, std::vector<double> & data)
        {
            this->class_id = class_id;
            this->data     = data;
        }

        std::string to_string()
        {

            std::ostringstream oss;
            oss << class_id << ": ";
            for(size_t i = 0; i<data.size(); i++)
            {
                oss << data[i] << " ";
            }
            oss << std::endl;
            return oss.str();
        }

    private:
        size_t                 class_id;
        std::vector<double>    data;
};

#endif // QRS_DATA_H
