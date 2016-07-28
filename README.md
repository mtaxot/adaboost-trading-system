# adaboost-trading-system

An trading system basing on adaboost algorithm using weak classifiers assembled to predict.
Note: Firstly, I should say something about this program and its implementation motivation.


Investment has certain risks, be wised with your decision.
I have never apply this program to real market now.



As we all know stock market is unpredictable, and somewhat stochastic. That's why I don't
"precict" its price ups and downs. This program is not mean to achieve 90%-100% accuracy
prediction. In fact, if you have 70% accuracy or so, you could profit in the long run.
The main idea of this program is to find some "weak predictors", we call it weak classifier in
adaboost algorithm, which performs better than random guess. That is to say, we could find some
"weak predictors" and assemble them together to achieve a "strong predictor". If the "weak predictors"
we have found could lead adaboost algorithm to converge, then the "mode" exist.
I was very optimistic about this program, and now I open source it. The code may be unpleasant, and it
is originally coded in Java, I rewrited it in C/C++ to achieve better running performance.





How to run this program:
I have put 2 stock data(no future data) in input folder named [code]_train.txt and [code]_test.txt, so you could:
1.clone the project using git cmd line or other method, whatever as you will, or download it directly.
2.enter the src folder executing "make" cmd.
3.then there will be an test.exe executable file.
4.run it : "./test.exe -e 2000 -c 600707"   :::::::  -e option is to set the epoch to run----max iterating times or learning times.
  -c is to set the input data to load. The data is located in input folder naming "600707_train.txt" and "600707_test.txt" one for
  training and the other for test.
5.then you will see some outputs strings. If you wana know what it exactly means in detail, "read the fucking source code".






What need to do next:
1.Data!!!  Data!!! , good data is better than algorithms itself. So we need to get good data.
2.Finding better performing no-linear weak classifier to test.
3.If possible, huge data test is urgent.
4.A reasonable and workable hypothesis.


                                                          Thank you! waiting for your contribution.
							  If you have some insight, my email:106601549@qq.com
