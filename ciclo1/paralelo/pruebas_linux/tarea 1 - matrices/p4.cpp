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
	vector<float> A2(N*N);
	vector<float> B2(N*N);
	
	cout << "t4:" << endl;
	prob2_2_v2(A2, B2);
	cout << "-------" << endl;

	return 0;
}
