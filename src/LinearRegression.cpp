#include"LinearRegression.h"
LinearRegression::~LinearRegression(){
	if(mWeights != NULL){
		delete[] mWeights;
		mWeights = NULL;
	}
}

LinearRegression::LinearRegression(){
	lr = 0.01;
	EPOCH = rand()% 2000 + 1500;
}
double LinearRegression::getBias(){
	return mBias;
}

double LinearRegression::train(double** trainX, double* trainY, const int rows, const int colsX){
	if(rows == 0 || colsX == 0){
		printf("LinearRegression::train(double**, double*, int, int) invalid dim \n");
		int invalidDim = 4;
		throw invalidDim;
	}

	for(int epoch = 0; epoch < EPOCH; epoch++){
		//sequence gradient descent. not stochastic.

		for(int i = 0; i < rows; i++){
			double res = predict(trainX[i], colsX);
			if(trainY[i] * res <= 0){
				//update W and b.
				for(int j = 0; j < colsX; j++){
					mWeights[j] += lr * trainY[i] * trainX[i][j];//ok
				}
				mBias += lr * trainY[i];//ok
			}
		}
		
	}

/*
	for(int epoch = 0; epoch < EPOCH; epoch++){
		double deltaB = 0;
		for(int i = 0; i < rows; i++){
			deltaB += predict(trainX[i], colsX) - trainY[i];
		}

		double* deltaW = new double[colsX];
		memset((void*)deltaW, 0, sizeof(double) * colsX);//using bytes unit.
		for(int j = 0; j < colsX; j++){
			for(int i = 0; i < rows; i++){
				deltaW[j] += (predict(trainX[i], colsX) - trainY[i]) * trainX[i][j];
				//printf("trainX=%f\n",trainX[i][j]);
				//getchar();
			}
		}

		mBias -= lr * (1.0 / rows) * deltaB;
		for(int i = 0; i < colsX; i++){
			mWeights[i] -= lr * (1.0 / rows) * deltaW[i];
		}
		delete[] deltaW;
		
		double mse = 0;
		//printf("rows=%d  cols=%d\n", rows, colsX);
		for(int i = 0; i < rows; i++){
			double res = (predict(trainX[i], colsX) - trainY[i]);
			//if(epoch % 100 == 0)
			//printf("res=%10.4f  trainY=%10.4f\n",predict(trainX[i], colsX), trainY[i]);
			mse += (res * res);
		}
	}
	*/

	double errTimes = 0;
	for(int i = 0; i < rows; i++){
		double cls = predict(trainX[i], colsX);
		if(cls * trainY[i] <= 0){
			errTimes++;
		}
	}
	//printf("%f\n", (errTimes / rows));
	//getchar();
	return errTimes / rows;

}


double LinearRegression::predict(double* trainX, int colsX){
	double res = 0;
	for(int i = 0; i < colsX; i++){
		res += mWeights[i] * trainX[i];
		
	}
	res += mBias;
	//printf("mse=%f\n", res);
	return res;
}

/** X dim , exclude Y**/
void LinearRegression::init(const int colsX){
	
	mWeights = new double[colsX];
	memset((void*)mWeights, 0, sizeof(double) * colsX);//using bytes unit.
	//randomly init W and b
	double mid = RAND_MAX / 2.0;

	mBias = (rand() - mid) / mid * (rand()% 1000);
	for(int i = 0; i < colsX; i++){
		mWeights[i] = (rand() - mid) / mid * (rand()% 1000);
	}
}

