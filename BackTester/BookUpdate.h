#ifndef FINALPROJECT_BOOKUPDATE_H
#define FINALPROJECT_BOOKUPDATE_H

#include <string.h>

class BookUpdate {
private:
    int level;
    double price;
    double quantity;
    char venue[20];
    char symbol[20];
    bool is_buy;
    long epoch_time;
public:
    BookUpdate(int level_,
            double price_,
            double quantity_,
            const char *venue_,
            bool is_buy_,
            const char *symbol_,
            unsigned int epoch_time_):
            level(level_),
            price(price_),
            quantity(quantity_),
            is_buy(is_buy_),
            epoch_time(epoch_time_)
    {
        strcpy(venue,venue_);
        strcpy(symbol,symbol_);
    }
    BookUpdate(const BookUpdate& other){
        level = other.level;
        price = other.price;
        quantity = other.quantity;
        epoch_time = other.epoch_time;
        is_buy = other.is_buy;
        strcpy(venue,other.venue);
        strcpy(symbol,other.symbol);

    };
    double get_price() const {return price;}
    double get_quantity() const {return quantity;}
    int get_level() const {return level;}
    const char * get_venue() const {return venue;}
    bool get_is_buy() const {return is_buy;}
    const char * get_symbol() const {return symbol;}
    long get_epoch_time() const {return epoch_time;}
};


#endif //FINALPROJECT_BOOKUPDATE_H
