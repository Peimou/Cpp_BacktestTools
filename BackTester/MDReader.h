//
// Created by sebastiend on 18/10/2018.
//

#ifndef FINALPROJECT_MARKETDATAREADER_H
#define FINALPROJECT_MARKETDATAREADER_H
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "BookUpdate.h"
using namespace boost::posix_time;

using namespace std;

class MDReader
{
    const std::string fileName;
    const std::string delimeter;
    const unsigned int number_of_line;
    const bool is_header;

public:
    vector<long> timestampvec;
    MDReader(std::string filename,
             std::string delm = ",",
             unsigned int number_of_line_ = 5,
             bool is_header_ = true) :
            fileName(filename),
            delimeter(delm),
            number_of_line(number_of_line_),
            is_header(is_header_)
    { }

    std::vector<BookUpdate> getData()
    {
        unsigned int current_number_of_line=0;
        std::ifstream file(fileName);

        if (!file)
        {
            cerr << "File could not be opened!\n"; // Report error
            cerr << "Error code: " << strerror(errno); // Get some info as to why
            exit(1);
        }

        std::vector<BookUpdate> dataList;

        std::string line = "";
        bool is_header_handled=false;

        while (getline(file, line))
        {
            if(is_header and !is_header_handled)
            {
                is_header_handled=true;
                continue;
            }
            line.erase( std::remove(line.begin(), line.end(), '\r'), line.end() );
            std::vector<std::string> vec;
            boost::algorithm::split(vec, line, boost::is_any_of(delimeter));


            std::string ts(vec[0]);
            time_t tStart;
            int yy, month, dd, hh, mm, ss;
            struct tm whenStart;
            const char *zStart = ts.c_str();

            sscanf(zStart, "%d.%d.%d %d:%d:%d", &dd, &month, &yy, &hh, &mm, &ss);
            whenStart.tm_year = yy - 1900;
            whenStart.tm_mon = month - 1;
            whenStart.tm_mday = dd;
            whenStart.tm_hour = hh;
            whenStart.tm_min = mm;
            whenStart.tm_sec = ss;
            whenStart.tm_isdst = -1;
            unsigned int epoch = mktime(&whenStart);
            // We add real trading volume in this model.
            timestampvec.push_back(epoch);
            BookUpdate b1(0,std::stof(vec[2]),std::stof(vec[4])*100000.,"GAIN",true,vec[1].c_str(),epoch);
            BookUpdate b2(0,std::stof(vec[3]),std::stof(vec[4])*100000.,"GAIN",false,vec[1].c_str(),epoch);
            dataList.push_back(b1);
            dataList.push_back(b2);

            if(number_of_line!=0 and current_number_of_line++>=number_of_line)
            {
                break;
            }
        }
        file.close();

        std::ofstream outFile;
        outFile.open("timestepfunc.csv",std::ios::out);
        for(auto p: timestampvec){
            outFile<<p<<"\r";
        }
        outFile.close();
        std::cout<<"Timestamp for all record was stored in timestampfunc.csv"<<std::endl;

        return dataList;
    }
};


#endif //FINALPROJECT_MARKETDATAREADER_H
