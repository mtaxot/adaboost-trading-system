#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fstream>

#include "DoubleMatrix.h"

using namespace std;
DoubleMatrix::DoubleMatrix(){
	mRows = 0;
	mCols = 0;
	mData = NULL;

}


int DoubleMatrix::readFromFile(const char* srcFile){

	return readFromFile(srcFile, '\0');
}

int DoubleMatrix::readFromFile(const char* srcFile, char removeChar){
	if(srcFile == NULL){
		printf("DoubleMatrix::readFromFile(char*) no input file!!\n");
		int noInputFile = 11;
		throw noInputFile;
	}
	if(removeChar == '\t' || removeChar == ' '
	    || removeChar == '\n'){
		printf("removeChar can't be separator!!!");
		return -1;
	}

	ifstream in(srcFile);
	const int bufLen = 256;
	char* buffer = new char[bufLen];

	if(!in.is_open()){
		printf("Error open input file!!\n");
		delete[] buffer;
		in.close();//???
		return -1;
	}

	int rows = 0;
	//count data lines, check empty lines.
	while(!in.eof()){
		in.getline(buffer, bufLen);
		if(strlen(buffer) == 0){
			printf("empty matrix line, please check!!!");
			delete[] buffer;
			in.close();
			return -1;
		}
		rows++;
	}

	//check cols
	int cols = -1;
	in.seekg(0);
	while(!in.eof()){
		int colsTmp = 0;
		int rd = -1;
		int sep = 0;

		memset((void*)buffer, '\0', sizeof(char) * bufLen);//using bytes unit.
		in.getline(buffer, bufLen);
		
		for(int i = 0; i < strlen(buffer) + 1; i++){
			if(buffer[i] != '\t' && buffer[i] != ' ' &&
					           buffer[i] != '\n' && buffer[i] != '\0'){
				sep = 0;
				rd++;
			}else if(buffer[i] == removeChar){
				continue;
			}else{
				if(sep == 0){
					rd = -1;
					colsTmp++;
					sep++;
				}else{
					continue;//skip separator.
				}
			}
		}

		if(cols < 0){
			cols = colsTmp;
		}else{
			if(cols != colsTmp){
				printf("matrix invalid dim!!! %d!=%d please check!\n",cols, colsTmp);
				delete[] buffer;
				in.close();//???
				return -1;
			}
		}
	}

	if(rows <= 0 || cols <= 0){
		printf("matrix no data !!! please check!\n");
		delete[] buffer;
		in.close();
		return -1;
	}

	//printf("matrix rows=%d  cols=%d\n", rows, cols);
	mRows = rows;
	mCols = cols;

	mData = new double*[rows];
	for(int i = 0; i < rows; i++){
		mData[i] = new double[cols];
	}


	// having got the cols, we start read data.
	int idx = -1;
	int jdx = -1;

	in.seekg(0);
	while(!in.eof()){
		idx++;//row index.
		char temp[128];
		int colsTmp = 0;
		memset((void*)temp, '\0', sizeof(temp));//using bytes unit.

		memset((void*)buffer, '\0', sizeof(char) * bufLen);//using bytes unit.
		in.getline(buffer, bufLen);

		if(strlen(buffer) == 0){
			continue;
		}
		int rd = -1;
		int sep = 0;
		jdx = -1;//col index
		for(int i = 0; i < strlen(buffer) +1; i++){
			if(buffer[i] == removeChar){
				continue;
			}
			if(buffer[i] != '\t' && buffer[i] != ' ' &&
					           buffer[i] != '\n' && buffer[i] != '\0'){
				sep = 0;
				rd++;
				temp[rd] = buffer[i];
			}else{
				if(sep == 0){
					jdx++;
					mData[idx][jdx] = atof(temp);
					memset((void*)temp, '\0', sizeof(temp));//using bytes unit.
					rd = -1;
					colsTmp++;
					sep++;
				}else{
					continue;//skip separator.
				}
				
			}
		}
	}
	
	in.close();
	delete[] buffer;
	return 0;
}

int DoubleMatrix::writeToFile(char* targetFile){

	return -1;
}


int DoubleMatrix::getRows(){
	return mRows;
}


int DoubleMatrix::DoubleMatrix::getCols(){
	return mCols;
}


double DoubleMatrix::at(int i, int j){
	return mData[i][j];
}

double* DoubleMatrix::dupCol(int col){
	if(col < 0 || col >= mCols){
		return NULL;
	}
	double* data = new double[mRows];
	for(int i = 0; i < mRows; i++){
		data[i] = mData[i][col];
	}
	return data;
}

double* DoubleMatrix::dupRow(int row){
	if(row < 0 || row >= mRows){
		return NULL;
	}
	double* data = new double[mCols];
	for(int i = 0; i < mCols; i++){
		data[i] = mData[row][i];
	}
	return data;
}

double** DoubleMatrix::dupMatrix(int startrow, int endrow, int startcol, int endcol){
	printf("startrow=%d  endrow=%d  startcol=%d  endcol=%d\n", startrow, endrow, startcol, endcol);
	if(startrow < 0 || startrow > mRows){
		printf("start row invalid!!\n");
		return NULL;
	}
	if(endrow < 0 || endrow > mRows){
		printf("end row invalid!!\n");
		return NULL;
	}
	if(startcol < 0 || startcol > mCols){
		printf("start col invalid!!\n");
		return NULL;
	}
	if(endcol < 0 || endcol > mCols){
		printf("end row invalid!!\n");
		return NULL;
	}
	if(startrow > endrow){
		printf("start end row invalid!!\n");
		return NULL;
	}
	if(startcol > endcol){
		printf("start end col invalid!!\n");
		return NULL;
	}
	int rows = endrow - startrow;
	int cols = endcol - startcol;

	double** data = new double*[rows];

	//startrow=0,endrow=12, startcol=1 endcol=4
	for(int i = 0; i < rows; i++){
		data[i] = new double[cols];
		for(int j = 0; j < cols; j++){
			//printf("i=%d j=%d\n", i,j);
			data[i][j] = mData[startrow + i][startcol + j];
		}
	}
	return data;
}

int DoubleMatrix::scale(double** mat, int rows, int cols){
	double* max = new double[cols];
	double* min = new double[cols];
	double* mean = new double[cols];
	memset((void*)max, 0, sizeof(double) * cols);//using bytes unit.
	memset((void*)min, 0, sizeof(double) * cols);//using bytes unit.
	memset((void*)mean, 0, sizeof(double) * cols);//using bytes unit.
	//find max min mean
	for(int j = 0; j < cols; j++){
		for(int i = 0; i < rows; i++){
			if(mat[i][j] > max[j]){
				max[j] = mat[i][j];
			}

			if(mat[i][j] < min[j]){
				min[j] = mat[i][j];
			}
			mean[j] += mat[i][j];
		}
		mean[j] /= rows;
	}
	
	//start scale
	for(int j = 0; j < cols; j++){
		for(int i = 0; i < rows; i++){
			mat[i][j] = (mat[i][j] - mean[j]) / (max[j] - min[j]);
		}
	}
	delete[] max;
	delete[] min;
	delete[] mean;
}

DoubleMatrix::~DoubleMatrix(){
	if(mData != NULL){
		for(int i = 0; i < mRows; i++){
			if(mData[i] != NULL){
				delete[] mData[i];
			}
			
		}
		delete[] mData;
	}
	
}