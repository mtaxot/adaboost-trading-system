#include<stdio.h>
#ifndef WEAKCLASSIFIER_H
#define WEAKCLASSIFIER_H
class WeakClassifier{
	public:
		static const int RIGHT = 1;

		static const int WRONG = 0;

		double weight;

		double errorClassifiedRate;

		double calculateErrorPositive(double** inputX, double* inputY, double* weights, const int rows, const int colsX);

		double calculateErrorNegative(double** inputX, double* inputY, double* weights, const int rows, const int colsX);

		double calculateErrorSum(double** inputX, double* inputY, double* weights, const int rows, const int colsX);

		double calculateAdaBoostError(double** inputX, double* inputY, double* weights, int* rightOrWrong, const int rows, const int colsX);
		
		double calculateIRP(double** inputX, double* inputY, double* trainRate, const int rows, const int colsX);

		double calculateIRN(double** inputX, double* inputY, double* trainRate, const int rows, const int colsX);

		virtual double predict(double *x, const int colsX) = 0;

		virtual double train(double** inputX, double* inputY, double* weights, const int rows, const int colsX) = 0;
};
#endif