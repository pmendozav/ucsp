#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <mpi.h>

using namespace std;

#define DEBUG

//global variables
int nTasks, id;
//
#include "helper.h"
#include "program.h"

void nBody(int argc, char** argv)
{
	Input 
		in;
	MPI_Input
		mpi_in;

	in = readInput(100);
	n_body_serial_v1(in);
	//n_body_serial_v2(in);
	//n_body_omp_v1(in);
	//n_body_omp_v2(in);
	//n_body_omp_v3(in);

	//MPI_Init(&argc,&argv);

	//MPI_Comm_size(MPI_COMM_WORLD,&nTasks);
	//MPI_Comm_rank(MPI_COMM_WORLD,&id);
	//
	//if (id == 0)
	//	mpi_in = read_MPI_Input(argc, argv);

	//n_body_mpi_v1(mpi_in);
	//n_body_mpi_v2(mpi_in);

	//MPI_Finalize();
}

int main(int argc, char** argv)
{
	nBody(argc, argv);
	
	//tsp(argc, argv);


	cout << endl << endl << "Fin!" << endl;
	getchar();
	return 0;
}
