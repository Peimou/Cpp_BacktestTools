
#ifndef FINALPROJECT_MARKETSIMULATOR_H
#define FINALPROJECT_MARKETSIMULATOR_H
#include "AppBase.h"
#include "Order.h"
#include <set>
#include <queue>
#include <string>
#include <iostream>

class MarketSimulator : public AppBase {
public:
    unsigned execution_id;
    bool isfirst = true;
    std::set<std::string> list_symbols;
    std::vector<long> timestamp;
    double pnl;
    MarketSimulator(
             std::queue<Order> &strategy_to_ordermanager_,
             std::queue<ExecutionOrder> &ordermanager_to_strategy_,
             std::queue<Order> &ordermanager_to_simulator_,
             std::queue<ExecutionOrder> &simulator_to_ordermanager_,
             std::queue<BookUpdate> &bookbuilder_to_strategy_
    );
    bool handle_order();

    virtual void start() {is_working=true;}
    virtual void stop() {is_working=false;}
    double get_pnl();
    void Output_csv();

    //in the part, we assume if the order quantity is bigger than 500, then the buy will increase 1.1,
    //and the sell price will be 0.9

    double impact_cost(double quantity, bool isbuy, double thres = 500., double discount = 0.1);

};


#endif //FINALPROJECT_MARKETSIMULATOR_H
