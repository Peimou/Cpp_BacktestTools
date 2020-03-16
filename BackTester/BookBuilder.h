
#ifndef FINALPROJECT_BOOKBUILDER_H
#define FINALPROJECT_BOOKBUILDER_H
#include "AppBase.h"
#include "MDReader.h"

class BookBuilder: public AppBase {
public:
    std::string datapath;
    int datalength;
    BookBuilder(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_,
            std::queue<BookUpdate> &bookbuilder_to_strategy_,
            std::string datapath_, int datalength
    );
    virtual void start(){
        MDReader reader(datapath,",",datalength);
        std::vector<BookUpdate> dataList = reader.getData();
        for(const BookUpdate &bu : dataList)
        {
            bookbuilder_to_strategy.push(bu);
        }
    };
    virtual void stop(){

    };

};


#endif //FINALPROJECT_BOOKBUILDER_H
