#include<vector>
#include<math.h>
#include<string.h>
#include<time.h>
#include"LRWeakClassifier.h"
#ifndef ADABOOST_H
#define ADABOOST_H
using namespace std;

class Adaboost
{
	private:
		double** mInputX;
		double* mInputY;
		double mRows;
		double mColsX;
		double* mWeights;
		int mSampleNum;
		bool mUseModel0;
		vector<LRWeakClassifier*> *mWeakClassifierSet;
	public:
		Adaboost();

		Adaboost(double** input, const int rows, const int colsAll);//colsY = 1, colsX = n (n > 1)

		Adaboost(double** X, double* Y, const int rows, const int colsX);

		~Adaboost();

		void initWeights();

		void setInput(double** X, double* Y, const int rows, const int colsX);

		double predict(double* x);

		double* testTrainedModel(double** testX, double* testY);

		void updateWeights(int* rightOrWrong, double alpha);

		void trainWeakClassifiers(int epoch, double* trainRate);

		double getSumAlpha();
		
};
#endif