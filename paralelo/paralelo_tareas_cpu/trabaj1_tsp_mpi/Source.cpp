#pragma once

#define NUM_THREADS 3

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <stack>
#include <queue>
#include <sstream>
#include <mpi.h>

using namespace std;
#define INF 1 << 30
#define MAX_SIZE 50

#include "helper.h"

MPI_Datatype MPI_tour;
static void BuildMPITourType()
{
	int blocks[3] = {1, 1, MAX_SIZE};

	MPI_Aint intex;
	MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
	MPI_Aint displacements[3];
	
	MPI_Type_extent(MPI_INT, &intex);
	displacements[0] = static_cast<MPI_Aint>(0);
	displacements[1] = intex;
	displacements[2] = intex + intex;

	MPI_Type_struct(3, blocks, displacements, types, &MPI_tour);

	

	//return MPI_tour;
}


//there aren't v1 because it's the same that the serial version
//void dfs_omp_v2(stack<Tour *> &s, Tour *best_tour, int num_cities, int **G) 
//{
//	int city, cost, root_id, next_city;
//	int idx, current_city;
//
//	while (!s.empty())
//	{
//		Tour *cur_tour = s.top();
//		s.pop();
//		if (cur_tour->num_cities == num_cities)
//		{
//			cost = 0;
//			idx = -1;
//			current_city = cur_tour->visited[cur_tour->num_cities - 1];
//			
//			if ((cost = G[current_city][0]) == INF) continue;
//
//			if(cur_tour->cost + cost < best_tour->cost)
//			{
//				*best_tour = *cur_tour;
//				//best_tour = Clone(cur_tour);
//				best_tour->cost += cost;
//				best_tour->visited[best_tour->num_cities++] = 0;
//			}
//		}
//		else {
//			current_city = cur_tour->visited[cur_tour->num_cities - 1];
//			for (int next_city = 0; next_city < num_cities; next_city++) 
//			{
//				if(IsFeasible(cur_tour, G, current_city, next_city)) 
//				{
//					cost = G[current_city][next_city];
//					AddCity(cur_tour, next_city, cost);
//					
//					Tour *new_tour = new Tour;
//					*new_tour = *cur_tour;
//					//new_tour = Clone(cur_tour);
//					s.push(new_tour);
//					RemoveLastCity(cur_tour, next_city, cost);
//				}
//			}
//		}
//	}
//}

int nTasks, myrank;


//bool Terminate(stack<Tour *> &my_stack, int flag)
//{
//	int whereis = -1;
//	//int my_stack_size = my_stack.size();
//
//	if (my_stack.size() >= 2 && 
//		threads_in_cond_wait > 0 &&
//		new_stack.size() == 0)
//	{
//		pthread_mutex_trylock(&term_mutex);
//		if (threads_in_cond_wait > 0 &&
//			new_stack.size() == 0)
//		{
//			//split my_stack -> my_stack, new_stack
//			for (int i = 0; i < my_stack.size() / 2; i++)
//			{
//				new_stack.push(my_stack.top());
//				my_stack.pop();
//			}
//				
//			pthread_cond_signal(&term_cond_var);
//		}
//		pthread_mutex_unlock(&term_mutex);
//		return false;
//	}
//	else if (my_stack.size() != 0) //keep working
//	{
//		return false; //continue working
//	}
//	else //my_stack is empty
//	{
//		pthread_mutex_trylock(&term_mutex);
//		if (threads_in_cond_wait == NUM_THREADS - 1)//last threag running
//		{
//			threads_in_cond_wait++;
//			pthread_cond_broadcast(&term_cond_var);
//			pthread_mutex_unlock(&term_mutex);
//			return true; //quit
//		}
//		else //other threads still working, wait for a work
//		{
//			threads_in_cond_wait++;
//			while (pthread_cond_wait(&term_cond_var, &term_mutex) != 0);
//			//{
//				if (threads_in_cond_wait < NUM_THREADS)//we got work
//				{
//					my_stack = new_stack;
//					while (!new_stack.empty()){new_stack.pop();}
//					threads_in_cond_wait--;
//					pthread_mutex_unlock(&term_mutex);
//					return false;
//				}
//				else //all threads done
//				{
//					pthread_mutex_unlock(&term_mutex);
//					return true;
//				}
//			//}
//		}
//	}
//
//	return false;
//}
//
//void dfs_pthreads(stack<Tour *> &my_stack, int num_cities, vector<vector<int>> G, int flag) 
//{
//	int city, cost, root_id, next_city;
//	int idx, current_city;
//
//	//echo(my_stack.size());
//	while (!Terminate(my_stack, flag))
//	{
//		Tour *cur_tour = my_stack.top();
//		my_stack.pop();
//		
//		if (cur_tour->visited.size() == num_cities)
//		{
//			cost = 0;
//			idx = -1;
//			current_city = cur_tour->visited[cur_tour->visited.size() - 1];
//			
//			if ((cost = G[current_city][0]) == INF) continue;
//
//			pthread_mutex_lock(&best_tour_mutex);
//			if(cur_tour->cost + cost < best_tour->cost)
//			{
//				*best_tour = *cur_tour; //best_tour = Clone(cur_tour);
//				best_tour->cost += cost;
//				best_tour->visited.push_back(0);
//			}
//			//cout << "qqq:" << best_tour->cost << endl;
//			pthread_mutex_unlock(&best_tour_mutex);
//		}
//		else 
//		{
//			current_city = cur_tour->visited[cur_tour->visited.size() - 1];
//			for (int next_city = 0; next_city < G.size(); next_city++) 
//			{
//				if(IsFeasible(cur_tour, G, current_city, next_city)) 
//				{
//					cost = G[current_city][next_city];
//					AddCity(cur_tour, next_city, cost);
//					
//					Tour *new_tour = new Tour;
//					*new_tour = *cur_tour; //new_tour = Clone(cur_tour);
//					my_stack.push(new_tour);
//					RemoveLastCity(cur_tour, next_city, cost);
//				}
//			}
//		}
//	}
//
//	pthread_exit(NULL);
//
//	cout << "best_tour = " << best_tour->cost << endl;
//}

