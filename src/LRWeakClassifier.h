#include<stdio.h>
#include"WeakClassifier.h"
#include"LinearRegression.h"
class LRWeakClassifier: public WeakClassifier{
	public:
		LinearRegression *mLr;

		int tryMark(bool* toBeTrain, double* weights, const int rows);

		double predict(double *x, const int colsX);

		double train(double** inputX, double* inputY, double* weights, const int rows, const int colsX);
		
		~LRWeakClassifier();
};