Description: A enhanced moving average strategy

In CTA, we always use momentum indicator to choose the price and position. I still used moving average in this project because
it is the most common method to decribe the momentum effect of price. I also made some improvement based on Project7. For example,
I further considered an impact cost and a naive model to generate the trading volume. (it was described in the main of the finalproject. You can 
see more details there).

In this strategy, I only sell if I have long position. What's more, if I have long position (short position), I will first execute all the position and then
build the new position with opposite side. For example, if I have 100 long position, and if the signal ask me to sell 500, I will trade 600 in total.

I output all the trading record in the main.cpp. You may compare it with the excel if you need, which is totally the same logic. I also made 
some checks here in googletest, but only respected to the first 100 record. All the trading record is the same as the result listed in excel as 
expected. 

Notes:
You may change the path of boost in the CMakeLists.txt.(boost_1_66_0 required)