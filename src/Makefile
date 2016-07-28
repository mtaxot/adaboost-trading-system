all:
	@rm -rf test.exe test *.bak
	@g++ test.cpp LinearRegression.cpp Adaboost.cpp LRWeakClassifier.cpp WeakClassifier.cpp DoubleMatrix.cpp -O3 -o test
clean:
	@rm -rf test.exe test *.bak ../input/*.bak ../result/*.bak ../model/*.bak
cleanbak:
	@rm -rf *.bak ../input/*.bak ../result/*.bak ../model/*.bak