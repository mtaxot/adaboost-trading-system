#include<stdio.h>
#include"LRWeakClassifier.h"
LRWeakClassifier::~LRWeakClassifier(){
	if(mLr != NULL){
		delete mLr;
		mLr = NULL;
	}
}
int LRWeakClassifier::tryMark(bool* toBeTrain, double* weights, const int rows){
	if(toBeTrain == NULL || weights == NULL){
		printf("LRWeakClassifier::tryMark(bool*, double*, int, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	int selected = 0;
	for(int i = 0; i < rows; i++){
		if(weights[i] - 1.0 / rows > 0.00001){
			toBeTrain[i] = 1;
			selected++;
		}else{
			toBeTrain[i] = 0;
		}
	}
	return selected;
}


double LRWeakClassifier::predict(double *x, const int colsX){
	if(x == NULL){
		printf("LRWeakClassifier::predict(double*, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	//if(mLr == NULL) printf("------------\n");
	double res = (*mLr).predict(x, colsX);
	
	return res;
}

double LRWeakClassifier::train(double** inputX, double* inputY, double* weights, const int rows, const int colsX){
	if(inputX == NULL || inputY == NULL || weights == NULL){
		printf("LRWeakClassifier::train(double**, double*, double*, int, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	bool *toBeTrain = new bool[rows];
	int selected = tryMark(toBeTrain, weights, rows);
	if(selected <= 0){
		
		for(int i = 0; i < rows; i++){
			toBeTrain[i] = 1;
			selected++;
		}
	}else if(selected > 0 && selected <= rows * 0.001){

		delete[] toBeTrain;
		return 1.0;
	}else{

		selected = tryMark(toBeTrain, weights, rows);
	}
	double errorClassified = (selected * 1.0 / rows * 100);
	errorClassifiedRate = errorClassified;

	double** toBeTrainDataX = new double*[selected];
	double* toBeTrainDataY = new double[selected];
	int last = -1;
	for(int i = 0; i < rows; i++){
		if(toBeTrain[i]){
			last++;
			toBeTrainDataX[last] = inputX[i];
			toBeTrainDataY[last] = inputY[i];
		}
	}
	
	mLr = new LinearRegression();
	mLr->init(colsX);
	//train the whole set or sub sample set?
	double err = mLr->train(toBeTrainDataX, toBeTrainDataY, selected/*rows*/, colsX);
	delete[] toBeTrainDataY;
	delete[] toBeTrainDataX;
	delete[] toBeTrain;
	return err;
}
