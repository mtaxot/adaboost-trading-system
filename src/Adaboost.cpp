#include<vector>
#include<math.h>
#include"Adaboost.h"


void Adaboost::initWeights(){
	for(int i = 0; i < mRows; i++){
		mWeights[i] = 1.0 / mRows;
	}
}

Adaboost::~Adaboost(){
	delete[] mWeights;
	delete[] mInputY;
	
	for(int i = 0; i < mRows; i++){
		delete[] mInputX[i];
	}
	delete[] mInputX;
	for(unsigned int i = 0; i < mWeakClassifierSet->size(); i++){
		delete mWeakClassifierSet->at(i);
	}
	mWeakClassifierSet->clear();
	std::vector<LRWeakClassifier*>(*mWeakClassifierSet).swap(*mWeakClassifierSet);
	
	delete mWeakClassifierSet;
}
Adaboost::Adaboost(){
	mInputX = NULL;
	mInputY = NULL;
	mWeights = NULL;
	mUseModel0 = 0;
	mRows = 0;
	mColsX = 0;
}
Adaboost::Adaboost(double** X, double* Y, const int rows, const int cols){
	if(X == NULL || rows == 0 ||
		Y == NULL || cols == 0){
		printf("Adaboost::Adaboost(double** X, double* Y, int, int) invalid input dim, please check !\n");
		int noData = 1;
		throw noData;
	}
	//make a copy of input, for memory management.
	mRows = rows;
	mColsX = cols;
	double** cpX = new double*[rows];
	
	double* cpY = new double[rows];
	for(int i = 0; i < rows; i++){
		cpY[i] = Y[i];
		cpX[i] = new double[cols];
		for(int j = 0; j < cols; j++){
			cpX[i][j] = X[i][j];
		}
	}
	setInput(cpX, cpY, rows, cols);
	
}



Adaboost::Adaboost(double** input, const int rows, const int colsAll){
	if(input == NULL || rows == 0 || colsAll == 0){
		printf("Adaboost::Adaboost(double**, int, int) no input data, please check !\n");
		int noData = 1;
		throw noData;
	}
	mRows = rows;
	mColsX = colsAll - 1;
	double** X = new double*[rows];//[cols];
	double* Y = new double[rows];
	for(int i = 0; i < rows; i++){
		X[i] = new double[colsAll - 1];
		for(int j = 0; j < colsAll; j++){
			if(j < colsAll - 1){
				X[i][j] = input[i][j];
			}else{
				Y[i] = input[i][j];
			}
		}
	}
	
	setInput(X, Y, rows, mColsX);
}

void Adaboost::setInput(double** X, double* Y, const int rows, const int colsX){
	if(X == NULL || Y == NULL || colsX == 0 || rows == 0){
		printf("Adaboost::setInput(double**, double*, int, int),input X or input Y can not be null, please check!\n");
		int noInputXorInputY = 2;
		throw noInputXorInputY;
	}
	mRows = rows;
	mColsX = colsX;

	mInputX = X;
	mInputY = Y;
	mWeights = new double[rows];
	memset((void*)mWeights, 0, sizeof(double) * rows);//using bytes unit.
	mWeakClassifierSet = new vector<LRWeakClassifier*>();
}

