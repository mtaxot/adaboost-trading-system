#include<stdio.h>
#ifndef MATRIX_H
#define MATRIX_H
class DoubleMatrix{
	private:
		int mRows;
		int mCols;
		double** mData;

	public:
		DoubleMatrix();
		//read a matrix from a TXT file.
		int readFromFile(const char* srcFile);

		int readFromFile(const char* srcFile, char removeChar);

		//write a matrix to a TXT file.
		int writeToFile(char* targetFile);

		//return the rows of this matrix.
		int getRows();

		//return the cols of this matrix.
		int getCols();

		//return the element at (i,j) of this matrix.
		double at(int i, int j);

		double* dupCol(int col);

		double* dupRow(int row);

		double** dupMatrix(int startrow, int endrow, int startcol, int endcol);

		int scale(double** mat, int rows, int cols);

		~DoubleMatrix();
};
#endif