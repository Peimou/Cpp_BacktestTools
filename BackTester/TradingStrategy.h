//
// Created by sebastiend on 06/10/2018.
//

#ifndef FINALPROJECT_TRADINGSTRATEGY_H
#define FINALPROJECT_TRADINGSTRATEGY_H
#include "AppBase.h"
#include "BookUpdate.h"
#include "Order.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

class Signal{
public:
    BookUpdate *lastbook = nullptr;
    std::vector<std::pair<std::pair<long, double>, double>> meanprice;
    double ma5 = 0., ma20=0.;
    std::vector<double> ma5vec, ma20vec;
    void insert_book_update(BookUpdate bu);
    double get_quantity(); // if ma20 - ma40>= 0.015(<=0.015), we buy(sell) 600, otherwise, we buy(sell) 100
    double get_20min_moving_average();
    double get_40min_moving_average();
    bool go_long();
    bool go_short();
    bool is_tradeable(BookUpdate &bu){return true;}
    ~Signal(){
        delete lastbook;
        lastbook = nullptr;
    }
};

class Execution{
private:
    Order e;
    bool tradeable;
public:

    Execution():tradeable(false){}
    bool insert_order(long timestamp_,
                      bool is_buy_,
                      double price_,
                      unsigned int quantity_,
                      const char * venue_,
                      const char * symbol_,
                      ordertype type_,
                      unsigned int id_);
    bool is_tradeable() {return tradeable;}
    void set_tradeable(bool is_tradable){tradeable=is_tradable;};
    Order & get_order() {return e;}
};


class TradingStrategy : public AppBase {
private:
    Execution execution;
    int order_id;
    bool cur_position = false;
    bool isfirst = true;
    double quantity = 100;
    std::unordered_map<std::string,int> positions;
    unsigned int number_of_rejections;
    unsigned int number_of_fills;
    double pnl;

public:
    Signal signal;
    double curpostion = 0;
    int trading_number = 0;
    TradingStrategy(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_,
            std::queue<BookUpdate> &bookbuilder_to_strategy_
    ):
    AppBase(strategy_to_ordermanager_,
              ordermanager_to_strategy_,
              ordermanager_to_simulator_,
              simulator_to_ordermanager_,
              bookbuilder_to_strategy_),
              signal(),
              execution(),
              order_id(1),
              number_of_rejections(0),
              number_of_fills(0),
              pnl(0){}
    virtual void start() {is_working=true;}
    virtual void stop() {
        positions.clear();
        is_working=false;
    }


    bool process_book_update(BookUpdate &bu);
    bool process_book_update();
    bool process_execution();
    bool process_market_response();
    int get_position();
    unsigned int get_number_of_rejections();
    unsigned int get_number_of_fills();
    void reset_position();
    double get_pnl();
    void save_signal();

};


#endif //FINALPROJECT_TRADINGSTRATEGY_H
