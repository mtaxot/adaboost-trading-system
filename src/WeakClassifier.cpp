#include"WeakClassifier.h"

double WeakClassifier::calculateErrorPositive(double** inputX, double* inputY, double* weights,  const int rows, const int colsX){
	if(inputX == NULL || inputY == NULL){
		printf("WeakClassifier::calculateErrorPositive(double**, double*, int, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double errorTimes = 0;
	double pnum = 0.000001;
	double err = 0;
	for(int i = 0; i < rows; i++){
		if(inputY[i] >= 0){
			pnum++;
			double res = predict(inputX[i], colsX);
			if(res < 0){
				errorTimes++;
			}
		}
	}
	return errorTimes / pnum;
}


double WeakClassifier::calculateErrorNegative(double** inputX, double* inputY, double* weights, const int rows, const int colsX){
	if(inputX == NULL || inputY == NULL){
		printf("WeakClassifier::calculateErrorNegative(double**, double*, int, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double errorTimes = 0;
	double nnum = 0.000001;
	double err = 0;
	for(int i = 0; i < rows; i++){
		if(inputY[i] < 0){
			nnum++;
			double res = predict(inputX[i], colsX);
			if(res >= 0){
				errorTimes++;
			}
		}
	}
	return errorTimes / nnum;
}

double WeakClassifier::calculateAdaBoostError(double** inputX, double* inputY, double* weights, int* rightOrWrong, const int rows, const int colsX){
	if(inputX == NULL || inputY == NULL || rightOrWrong == NULL || weights == NULL){
		printf("WeakClassifier::calculateAdaBoostError(double**, double*, double*, int*, int, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double err = 0;
	for(int i = 0; i < rows; i++){
		double res = predict(inputX[i], colsX);
		if(inputY[i] * res <= 0){
			rightOrWrong[i] = WRONG;
			err += weights[i];
			//err++;
		}else{
			rightOrWrong[i] = RIGHT;
		}
	}
	//return err / rows;
	return err;
}

double WeakClassifier::calculateErrorSum(double** inputX, double* inputY, double* weights, const int rows, const int colsX){
	if(inputX == NULL || inputY == NULL || weights == NULL){
		printf("WeakClassifier::calculateErrorSum(double**, double*, double*, int, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double err = 0.000001;
	for(int i = 0; i < rows; i++){
		double res = predict(inputX[i], colsX);
		if(inputY[i] * res <= 0){
			err++;
		}
	}
	return err / rows;
}




double WeakClassifier::calculateIRP(double** inputX, double* inputY, double* trainRate, const int rows, const int colsX){
	if(inputX == NULL || inputY == NULL || trainRate == NULL){
		printf("WeakClassifier::calculateError(double**, double*, double*, int, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double ir = 0;
	for(int i = 0; i < rows; i++){
		double res = predict(inputX[i], colsX);
		if(res > 0){
			ir += trainRate[i];
		}
	}
	return ir;
}

double WeakClassifier::calculateIRN(double** inputX, double* inputY, double* trainRate, const int rows, const int colsX){
	if(inputX == NULL || inputY == NULL || trainRate == NULL){
		printf("WeakClassifier::calculateIRN(double**, double*, double*, int, int), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double ir = 0;
	for(int i = 0; i < rows; i++){
		double res = predict(inputX[i], colsX);
		if(res < 0){
			ir += trainRate[i];
		}
	}
	return ir;
}