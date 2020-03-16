#include "OrderManager.h"


bool OrderManager::handle_execution_order(){
    if(!is_working)
        return false;
    if (simulator_to_ordermanager.empty())
        return true;
    else{
        ExecutionOrder exo = simulator_to_ordermanager.front();
        simulator_to_ordermanager.pop();
        ordermanager_to_strategy.push(exo);
    }
    return true;
};


unsigned int OrderManager::get_number_of_open_orders()
{
    return 0;
}


unsigned int OrderManager::get_number_of_non_acknowledged_orders()
{
    return 0;
}

int OrderManager::get_position(std::string symbol)
{
    return 0;
}

bool OrderManager::handle_order(){
    if(!is_working)
        return false;
    if (strategy_to_ordermanager.empty())
        return true;
    const Order &e = strategy_to_ordermanager.front();
    strategy_to_ordermanager.pop();
    order_id++;
    order=e;
    order.setOrderID(order_id);
    ordermanager_to_simulator.push(order);
    return true;
};
