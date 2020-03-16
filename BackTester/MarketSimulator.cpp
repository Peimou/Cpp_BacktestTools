#include "MarketSimulator.h"
#include <iostream>

#include <string>
#include <fstream>


MarketSimulator::MarketSimulator(
        std::queue<Order> &strategy_to_ordermanager_,
        std::queue<ExecutionOrder> &ordermanager_to_strategy_,
        std::queue<Order> &ordermanager_to_simulator_,
        std::queue<ExecutionOrder> &simulator_to_ordermanager_,
        std::queue<BookUpdate> &bookbuilder_to_strategy_
        ):AppBase(strategy_to_ordermanager_,
                ordermanager_to_strategy_,
                ordermanager_to_simulator_,
                 simulator_to_ordermanager_,
                 bookbuilder_to_strategy_),
                 execution_id(0),
                 pnl(0)
     {
        list_symbols = {"EUR/USD","Gas"};
     };


double MarketSimulator::impact_cost(double quantity, bool isbuy, double thres, double discount) {
    if(quantity >= thres){
        return isbuy?(1 + discount):(1 - discount);
    }
    else{
        return 1;
    }
}

bool MarketSimulator::handle_order(){
    if(!is_working)
        return false;
    if(ordermanager_to_simulator.empty())
        return true;
    const Order o = ordermanager_to_simulator.front();
    ordermanager_to_simulator.pop();

    const bool is_tradeable = list_symbols.find(o.getSymbol()) != list_symbols.end();

    ExecutionOrder new_execution(o);
    if(is_tradeable){
        new_execution.setState(o.getQuantity()>1000?orderstate::ACKNOWLEDGED:orderstate::REJECTED);
        new_execution.setExecutionID(execution_id++);

        //Add the cost of impact
        double dr = impact_cost(o.getQuantity(), o.isBuy());
        double discount = dr * o.getPrice();

        new_execution.setPrice(discount);
        new_execution.setState(orderstate::FILLED);

        double sign = new_execution.isBuy()?-1.:1.;
        double price = new_execution.getPrice();
        double quantity = new_execution.getQuantity();

        pnl += sign * price * quantity;

        std::cout << "simulator push a fill|" <<
                    new_execution.getTimeStamp() <<"|"<<
                    new_execution.getPrice() << "|"  <<
                    new_execution.isBuy()  << "|" <<
                    new_execution.getQuantity() << "|" <<
                    std::endl;
        timestamp.push_back(new_execution.getTimeStamp());
        simulator_to_ordermanager.push(new_execution);
    }
    return true;
}


double MarketSimulator::get_pnl() {
    return pnl;
}

void MarketSimulator::Output_csv() {
    std::ofstream outFile;
    outFile.open("FinalProgram.csv",std::ios::out);
    for(auto p:timestamp){
        outFile<<p<<"\r";
    }
    outFile.close();
    std::cout<<"The output of the timestamp is FinalProgram.csv"<<std::endl;
}