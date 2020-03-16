#include <BackTester.h>
#include <gtest/gtest.h>
#include <TradingStrategy.h>
#include <string>

//The Trading Logic has been checked in the Excel, so we focus in checking the signal.

TEST(Unit_Test, ChangeDataPath){
    string path = "C:\\Users\\hp\\Desktop\\FinalProject\\InputData\\Gasdata.csv";
    int length = 0;
    BackTester BT(path,0);
    EXPECT_TRUE(BT.datapath == path);
    EXPECT_TRUE(BT.bb.datapath == path);
}

TEST(Unit_Test, ChangeDataLength){
    string path = "C:\\Users\\hp\\Desktop\\FinalProject\\InputData\\Gasdata.csv";
    int length = 5;
    BackTester BT(path,length);
    EXPECT_TRUE(BT.datalength == length);
    EXPECT_TRUE(BT.bb.datalength == length);
    BT.bb.start();
    EXPECT_TRUE(BT.bookbuilder_to_strategy.size() == 12);
}

TEST(Unit_Test, TradingStrategy_Signal_1){
    Signal sig;
    for(int i = 0; i < 20; i++){
        bool isbuy = i%2==0?true:false;
        sig.insert_book_update(BookUpdate(0,i,10,"GAIN",
                isbuy,"Gas",i + 10));
    }
    EXPECT_TRUE(sig.ma5 == 9.5);
    EXPECT_TRUE(sig.ma20 == 9.5);
}

TEST(Unit_Test, TradingStrategy_Signal_2){
    //in this test, I will the real data to check the ma signal
    string path = "C:\\Users\\hp\\Desktop\\FinalProject\\InputData\\Gasdata.csv";
    BackTester BT(path, 10);
    BT.start();
    BT.end();
    //then we should have zero signal (no trading);
    EXPECT_TRUE(BT.ts.signal.ma5 - 588.436 <= 0.0001); //That's due to the error of double in cpp
    EXPECT_TRUE(BT.ts.signal.ma20 - 588.436 <= 0.0001);
    EXPECT_TRUE(BT.ts.trading_number == 0); //there should be no trading signal, since we didnot
    //in this period, we do not execute the last order. ma20vec reserved the
}


TEST(Unit_Test, TradingStrategy_Signal_3){
    //in this test, I will the real data to check the ma signal
    string path = "C:\\Users\\hp\\Desktop\\FinalProject\\InputData\\Gasdata.csv";
    BackTester BT(path, 100); // I calculated the first 100 record in execl to check the signal
    BT.start();
    BT.end();
    //then we should have zero signal (no trading);
    EXPECT_TRUE(BT.ts.signal.ma5 - 588.223 <= 0.0001); //That's due to the error of double in cpp
    EXPECT_TRUE(BT.ts.signal.ma20 - 588.242 <= 0.0001);
    EXPECT_TRUE(BT.ts.trading_number == 0); //there should be no trading signal
    EXPECT_TRUE(BT.ts.curpostion == 0); //there should be no postion
}


TEST(Unit_Test, TradingStrategy_Volume){
    //in this test, I will the check function for the quantity calculation
    string path = "C:\\Users\\hp\\Desktop\\FinalProject\\InputData\\Gasdata.csv";
    BackTester BT(path, 100);
    BT.ts.signal.ma5 = 10;
    BT.ts.signal.ma20 = 20;//biger than 0.015
    double quantity = BT.ts.signal.get_quantity();//
    EXPECT_TRUE(quantity - 600 <= 0.0001); //That's due to the error of double in cpp
    BT.ts.signal.ma5 = 10;
    BT.ts.signal.ma20 = 10.0001;
    quantity = BT.ts.signal.get_quantity();//smaller than 0.015
    EXPECT_TRUE(quantity - 100 <= 0.0001); //That's due to the error of double in cpp
}


TEST(Unit_Test, MarketSimulator_Impact_cost){
    string path = "C:\\Users\\hp\\Desktop\\FinalProject\\InputData\\Gasdata.csv";
    BackTester BT(path, 100);
    double dr = BT.ms.impact_cost(600,true,500,0.1);//buy cost will increase 0.1, it becomes 1.1
    double dr1 = BT.ms.impact_cost(600,true,601,0.1);//lower than threshold, it becomes 1
    double dr2 = BT.ms.impact_cost(600,false,200,0.1);//lower than threshold, it becomes 0.9
    EXPECT_TRUE(dr - 1.1 <0.00001);
    EXPECT_TRUE(dr1 - 1 <0.00001);
    EXPECT_TRUE(dr2 - 0.9 <0.00001);
}

TEST(Unit_Test, BackTester_get_postion){
    //in the code of backtest, I did not add the code to print the current postion because we can read the
    //record in the cout of fill order. in this test, I will show you this function really work.
    string path = "C:\\Users\\hp\\Desktop\\FinalProject\\InputData\\Gasdata.csv";
    BackTester BT(path, 100); //in the first 100 hundred record, there is no trading , the postion should
    //be zero
    BT.start();
    BT.end();
    double pos = BT.ts.get_position();
    EXPECT_TRUE(pos == 0);
}