double Adaboost::predict(double* x){
	if(x == NULL){
		printf("Adaboost::predict(double*), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double res = 0;
	if(mWeakClassifierSet->size() == 0){
		printf("no weak classifiers found!!\n");
		int noMode = 4;
		throw noMode;
	}
	for(unsigned int i = 0; i < mWeakClassifierSet->size(); i++){
		res += mWeakClassifierSet->at(i)->weight * mWeakClassifierSet->at(i)->predict(x, mColsX);
	}
	return res;
}

double* Adaboost::testTrainedModel(double** testX, double* testY){
	if(testX == NULL || testY == NULL){
		printf("Adaboost::testTrainedModel(double**, double*), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double errTimes = 0;
	double sumTimes = 0;

	double errTimes1 = 0;
	double sumTimes1 = 0;

	for(int i = 0; i < mRows; i++){
	
		double res = predict(testX[i]);
		if(testY[i] <= 0){
			sumTimes++;
			if(res > 0){
				errTimes++;
			}
		}else{
			sumTimes1++;
			if(res < 0){
				errTimes1++;
			}
		}
		/*
		double res = predict(testX[i]);
		if(res > 0){
			sumTimes++;
			if(testY[i] <= 0){
				errTimes++;
			}
		}else{
			sumTimes1++;
			if(testY[i] > 0){
				errTimes1++;
			}
		}
		*/
	}
	
	double* err = new double[2];
	memset((void*)err, 0, sizeof(double) * 2);//using bytes unit.
	//up layer release it
	err[0] = errTimes / (sumTimes + 0.000001);
	err[1] = errTimes1 / (sumTimes1 + 0.000001);
	return err;
}

void Adaboost::updateWeights(int* rightOrWrong, double alpha){
	if(rightOrWrong == NULL){
		printf("Adaboost::updateWeights(int*, double), param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}
	double Z = 0;
	for(int i = 0; i < mRows; i++){
		if(rightOrWrong[i] == WeakClassifier::RIGHT){
			mWeights[i] *= exp(-alpha);
		}else if(rightOrWrong[i] == WeakClassifier::WRONG){
			mWeights[i] *= exp(alpha);
		}else{
			printf("unknown right or wrong flag, please check!\n");
			int unknownFlag = 5;
			throw unknownFlag;
		}
		Z += mWeights[i];
	}

	for(int i = 0; i < mRows; i++){
		mWeights[i] /= Z;
	}
}
void Adaboost::trainWeakClassifiers(int epoch, double* trainRate){
	if(epoch <= 1){
		printf("training epoch must be greater than 1, please check!\n");
		int epochSmall = 6;
		throw epochSmall;
	}
	if(mInputX == NULL || mInputY == NULL || mWeights == NULL){
		printf("Adaboost::trainWeakClassifiers(int, double*), class member param NULL\n");
		int paramNull = 9;
		throw paramNull;
	}

	printf("start training......\n");
	initWeights();
	double sumErr0 = 0;
	double sumErr1 = 0;
	const int rows = mRows;
	int* rightOrWrong = new int[rows];
	for(int i = 0; i < epoch; i++){
		
		const int rows = mRows;
		const int colsX = mColsX;
		//printf("epoch %d\n", i);

		LRWeakClassifier* weakClassifier = new LRWeakClassifier();
		double trainSubClassError = (*weakClassifier).train(mInputX, mInputY, mWeights, rows, colsX);
		//printf("%f\n", trainSubClassError);
		//getchar();
		
		if(trainSubClassError >= 0.5){
			delete weakClassifier;
			continue;
		}

		
		memset((void*)rightOrWrong, 0, sizeof(int) * rows);//using bytes unit.

		double errorP = weakClassifier->calculateErrorPositive(mInputX, mInputY, mWeights, rows, colsX);//segment falt
		double errorN = weakClassifier->calculateErrorNegative(mInputX, mInputY, mWeights, rows, colsX);
		double errorSum = weakClassifier->calculateErrorSum(mInputX, mInputY, mWeights, rows, colsX);
		double error = weakClassifier->calculateAdaBoostError(mInputX, mInputY, mWeights, rightOrWrong, rows, colsX);
		double ir0 = weakClassifier->calculateIRP(mInputX, mInputY, trainRate, rows, colsX);
		double ir1 = weakClassifier->calculateIRN(mInputX, mInputY, trainRate, rows, colsX);
		
		if(i % 100 == 0){
			printf("Epoch [   ] %7d\t ECR=%0.4f\t EP=%0.4f\t EN=%0.4f\t SUME=%0.4f\t E=%0.4f\t IR0=%0.4f\t IR1=%0.4f\t SUMIR=%0.4f\n",
			  i, weakClassifier->errorClassifiedRate, errorP, errorN, errorSum, error, ir0, ir1, (ir0 + ir1));
			//printf("Epoch %d\n", i);
		}
		if(errorP >= 0.5 || errorN >= 0.5 || error >= 0.5 || errorSum>= 0.5 || !(ir0>=0 && ir1<=0)){
			delete weakClassifier;
			continue;
		}
		
		if(error == 0){ error = 0.01;}
		if(error == 1){ error = 0.99;}

		double alpha = log((1.0 - error) / error) / 2;
		weakClassifier->weight = alpha;
		//printf("alpha = %f\n", alpha );
		updateWeights(rightOrWrong, alpha);

		mWeakClassifierSet->push_back(weakClassifier);
		double *enp = testTrainedModel(mInputX, mInputY);
		//remove one that could lead adaboost to bad performance.
		if(enp[0] >=0.5 || enp[1] >= 0.5){
			vector<LRWeakClassifier*>::iterator it = mWeakClassifierSet->end();
			--it;
			LRWeakClassifier* badone = mWeakClassifierSet->at(mWeakClassifierSet->size()-1);
			mWeakClassifierSet->erase(it);
			delete badone;
		}else{
			printf("Epoch [GOT] %7d\t ECR=%0.4f\t EP=%0.4f\t EN=%0.4f\t SUME=%0.4f\t E=%0.4f\t IR0=%0.4f\t IR1=%0.4f\t SUMIR=%0.4f\n",
			  i, weakClassifier->errorClassifiedRate, enp[0], enp[1], errorSum, error, ir0, ir1, (ir0 + ir1));
		}
		
		delete[] enp;
		
	}
	delete[] rightOrWrong;

	double* errarr = testTrainedModel(mInputX, mInputY);
	sumErr0 = errarr[0];
	sumErr1 = errarr[1];
	delete[] errarr;
	printf("\ncalculate trained model:E0=%0.4f  E1=%0.4f\n\n", sumErr0, sumErr1);
	if(sumErr0 < sumErr1){
		mUseModel0 = 1;
		printf("Use model 0\n\n");
	}else{
		mUseModel0 = 0;
		printf("Use model 1, but result is unreliable !!!!!\n\n");
	}
	printf("train over!!  %ld weak classifier(s) was trained !!!\n\n", mWeakClassifierSet->size());
}

double Adaboost::getSumAlpha(){
	double res = 0;
	for(unsigned int i = 0; i < mWeakClassifierSet->size(); i++){
		res += mWeakClassifierSet->at(i)->weight;
	}

	return res;
}