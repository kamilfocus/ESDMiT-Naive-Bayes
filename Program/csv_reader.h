#ifndef CSV_READER_H
#define CSV_READER_H

#include <list>
#include <string>
#include "qrs_data.h"


class CSVRow
{
    public:

        double operator[](std::size_t index) const
        {
            return m_data[index];
        }

        std::size_t size() const
        {
            return m_data.size();
        }

        std::vector<double> get_vector()
        {
            return this->m_data;
        }

        void read_next_row(std::istream& str)
        {
            std::string         line;
            std::getline(str, line);
            line.erase(0, line.find_first_not_of(' '));

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();

            uint32_t cnt = 0;
            double qrs_cell;

            while(std::getline(lineStream, cell, ' '))
            {
                std::istringstream(cell) >> qrs_cell;
                if (0 == (features_mask & (1<<cnt++)))
                	continue;

                m_data.push_back(qrs_cell);
            }
        }

        static void set_features_mask(uint32_t fm){
        	features_mask = fm;
        }

    private:
        static uint32_t features_mask;
        std::vector<double>    m_data;
};

uint32_t CSVRow::features_mask = 0;

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.read_next_row(str);
    return str;
}

std::list<Qrs> csv_read(std::string data_input_path, std::string class_input_path)
{
    std::list<Qrs>      qrs_list;
    std::ifstream       qrs_data_file(data_input_path);
    std::ifstream       class_data_file(class_input_path);
    std::string         class_id_line;
    CSVRow              row;

    while(qrs_data_file >> row && class_data_file >> class_id_line)
    {
        size_t class_id;
        std::istringstream(class_id_line) >> class_id;
        std::vector<double> qrs_data = row.get_vector();

        Qrs qrs(class_id, qrs_data);
        qrs_list.push_back(qrs);
    }
    return qrs_list;
}

#endif // CSV_READER_H
