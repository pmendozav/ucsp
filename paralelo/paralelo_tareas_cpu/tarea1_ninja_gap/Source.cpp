#include <iostream>
#include <list>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <array>
#include <string>

#include <omp.h>

using namespace std;

#pragma region Convolution2d
void conv2dSerial(const vector<vector<float>> &img, 
				const vector<vector<float>> &kernel,
				vector<vector<float>> &result, 
				const int &dim, 
				const int &r)
{
	float sum;

	//#pragma loop(no_vector)
	for (int i=r; i<dim-r; i++)
	{
		//#pragma loop(no_vector)
		for (int j=r; j<dim-r; j++)
		{
			sum = 0;
			for (int m=-r; m<=r; m++)
			{
				#pragma unroll
				for (int n=-r; n<=r; n++)
				{
					sum += img[i+m][j+n] * kernel[m+r][n+r];
				}
			}

			result[i][j] = sum;
		}
	}
}
void conv2dPar(const vector<vector<float>> &img, 
				const vector<vector<float>> &kernel,
				vector<vector<float>> &result, 
				const int &dim, 
				const int &r)
{
	float sum;

	
	#pragma omp parallel for private(sum)
	for (int i=r; i<dim-r; i++)
	{
		//#pragma loop(no_vector)
		for (int j=r; j<dim-r; j++)
		{
			sum = 0;
			//#pragma loop(no_vector)
			#pragma unroll
			for (int m=-r; m<=r; m++)
			{
				for (int n=-r; n<=r; n++)
				{
					sum += img[i+m][j+n] * kernel[m+r][n+r];
				}
			}

			result[i][j] = sum;
		}
	}
}
void conv2d(const int &N, 
			const int &r)
{
	double 
		t;
	vector<vector<float>> 
		img,
		result,
		kernel;
	
	img.resize(N);
	result.resize(N);
	kernel.resize(2*r + 1);

	for (int i=0; i<N; i++)
		img[i].resize(N);

	for (int i=0; i<2*r + 1; i++)
		kernel[i].resize(2*r + 1);
		
	for (int i=0; i<N; i++)
		result[i].resize(N);

	cout << "*****************Convolution 2D*****************" << endl;
	cout << "----Serial:" << endl;
	t = omp_get_wtime();
	conv2dSerial(img, kernel, result, N, r);
	t = omp_get_wtime() - t;
	cout << "Execution Time = " << t << endl;
	
	cout << endl << "----Parallel:" << endl;
	t = omp_get_wtime();
	conv2dPar(img, kernel, result, N, r);
	t = omp_get_wtime() - t;
	cout << "Execution Time = " << t << endl;
	cout << "------------------------------------" << endl << endl;

	for (int i=0; i<img.size(); i++) img[i].clear();
	for (int i=0; i<result.size(); i++) result[i].clear();
	for (int i=0; i<kernel.size(); i++) kernel[i].clear();

	img.clear();
	result.clear();
	kernel.clear();
}
#pragma endregion

#pragma region MergeSort
#define SWAP(a,b) {int tmp = a; a = b; b = tmp;}
void checkSort(double start, 
			   double end,
			   vector<int *> a,
			   int size){
	int ok = 1;

	for (int i=0; i < size-1;i++){
		ok &= (*(a[i]) <= *(a[i+1]));
	}

	cout << "Execution Time = " << end-start << endl;
	if (ok)
		cout << "Sorted = true" << endl;
	else
		cout << "Sorted = false" << endl;
}
void merge(vector<int *> a,
		   int size, 
		   vector<int *> tmp){
	int 
		i1,
		i2,
		it;

	//#pragma simd
	//#pragma loop(no_vector)
	for (i1 = 0, i2 = size/2, it = 0; i1 < size/2 && i2 < size; it++)
	{
		if(*(a[i1]) <= *(a[i2]))
			*(tmp[it]) = *(a[i1++]);
		else
			*(tmp[it]) = *(a[i2++]);
	}

	//#pragma simd
	//#pragma loop(no_vector)
	for ( ; i1 < size/2; i1++, it++)
		*(tmp[it]) = *(a[i1]);

	//#pragma simd
	//#pragma loop(no_vector)
	for ( ; i2 < size; i2++, it++)
		*(tmp[it]) = *(a[i2]);

	//#pragma simd
	//#pragma loop(no_vector)
	for (int i=0; i<size; i++)
		*(a[i]) = *(tmp[i]);
}
void mergesort_serial(vector<int *> a,
					  int size, 
					  vector<int *> tmp){
	int i;
	if(size == 2) {
		if(*(a[0]) <= *(a[1]))
			return;
		else {
			SWAP(*(a[0]),*(a[1]));
			return;
		}
	}

	vector<int*> b(a.begin(), a.begin() + size/2);
	vector<int*> c(a.begin() + size/2, a.begin() + size);

	vector<int*> t_b(tmp.begin(), tmp.begin() + size/2);
	vector<int*> t_c(tmp.begin() + size/2, tmp.begin() + size);

	mergesort_serial(b,size/2,t_b);
	mergesort_serial(c,size - size/2,t_c);
	merge(a, size, tmp);
}
void mergesort_parallel(vector<int *> a,
						int size, 
						vector<int *> tmp,int threads){
	if(threads == 1) {
		mergesort_serial(a, size, tmp);
	} else if (threads > 1){
		vector<int *> b(a.begin(), a.begin() + size/2);
		vector<int *> c(a.begin() + size/2, a.begin() + size);

		vector<int *> t_b(tmp.begin(), tmp.begin() + size/2);
		vector<int *> t_c(tmp.begin() + size/2, tmp.begin() + size);

#pragma omp parallel sections
		{
#pragma omp section 
			mergesort_parallel(b, size/2, t_b, threads/2);
#pragma omp section
			mergesort_parallel(c, size-size/2, t_c, threads-threads/2);
		}
		merge(a,size,tmp);
	}
}
void mergeSort(const int &N){
	vector<int *> 
		a,
		a2,
		tmp,
		tmp2;
	double
		t;
	int
		num_threads;

	a.resize(N);
	a2.resize(N);
	tmp.resize(N);
	tmp2.resize(N);

#pragma omp parallel
	{
#pragma omp master
		{
			num_threads = omp_get_num_threads();
		}
	}

	for (int i=0; i<N; i++)
	{
		a[i] = new int;
		a2[i] = new int;
		tmp[i] = new int;
		tmp2[i] = new int;
		*(a[i]) = rand() % N;
		a2[i] = a[i];
	}

	cout << "*****************MergeSort*****************" << endl;
	cout << "----Parallel:" << endl;
	t = omp_get_wtime();
	mergesort_parallel(a, N, tmp, num_threads);
	t = omp_get_wtime() - t;
	cout << "Execution Time = " << t << endl;

	cout << endl << "----Serial:" << endl;
	t = omp_get_wtime();
	mergesort_serial(a, N, tmp);
	t = omp_get_wtime() - t;
	cout << "Execution Time = " << t << endl;
	cout << "------------------------------------" << endl << endl;

	a.clear();
	a2.clear();
	tmp.clear();
	tmp2.clear();
}
#pragma endregion

int main()
{
	int 
		N,
		r,
		threads;

	threads = 4;
	omp_set_num_threads(threads);
	
#pragma omp parallel
	{
#pragma omp master
		{
			printf("Num threads = %d\n", omp_get_num_threads());
		}
	}

	//test Convolution 2d
	N = 15000;
	r = 2;
	conv2d(N, r);

	//test MergeSort
	N = 8388608;
	mergeSort(N);

	cout << "Fin!" << endl;
	//getchar();
	return 0;
}