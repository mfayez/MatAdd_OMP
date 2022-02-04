// MatrixMulitplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <fstream>
#include <omp.h>

void SaveMatrix(int* mat, int iSize, char* fileName) {
	std::ofstream file(fileName, std::ofstream::out);
	file << iSize << " " << iSize << "\n";
	for (int i = 0; i < iSize; i++)
	{
		for (int j = 0; j < iSize; j++)
			file << *mat++ << "\t";
		file << "\n";
	}
	file.close();
}

int main()
{
	int iThreadsCount = 0;
	int iSizeIndex = 0;
	int iMatrixSize = 0;
	int i1DSize = 0;
	int* matA;
	int* matB;
	int* matC;
	char szFileName[100];
	
	std::cout << "How many cores do you want to use [1 - " << omp_get_max_threads() << "]? ";
	std::cin >> iThreadsCount;
	while (iThreadsCount < 1 || iThreadsCount > omp_get_max_threads()) {
		std::cout << "Number of cores must be in the correct range [1 - " << omp_get_max_threads() << "]? ";
		std::cin >> iThreadsCount;
	}
	std::cout << "1- 100x100\n"
		<< "2- 200x200\n"
		<< "3- 300x300\n"
		<< "4- 400x400\n"
		<< "5- 500x500\n"
		<< "6- 600x600\n"
		<< "7- 700x700\n"
		<< "8- 800x800\n"
		<< "9- 900x900\n"
		<< "10- 1000x1000\n";
	
	while (iSizeIndex < 1 || iThreadsCount > 10) {
		std::cout << "Select matrix size from previous options [1-10]: ";
		std::cin >> iSizeIndex;
	}
	iMatrixSize = iSizeIndex * 100;
	i1DSize = iMatrixSize * iMatrixSize;
	std::cout << "Allocating memory for the matrices\n";
	matA = new int[i1DSize];
	matB = new int[i1DSize];
	matC = new int[i1DSize];

	std::cout << "Initalizing matrices with random numbers\n";
	for (int i = 0; i < i1DSize; i++)
	{
		matA[i] = rand();
		matB[i] = rand();
	}
	std::cout << "Saving Matrix A, and Matrix B\n";
	sprintf_s(szFileName, 100, "Files/A_%dx%d.txt", iMatrixSize, iMatrixSize);
	SaveMatrix(matA, iMatrixSize, szFileName);
	sprintf_s(szFileName, 100, "Files/B_%dx%d.txt", iMatrixSize, iMatrixSize);
	SaveMatrix(matB, iMatrixSize, szFileName);
	std::cout << "Calcaulating sum of Matrix A and B using " << iThreadsCount << " core(s)\n";
	omp_set_num_threads(iThreadsCount);
	double start;
	double end;
	start = omp_get_wtime();
#pragma omp parallel for
	for (int i = 0; i < i1DSize; i++)
		matC[i] = matA[i] + matB[i];
	end = omp_get_wtime();
	std::cout << "Time consumed during addition is " << (end-start) << " seconds\n";

	std::cout << "Saving output matrix to disk\n";
	sprintf_s(szFileName, 100, "Files/C_%dx%d.txt", iMatrixSize, iMatrixSize);
	SaveMatrix(matC, iMatrixSize, szFileName);
}