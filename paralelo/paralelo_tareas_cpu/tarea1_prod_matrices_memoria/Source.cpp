#include <iostream>
#include <list>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstdio>

//Use DEBUG to try with 4x4 matrices and show the results
//#define DEBUG

//functions *_v2 use std::vector datatype and the others
//use only pointers
//std::vector is more lower than pointers but supports
//arrays with long sizes

#include "helper.h"

//naive product matrix
void prob1_1(const float **A, const float **B)
{
	clock_t t;
	int i, j, k;
	float **res;
	
	res = new float*[N];
	fori(i, 0, N) {
		res[i] = new float[N];
		memset(res[i], 0, N*sizeof(float));
	}

	t = clock();
	fori(i, 0, N) {
		fori(j, 0, N) {
			fori(k, 0, N) {
				res[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	t = clock() - t;

	cout << "time: " << (float)t/CLOCKS_PER_SEC << endl;

#ifdef DEBUG
	plotMat(res);
#endif // DEBUG

	fori(i, 0, N) {
		delete [] res[i];
	}
	delete [] res;
}


//product matrix implementation using A and B'
void prob1_2(const float **A, const float **B)
{
	clock_t t;
	int i, j , k;
	float **tmp, **res;
	
	tmp = new float*[N];
	res = new float*[N];

	fori(i, 0, N) {
		tmp[i] = new float[N];
		res[i] = new float[N];
		memset(res[i], 0, N*sizeof(float));
	}

	t = clock();
	fori(i, 0, N) {
		fori(j, 0, N) {
			tmp[i][j] = B[j][i];
		}
	}

	fori(i, 0, N) {
		fori(j, 0, N) {
			fori(k, 0, N) {
				res[i][j] += A[i][k] * tmp[j][k];
			}
		}
	}
	t = clock() - t;

	cout << "time: " << (float)t/CLOCKS_PER_SEC << endl;
	
#ifdef DEBUG
	plotMat(res);
#endif // DEBUG

	fori(i, 0, N) {
		delete [] res[i];
		delete [] tmp[i];
	}
	delete [] res;
	delete [] tmp;
}

//product matrix implementation using 6 loops
void prob2_1(const float **A, const float **B)
{
	clock_t t;
	int i, j , k, count_a, count_b;
	int i2, k2, j2;
	float **RES;
	const float *a, *b;
	float *res;
	
	RES = new float*[N];
	fori(i, 0, N) {
		RES[i] = new float[N];
		memset(RES[i], 0, N*sizeof(float));
	}

	t = clock();
	float s = 0;
	for (i = 0; i < N; i += SM) {
		for (j = 0; j < N; j += SM) {
			for (k = 0; k < N; k += SM) {
				count_a = 0;
				res = &RES[i][j];
				a = &A[i][k];
				for (i2 = 0; i2 < SM; i2++) {
					count_b = 0;
					b = &B[k][j];
					for (k2 = 0; k2 < SM; k2++) {
						for (j2 = 0; j2 < SM; j2++) {
							res[j2] += a[k2] * b[j2];
						}
						count_b++;
						b = &B[k+count_b][j]; //b += N;
					}
					count_a++;
					res = &RES[i+count_a][j]; //res += N;
					a = &A[i+count_a][k]; //a += N;
				}
			}
		}
	}
	t = clock() - t;

	cout << "time: " << (float)t/CLOCKS_PER_SEC << endl;

#ifdef DEBUG
	plotMat(RES);
#endif // DEBUG

	fori(i, 0, N) {
		delete [] RES[i];
	}
	delete [] RES;

	//delete a, b;
}


int main()
{
	int i;
	float A[N][N], B[N][N];
	
#ifdef DEBUG
	randMat(A);
	randMat(B);

	plotMat(A);
	cout << "-------" << endl;
	plotMat(B);
	cout << "-------" << endl;
	cout << "-------" << endl;
#endif // DEBUG

	cout << "t1:" << endl;
	prob1_1(A, B);
	cout << "-------" << endl;

	cout << "t2:" << endl;
	prob1_2(A, B);
	cout << "-------" << endl;

	cout << "t3:" << endl;
	prob2_1(A, B);
	cout << "-------" << endl;


#ifdef DEBUG
	float _A2[N*N] = {1,2,4,0,
					4,4,3,3,
					2,4,0,0,
					1,2,1,1};
	float _B2[N*N] = {0,2,2,1,
					1,4,2,3,
					2,2,1,1,
					3,0,2,1};
	vector<float> A2(_A2, _A2 + N*N);
	vector<float> B2(_B2, _B2 + N*N);
#else
	//float A2[N*N], B2[N*N];
	vector<float> A2(N*N);
	vector<float> B2(N*N);
#endif // DEBUG

	getchar();
	return 0;
}