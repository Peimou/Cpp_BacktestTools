#ifndef FINALPROJECT_BACKTESTER_H
#define FINALPROJECT_BACKTESTER_H

#include <iostream>
#include <queue>
#include "my_unit_test.h"
#include "Order.h"
#include "BookUpdate.h"
#include "MarketSimulator.h"
#include "OrderManager.h"
#include "TradingStrategy.h"
#include "MDReader.h"
#include "BookUpdate.h"
#include "BookBuilder.h"

class BackTester {
public:
    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;
    std::queue<BookUpdate> bookbuilder_to_strategy;
    std::string datapath;
    int datalength;
    void start();
    void end();
    void reset_communication();
    TradingStrategy ts = TradingStrategy(strategy_to_ordermanager,
                                         ordermanager_to_strategy,
                                         ordermanager_to_simulator,
                                         simulator_to_ordermanager,
                                         bookbuilder_to_strategy);

    MarketSimulator ms = MarketSimulator(strategy_to_ordermanager,
                                         ordermanager_to_strategy,
                                         ordermanager_to_simulator,
                                         simulator_to_ordermanager,
                                         bookbuilder_to_strategy);

    OrderManager om = OrderManager(strategy_to_ordermanager,
                                   ordermanager_to_strategy,
                                   ordermanager_to_simulator,
                                   simulator_to_ordermanager,
                                   bookbuilder_to_strategy);

    BookBuilder bb = BookBuilder(strategy_to_ordermanager,
                                 ordermanager_to_strategy,
                                 ordermanager_to_simulator,
                                 simulator_to_ordermanager,
                                 bookbuilder_to_strategy,
                                 datapath, datalength);

    BackTester(string datapath_, int datalength_){
        datapath = datapath_;
        datalength = datalength_;
        bb.datapath = datapath_;
        bb.datalength = datalength_;
    }

};



#endif //FINALPROJECT_BACKTESTER_H
