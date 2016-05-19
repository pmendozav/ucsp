

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
}
void prob1_1_v2(const vector<vector<float>> &A, const vector<vector<float>> &B)
{
	clock_t 
		t;
	int 
		i, 
		j, 
		k;
	vector<vector<float>>
		res;
	float
		tmp;
	
	res.resize(N);
	fori(i, 0, N) {
		res[i].resize(N);
	}

	t = clock();
	fori(i, 0, N) {
		fori(j, 0, N) {
			tmp = 0;
			fori(k, 0, N) {
				tmp += A[i][k] * B[k][j];
			}
			res[i][j] = tmp;
		}
	}
	t = clock() - t;

	cout << "time: " << (float)t/CLOCKS_PER_SEC << endl;
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
}
void prob1_2_v2(const vector<vector<float>> &A, const vector<vector<float>> &B)
{
	clock_t 
		t;
	int 
		i, 
		j,
		k;
	vector<vector<float>> 
		tmp, 
		res;
	
	tmp.resize(N);
	res.resize(N);
	
	fori(i, 0, N) {
		tmp[i].resize(N);
		res[i].resize(N);
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
}
void prob2_1_v2(const vector<vector<float>> &A, const vector<vector<float>> &B)
{
	clock_t t;
	int i, j , k, count_a, count_b;
	int i2, k2, j2;
	vector<vector<float>> RES;
	const float *a, *b;
	float *res;
	

	RES.resize(N);
	fori(i, 0, N) {
		RES[i].resize(N);
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
}

//product matrix implementation using 6 loops and only 1-dimensional array
void prob2_2(const float *A, const float *B)
{
	clock_t t;
	int i, j , k;
	int i2, k2, j2;
	float *RES;
	const float *a, *b;
	float *res;
	
	RES = new float[N*N];
	memset(RES, 0, N*N*sizeof(float));

	t = clock();
	float s = 0;
	for (i = 0; i < N; i += SM) {
		for (j = 0; j < N; j += SM) {
			for (k = 0; k < N; k += SM) {
				res = &RES[N*i + j];
				a = &A[N*i + k];
				for (i2 = 0; i2 < SM; i2++) {
					b = &B[N*k + j];
					for (k2 = 0; k2 < SM; k2++) {
						for (j2 = 0; j2 < SM; j2++) {
							res[j2] += a[k2] * b[j2];
						}
						b += N;
					}
					res += N;
					a += N;
				}
			}
		}
	}
	t = clock() - t;

	cout << "time: " << (float)t/CLOCKS_PER_SEC << endl;
}
void prob2_2_v2(const vector<float> &A, const vector<float> &B)
{
	clock_t t;
	int i, j , k;
	int i2, k2, j2;
	vector<float> RES;
	const float *a, *b;
	float *res;
	
	RES.resize(N*N);

	t = clock();
	float s = 0;
	for (i = 0; i < N; i += SM) {
		for (j = 0; j < N; j += SM) {
			for (k = 0; k < N; k += SM) {
				res = &RES[N*i + j];
				a = &A[N*i + k];
				for (i2 = 0; i2 < SM; i2++) {
					b = &B[N*k + j];
					for (k2 = 0; k2 < SM; k2++) {
						for (j2 = 0; j2 < SM; j2++) {
							res[j2] += a[k2] * b[j2];
						}
						b += N;
					}
					res += N;
					a += N;
				}
			}
		}
	}
	t = clock() - t;

	cout << "time: " << (float)t/CLOCKS_PER_SEC << endl;
}
