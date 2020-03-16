project(BackTester)

add_definitions(-std=gnu++11)

set(HEADER_FILES Backtester.h AppBase.h BookBuilder.h BookUpdate.h
        MarketSimulator.h MDReader.h Order.h OrderManager.h TradingStrategy.h)
set(SOURCE_FILES Backtester.cpp AppBase.cpp BookBuilder.cpp BookUpdate.cpp
        MarketSimulator.cpp MDReader.cpp Order.cpp OrderManager.cpp TradingStrategy.cpp)

add_library(BackTester STATIC ${SOURCE_FILES} ${HEADER_FILE} )