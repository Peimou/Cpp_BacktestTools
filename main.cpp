#include <iostream>
#include "BackTester.h"


int main() {
    /*description of the strategy:
     * 1. Data: I used gas commodity form 2019-11-18 to 2019-11-21 (tick level)
     * 2. Generation of signal:
     *    2.1 buy and sell: if ma20min is bigger than ma40min, we buy the position. Otherwise, we sell the position.
     *    2.2 quantity: I improved this moving average with a non constant quantity. if ma20 - ma40 is larger than .015
     *    or smaller than -0.015, we buy or sell 600; otherwise we only buy or sell 100. It means if the spread of
     *    ma20 and ma40 is very large, we will be more confident with this signal. Therefore, we will buy(sell) more.
     * 3. Market impact: I improved the market simulator by adding impact cost. I assume, if the quantity is more than
     *    500, than the buy or sell cost will increase 10%.
     * 4. Trading quantity: I assume that we must buy at first. if we change the direction of the position, we will clear
     *    all the position before we build new position. For example, I have 500 long position right now, and the signal is
     *    to short and the target position is 200. At this situation, we will sell 500 + 200 = 700.
     * 5. Result: -2.08047e+06 if we do not have the restrict of margin, which is exactly the same with the result in Excel.
     */

    string path = "../InputData/Gasdata.csv"; //change the data filepath here
    BackTester BT(path, 0);
    BT.start();
    BT.end();
}