void tsp_mpi(int argc, char** argv) 
{
	////////////////////////////////////////////////////estaban como glbal sobre Terminate
	Tour2 *g_best_tour, *l_best_tour;
	vector<Tour2 *> v_l_best_tour;
	vector<stack<Tour2 *>> l_tours_pthread;
	int threads_in_cond_wait = 0;
	stack<Tour2 *> new_stack;
	int num_cities;
	//vector<Tour *> g_stack;
	
	/////////////////////////////////////////////////////
	
	int n_local_queue_size, n_global_queue_size;
	int best_cost, cost;
	Tour2 *tour;
	
	Input_MPI in;

	vector<Tour2*> l_stack, g_stack;
	int *local_sizes, *local_offsets;
		
	g_best_tour = CreateTour2();
	l_best_tour =  CreateTour2();

	if (myrank == 0)
	{
		Initialize(in);
		//v_l_best_tour.resize(nTasks);
		//for (int i = 0; i < nTasks; i++)
		//{
		//	v_l_best_tour[i] = new Tour;
		//	v_l_best_tour[i]->cost = INF;
		//}

		for (int level = 0; level < in.num_cities; level++)
		{
			g_stack = PartitionTree(in.G_m, in.num_cities, level);
			if (g_stack.size() >= nTasks)
				break;
		}
		//cout << g_stack.size() <<  endl;
		n_local_queue_size = g_stack.size() / nTasks;
		//n_local_queue_size = 4;

		local_sizes = new int[nTasks];
		local_offsets = new int[nTasks];

		for (int i = 0; i < nTasks-1; i++)
			local_sizes[i] = n_local_queue_size;
		local_sizes[nTasks - 1] = g_stack.size() - n_local_queue_size*nTasks;

		for (int i = 0; i < nTasks; i++)
			local_offsets[i] = i * local_sizes[0];
	}
	MPI_Barrier(MPI_COMM_WORLD);

	if (myrank != 0)
	{
		local_sizes = new int[nTasks];
		local_offsets = new int[nTasks];
		g_stack.resize(9);
	}

	MPI_Bcast(&local_sizes[0], nTasks, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&local_offsets[0], nTasks, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&in.num_cities, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (myrank != 0)
		in.G_v = new int[in.num_cities*in.num_cities];
	

//	cout << myrank << endl;
	//MPI_Bcast(asd, 
	//		10, 
	//		MPI_tour, 
	//		0, 
	//		MPI_COMM_WORLD);

	//MPI_Bcast(&in.G_v[0], in.num_cities*in.num_cities, MPI_INT, 0, MPI_COMM_WORLD);
	
	//MPI_Bcast(&g_stack[0], g_stack.size(), MPI_tour, 0, MPI_COMM_WORLD);
	
	//l_stack.resize();

	MPI_Scatterv(&g_stack[0],
				local_sizes,
				local_offsets, 
				MPI_tour, 
				&l_stack[0],
				local_sizes[myrank],
				MPI_tour,
				0,
				MPI_COMM_WORLD);



	//v_l_stack.resize(NUM_THREADS);
	//for (int idx = 0; idx < NUM_THREADS; idx++)
	//{
	//	for (int i = 0; i < n_local_queue_size; i++)
	//	{
	//		v_l_stack[idx].push(g_stack.top());
	//		g_stack.pop();
	//	}
	//}

	//while (!g_stack.empty()) 
	//{
	//	v_l_stack[NUM_THREADS - 1].push(g_stack.top());
	//	g_stack.pop();
	//}

	//pthread_t tids[NUM_THREADS];
	//for (long i = 0; i < NUM_THREADS; i++) 
	//	pthread_create(&tids[i], NULL, compute, (void *)i);

	//for (long i = 0; i < NUM_THREADS; i++) 
	//	pthread_join(tids[i], NULL);

	////pthread_exit(NULL);
	//
	//cout << "best_tour = " << best_tour->cost << endl;
	//pthread_mutex_destroy(&best_tour_mutex);
	//pthread_mutex_destroy(&term_mutex);


	
}



int main(int argc, char** argv) 
{
	MPI_Init(&argc,&argv);

	BuildMPITourType();
	MPI_Type_commit(&MPI_tour);

	MPI_Comm_size(MPI_COMM_WORLD, &nTasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	tsp_mpi(argc, argv);
	//tsp_serial_v1();
	//tsp_serial_v2();

	MPI_Finalize();

	//MPI_Type_commit(&MPI_tour);

	cout << "Fin!" << endl;
	return 0;
}
























