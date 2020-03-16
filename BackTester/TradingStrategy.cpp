#define mk std::make_pair
#include "TradingStrategy.h"
#include <fstream>
#include <iomanip>

void Signal::insert_book_update(BookUpdate bu) {

    if(!bu.get_is_buy()){
        meanprice.push_back(mk(mk(bu.get_epoch_time(),
                bu.get_price()/2. + lastbook->get_price()/2.),lastbook->get_price()));
        get_20min_moving_average();
        get_40min_moving_average();
    }
    if(lastbook){
        delete lastbook;
    }
    lastbook = new BookUpdate(bu);
}

double Signal::get_quantity() {
    if(ma5 == 0 || ma20 == 0){
        return 0;
    }
    if((ma5 -ma20) >= 0.015 || (double)(ma5-ma20) <= -0.015){
        return 600;
    }else{
        return 100;
    }
}

double Signal::get_20min_moving_average() {
    int endindex = meanprice.size() - 1, num = 0;
    double sum = 0;
    for(int i = endindex; i>=0; i--){
        if(meanprice[endindex].first.first - meanprice[i].first.first <= 1200){
            sum += meanprice[i].first.second;
            num++;
        }
        else
            break;
    }
    ma5 = sum/num;
    ma5vec.push_back(ma5);
    return ma5;

}

double Signal::get_40min_moving_average() {
    int endindex = meanprice.size() - 1, num = 0;
    double sum = 0;
    for(int i = endindex; i>=0; i--){
        if(meanprice[endindex].first.first - meanprice[i].first.first <= 2400){
            sum += meanprice[i].first.second;
            num++;
        }
        else
            break;
    }
    ma20 = sum/num;
    ma20vec.push_back(ma20);
    return ma20;
}

bool Signal::go_long() {
    return ma5>ma20;
}

bool Signal::go_short() {
    return ma5<ma20;
}

bool TradingStrategy::process_book_update(BookUpdate &bu){
    if(!is_working)
        return false;
    if(bookbuilder_to_strategy.empty() && curpostion!=0){
        std::cout<<"============================================="<<std::endl;
        std::cout<<"Execute the last order to be position neutral"<<std::endl;
        quantity = signal.get_quantity() + curpostion;
        if(signal.go_long()){
            execution.insert_order(bu.get_epoch_time(), false,
                    bu.get_price(),quantity,bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
        }else{
            int idx = signal.meanprice.size() - 1;
            execution.insert_order(bu.get_epoch_time(), true,
                                   signal.meanprice[idx].first.second*2 - signal.meanprice[idx].second,
                                   quantity, bu.get_venue(),
                                   bu.get_symbol(),ordertype::LIMIT,order_id++);
        }
        execution.set_tradeable(true);
        curpostion = signal.get_quantity();
        return process_execution();
    }
    if(bu.get_is_buy())
        return false;
    if((!signal.go_long())^cur_position)
        return false;
    if (signal.is_tradeable(bu)) {
        quantity = signal.get_quantity() + curpostion; //sell the postion we have
        int ind = signal.meanprice.size() - 1;
        double price = signal.go_long()?signal.meanprice[ind].second:bu.get_price();
        execution.insert_order(bu.get_epoch_time(), signal.go_long(),
                price, quantity, bu.get_venue(),
                bu.get_symbol(),ordertype::LIMIT,order_id++);
        execution.set_tradeable(true);
        cur_position = signal.go_long();
        curpostion = signal.get_quantity(); //the target postion is the quantity genreated by the signal;
    }

    return process_execution();
}

bool Execution::insert_order(long timestamp_,
                             bool is_buy_,
                             double price_,
                             unsigned int quantity_,
                             const char * venue_,
                             const char * symbol_,
                             ordertype type_,
                             unsigned int id_)
{
    e.setTimestamp(timestamp_);
    e.setSide(is_buy_);
    e.setPrice(price_);
    e.setQuantity(quantity_);
    e.setVenue(venue_);
    e.setType(type_);
    e.setSymbol(symbol_);
    e.setOrderID(id_);
    return true;
}

bool TradingStrategy::process_book_update(){
    if(!is_working)
        return false;
    if(bookbuilder_to_strategy.empty())
        return true;
    BookUpdate bu = bookbuilder_to_strategy.front();
    bookbuilder_to_strategy.pop();
    signal.insert_book_update(bu);
    return process_book_update(bu);
}

bool TradingStrategy::process_execution() {
    if(!is_working)
        return false;

    Order order;
    trading_number++;

    if (execution.is_tradeable()) {
        order.setTimestamp(execution.get_order().getTimeStamp());
        order.setType(execution.get_order().getOrderType());
        order.setVenue(execution.get_order().getVenue());
        order.setQuantity(execution.get_order().getQuantity());
        order.setPrice(execution.get_order().getPrice());
        order.setOrderID(execution.get_order().getID());
        order.setSymbol(execution.get_order().getSymbol());
        order.setSide(execution.get_order().isBuy());
        execution.set_tradeable(false);
        strategy_to_ordermanager.push(order);
        if(bookbuilder_to_strategy.empty()){
            std::cout<<"Strategy push the last order to ordermanager"<<std::endl;
            std::cout<<"============================================"<<std::endl;
        }

    }
    return true;
};

void TradingStrategy::save_signal(){
    std::ofstream outFile;
    outFile.open("ma5.csv",std::ios::out);
    for(auto p: signal.ma5vec){
        outFile<<p<<"\r";
    }
    outFile.close();
    outFile.open("ma20.csv",std::ios::out);
    for(auto p: signal.ma20vec){
        outFile<<p<<"\r";
    }
    outFile.close();
    std::cout<<"Trading Signal Reserved"<<std::endl;
}

bool TradingStrategy::process_market_response()
{
    if(ordermanager_to_strategy.empty()){
        return true;
    }

    ExecutionOrder exo = ordermanager_to_strategy.front();
    ordermanager_to_strategy.pop();
    double sign = exo.isBuy()?-1.:1.;
    if(isfirst || bookbuilder_to_strategy.empty()){
        isfirst = false;
    }
    pnl += (double) sign * (double)exo.getPrice() * (double) exo.getQuantity();
    return true;
}

int TradingStrategy::get_position()
{
    return curpostion;
}

unsigned int TradingStrategy::get_number_of_rejections() {
    return 0;
}

unsigned int TradingStrategy::get_number_of_fills() {
    return 0;
}

double TradingStrategy::get_pnl() {
    return pnl;
}

void TradingStrategy::reset_position(){
    positions.clear();
}