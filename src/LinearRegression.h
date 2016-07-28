#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<vector>

#ifndef LINEARGRESSION_H
#define LINEARGRESSION_H

class LinearRegression{
    private:
        double* mWeights;
        double mBias;
        double lr;
        double EPOCH;

    public:
		LinearRegression();
		~LinearRegression();

        double getBias();

		double train(double** trainX, double* trainY, const int rows, const int colsX);

		double predict(double* trainX, const int colsX);
		/** X dim , exclude Y**/
		void init(const int colsX);
};
#endif
