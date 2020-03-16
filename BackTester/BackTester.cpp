//
// Created by hp on 2019/12/2.
//

#include "BackTester.h"
#include <iostream>

void BackTester::start() {
    bb.start();
    ms.start();
    om.start();
    ts.start();
    std::cout<<"The length of bookbuilder: "<<bookbuilder_to_strategy.size()<<std::endl;
    std::cout<<"++++++++++++++++++++++++++++++++++++++"<<std::endl;
    std::cout<<"Begin Back Test"<<std::endl;
    std::cout<<"++++++++++++++++++++++++++++++++++++++"<<std::endl;
    while(!bookbuilder_to_strategy.empty())
    {
        ts.process_book_update();
        om.handle_order();
        // if the order is bigger than 500, the buy or sell cost will increase by 1% as the impact cost
        ms.handle_order();
        om.handle_execution_order();
        ts.process_market_response(); // calculate pnl at this stage
    }

}

void BackTester::end(){
    std::cout<<"PnL of the strategy from simulator: "<<ms.get_pnl()<<std::endl;
    std::cout<<"PnL of the strategy from strategy: "<<ts.get_pnl()<<std::endl;
    ms.Output_csv();
    ts.save_signal();
    reset_communication();
    std::cout<<"++++++++++++++++++++++++++++++++++++++"<<std::endl;
    std::cout<<"End of the Back Test"<<std::endl;
    std::cout<<"++++++++++++++++++++++++++++++++++++++"<<std::endl;

}

void BackTester::reset_communication()
{
    while (!strategy_to_ordermanager.empty()) strategy_to_ordermanager.pop();
    while (!ordermanager_to_strategy.empty()) ordermanager_to_strategy.pop();
    while (!ordermanager_to_simulator.empty()) ordermanager_to_simulator.pop();
    while (!simulator_to_ordermanager.empty()) simulator_to_ordermanager.pop();
}