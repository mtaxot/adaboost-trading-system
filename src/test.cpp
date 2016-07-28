#include <stdio.h>
#include <string.h>
#include "LinearRegression.h"
#include "Adaboost.h"
#include "DoubleMatrix.h"

int main(const int argc, const char** argv){
	srand(0);//must init here
	int epoch = 5000;
	char fileTrain[128];
	char fileTest[128];
	//train -e epoch -c code
	if(argc != 5){
		printf("useage %s -e epoch -c code\n", argv[0]);
		return -1;
	}else{
		const char* e = argv[1];
		if(strcmp(e,"-e") != 0){
			printf("unknown option:%s\n", e);
			printf("useage %s -e epoch -c code\n", argv[0]);
			return -1;
		}

		const char* c = argv[3];
		if(strcmp(c,"-c") != 0){
			printf("unknown option:%s\n", c);
			printf("useage %s -e epoch -c code\n", argv[0]);
			return -1;
		}
		epoch = atoi(argv[2]);
		const char* file = argv[4];
		printf("train epoch is %d  code is %s\n", epoch, file);

		
		strcpy(fileTrain, "../input/");
		strcpy(fileTest, "../input/");
		strcat(fileTrain, file);
		strcat(fileTrain, "_train.txt");
		strcat(fileTest, file);
		strcat(fileTest, "_test.txt");

		//printf("File Train: %s\n", fileTrain);
		//printf("File Test: %s\n", fileTest);
		//getchar();
	}

	//test matrix
	DoubleMatrix *matTrain = new DoubleMatrix();
	//const char* fileTrain = "../input/input_train.txt";
	int resTrain = matTrain->readFromFile(fileTrain, '/');
	if(resTrain < 0)  return -1;
	double** trainX = matTrain->dupMatrix(0, matTrain->getRows(), 1, matTrain->getCols() - 2);//2cols
	matTrain->scale(trainX, matTrain->getRows(), matTrain->getCols() - 3);

	double* trainY = matTrain->dupCol(matTrain->getCols() - 2);
	double* trainRate = matTrain->dupCol(matTrain->getCols() - 1);
	//double* trainDate = matTrain->dupCol(0);



	DoubleMatrix *matTest = new DoubleMatrix();
	//const char* fileTest = "../input/input_test.txt";
	int resTest = matTest->readFromFile(fileTest, '/');
	if(resTest < 0)  return -1;
	double** testX = matTest->dupMatrix(0, matTest->getRows(), 1, matTest->getCols() - 2);//2cols
	matTest->scale(testX, matTest->getRows(), matTest->getCols() - 3);

	double* testY = matTest->dupCol(matTrain->getCols() - 2);
	double* testRate = matTest->dupCol(matTrain->getCols() - 1);
	double* testDate = matTest->dupCol(0);

	Adaboost *adaboost = new Adaboost(trainX, trainY, matTrain->getRows(), matTrain->getCols() - 3);

	

	adaboost->trainWeakClassifiers(epoch, trainRate);
	//predict.
	double ir0 = 0;
	double ir1 = 0;

	double errTimes0 = 0;
	double errTimes1 = 0;

	double sumTimes0 = 0;
	double sumTimes1 = 0;
	
	double sumIRP = 0;
	double sumIRN = 0;
	for(int i = 0; i < matTest->getRows(); i++){
		if(testY[i] > 0){
			sumIRP += testRate[i];
		}else{
			sumIRN += testRate[i];
		}
	}
	for(int i = 0; i < matTest->getRows(); i++){
		double res = adaboost->predict(testX[i]);
		if(res > 0){
			ir0 += testRate[i];
			sumTimes0++;
			if(testY[i] > 0){
				printf("MODEL0: [%0.0f] [ OK  ]  BLF:%7.2f    POINT=%7.2f    IR0=%7.2f\n", testDate[i], res, testRate[i], ir0);
			}else{
				printf("MODEL0: [%0.0f] [ERROR]  BLF:%7.2f    POINT=%7.2f    IR0=%7.2f\n", testDate[i], res, testRate[i], ir0);
				errTimes0++;
			}
		}
		
		if(res < 0){
			ir1 += testRate[i];
			sumTimes1++;
			if(testY[i] < 0){
				//printf("MODEL1: [%0.0f] [ OK  ]  BLF:%7.2f    POINT=%7.2f    IR1=%7.2f\n", testDate[i], res, testRate[i], ir1);
			}else{
				//printf("MODEL1: [%0.0f] [ERROR]  BLF:%7.2f    POINT=%7.2f    IR1=%7.2f\n", testDate[i], res, testRate[i], ir1);
				errTimes1++;
			}
		}
		
	}
	printf("\n\n");
	printf("MODEL0: IR0=%f ACCURACY=%f SUMIRP=%f\n\n", ir0,(1 - errTimes0/(sumTimes0+0.00001)),sumIRP );

	printf("MODEL1: IR1=%f ACCURACY=%f SUMIRN=%f\n\n", ir1,(1 - errTimes1/(sumTimes1+0.00001)),sumIRN );

	printf("SUMIR:%f\n",(ir0+ir1));






	for(int i = 0; i < matTrain->getRows(); i++){
		delete[] trainX[i];
	}
	for(int i = 0; i < matTest->getRows(); i++){
		delete[] testX[i];
	}
	delete[] trainX;
	delete[] trainY;
	delete[] testX;
	delete[] testY;
	delete[] trainRate;
	delete[] testRate;
	//delete[] trainDate;
	delete[] testDate;
	delete matTrain;
	delete matTest;
	delete adaboost;
	return 0;
}