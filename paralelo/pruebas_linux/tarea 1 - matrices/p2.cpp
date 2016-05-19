#include <bits/stdc++.h>
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
#include "functions.h"

int main()
{
	int i;
	vector<vector<float>> A, B;
	A.resize(N);
	B.resize(N);
	fori(i, 0, N) 
	{
		A[i].resize(N);
		B[i].resize(N);
	}

	cout << "t2:" << endl;
	prob1_2_v2(A, B);
	cout << "-------" << endl;

	return 0;
}
