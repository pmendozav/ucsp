#pragma once

#define NUM_THREADS 3

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <stack>
#include <queue>
#include <sstream>
#include <pthread.h>

using namespace std;
#define INF 1 << 30

struct Tour
{
	vector<int> visited;
	int cost;
	Tour(int _cost = 0) : cost(_cost) {}
};



#include "helper.h"

void AddCity(Tour *t, int city, int cost) 
{
	t->visited.push_back(city);
	t->cost += cost;
}
bool IsFeasible(Tour *t, vector<vector<int>> G, int from, int to) 
{
	for(int i=0; i < t->visited.size(); i++) 
		if (t->visited[i] == to) return false;

	if (G[from][to] == INF) return false;

	return true;
}
void RemoveLastCity(Tour *t,int city, const int &cost)
{
	//t->visited.erase(t->visited.begin() + t->visited.size() - 1);

	vector<int> asd(t->visited.begin(), t->visited.end() - 1);

	t->visited = asd;;
	t->cost -= cost;

}

void Initialize(int &num_cities, vector<vector<int>> &G)
{
	ifstream  
		f("in.in");
	stringstream 
		buf;
	char 
		s[100];
	int 
		n,
		from,
		to, 
		cost;
	char
		*pch;
	
	memset(s, '\0', 100);
	f.getline(s, 100);
	pch = strtok (s," ");
	n = atoi(pch);
	pch = strtok (NULL, " ");
	num_cities = atoi(pch);

	G.resize(num_cities);
	for (int i = 0; i < num_cities; i++)
		G[i].resize(num_cities);

	for (int i = 0; i < num_cities; i++)
		for (int j = 0; j < num_cities; j++)
			G[i][j] = INF;

	for(int i = 0; i < n; i++)
	{
		memset(s, '\n', 100);
		f.getline(s, 100);

		pch = strtok (s," ");
		from = atoi(pch);
		pch = strtok (NULL, " ");
		to = atoi(pch);
		pch = strtok (NULL, " ");
		cost = atoi(pch);

		G[from][to] = cost;
	}
}

void dfs_serial_v2(Tour *tour, Tour *best_tour, int num_cities, vector<vector<int>> G) 
{
	int city, cost, root_id, next_city;
	int idx, current_city;
	stack<Tour *> s;

	s.push(tour);
	while (!s.empty())
	{
		Tour *cur_tour = s.top();
		s.pop();
		if (cur_tour->visited.size() == num_cities)
		{
			cost = 0;
			idx = -1;
			current_city = cur_tour->visited[cur_tour->visited.size() - 1];
			
			if ((cost = G[current_city][0]) == INF) continue;

			if(cur_tour->cost + cost < best_tour->cost)
			{
				*best_tour = *cur_tour;
				//best_tour = Clone(cur_tour);
				best_tour->cost += cost;
				best_tour->visited.push_back(0);
			}
		}
		else {
			current_city = cur_tour->visited[cur_tour->visited.size() - 1];
			for (int next_city = 0; next_city < G.size(); next_city++) 
			{
				if(IsFeasible(cur_tour, G, current_city, next_city)) 
				{
					cost = G[current_city][next_city];
					AddCity(cur_tour, next_city, cost);
					
					Tour *new_tour = new Tour;
					*new_tour = *cur_tour;
					//new_tour = Clone(cur_tour);
					s.push(new_tour);
					RemoveLastCity(cur_tour, next_city, cost);
				}
			}
		}
	}
}

stack<Tour *> PartitionTree(vector<vector<int>> G, int level) {
	queue<pair<Tour*, int> > q;
	Tour* tour = new Tour;
	tour->cost = 0;
	tour->visited.push_back(0);

	q.push(make_pair(tour, 0));
	int times = 0;
	while (!q.empty()) {
		int q_size = q.size();
		pair<Tour*, int> ptour_t = q.front();
		Tour *cur_tour = ptour_t.first;
		Tour *tmp_tour = new Tour;

		tmp_tour = Clone(cur_tour);

		int depth = ptour_t.second;
		if (depth >= level) break;
		
		q.pop();
		
		int city = cur_tour->visited.at(cur_tour->visited.size() - 1);
		
		for (int nbr = 0; nbr < G[city].size(); nbr++) {
			int cur_city = nbr;
			int w = G[city][nbr];
			if(IsFeasible(cur_tour, G, city, cur_city)) {
				Tour* new_tour = new Tour;
				new_tour = Clone(cur_tour);

				AddCity(new_tour, cur_city, w);
				q.push(make_pair(new_tour, depth + 1));
			}
		}
	}

	stack<Tour *> qq;
	while (!q.empty()) {
		qq.push(q.front().first);
		//printVec(qq[qq.size() - 1]->visited);
		q.pop();
	}

	return qq;
}

//there aren't v1 because it's the same that the serial version
void dfs_omp_v2(stack<Tour *> &s, Tour *best_tour, int num_cities, vector<vector<int>> G) 
{
	int city, cost, root_id, next_city;
	int idx, current_city;

	while (!s.empty())
	{
		Tour *cur_tour = s.top();
		s.pop();
		if (cur_tour->visited.size() == num_cities)
		{
			cost = 0;
			idx = -1;
			current_city = cur_tour->visited[cur_tour->visited.size() - 1];
			
			if ((cost = G[current_city][0]) == INF) continue;

			if(cur_tour->cost + cost < best_tour->cost)
			{
				*best_tour = *cur_tour;
				//best_tour = Clone(cur_tour);
				best_tour->cost += cost;
				best_tour->visited.push_back(0);
			}
		}
		else {
			current_city = cur_tour->visited[cur_tour->visited.size() - 1];
			for (int next_city = 0; next_city < G.size(); next_city++) 
			{
				if(IsFeasible(cur_tour, G, current_city, next_city)) 
				{
					cost = G[current_city][next_city];
					AddCity(cur_tour, next_city, cost);
					
					Tour *new_tour = new Tour;
					*new_tour = *cur_tour;
					//new_tour = Clone(cur_tour);
					s.push(new_tour);
					RemoveLastCity(cur_tour, next_city, cost);
				}
			}
		}
	}
}

pthread_mutex_t best_tour_mutex, term_mutex, my_mutex;
Tour *best_tour;
vector<stack<Tour *>> l_tours_pthread;
bool webbing[NUM_THREADS];
int threads_in_cond_wait = 0;
stack<Tour *> new_stack;
pthread_cond_t term_cond_var = PTHREAD_COND_INITIALIZER; //??????

bool Terminate(stack<Tour *> &my_stack, int flag)
{
	int whereis = -1;
	//int my_stack_size = my_stack.size();

	if (my_stack.size() >= 2 && 
		threads_in_cond_wait > 0 &&
		new_stack.size() == 0)
	{
		pthread_mutex_trylock(&term_mutex);
		if (threads_in_cond_wait > 0 &&
			new_stack.size() == 0)
		{
			//split my_stack -> my_stack, new_stack
			for (int i = 0; i < my_stack.size() / 2; i++)
			{
				new_stack.push(my_stack.top());
				my_stack.pop();
			}
				
			pthread_cond_signal(&term_cond_var);
		}
		pthread_mutex_unlock(&term_mutex);
		return false;
	}
	else if (my_stack.size() != 0) //keep working
	{
		return false; //continue working
	}
	else //my_stack is empty
	{
		pthread_mutex_trylock(&term_mutex);
		if (threads_in_cond_wait == NUM_THREADS - 1)//last threag running
		{
			threads_in_cond_wait++;
			pthread_cond_broadcast(&term_cond_var);
			pthread_mutex_unlock(&term_mutex);
			return true; //quit
		}
		else //other threads still working, wait for a work
		{
			threads_in_cond_wait++;
			while (pthread_cond_wait(&term_cond_var, &term_mutex) != 0);
			//{
				if (threads_in_cond_wait < NUM_THREADS)//we got work
				{
					my_stack = new_stack;
					while (!new_stack.empty()){new_stack.pop();}
					threads_in_cond_wait--;
					pthread_mutex_unlock(&term_mutex);
					return false;
				}
				else //all threads done
				{
					pthread_mutex_unlock(&term_mutex);
					return true;
				}
			//}
		}
	}

	return false;
}

void dfs_pthreads(stack<Tour *> &my_stack, int num_cities, vector<vector<int>> G, int flag) 
{
	int city, cost, root_id, next_city;
	int idx, current_city;

	//echo(my_stack.size());
	while (!Terminate(my_stack, flag))
	{
		Tour *cur_tour = my_stack.top();
		my_stack.pop();
		
		if (cur_tour->visited.size() == num_cities)
		{
			cost = 0;
			idx = -1;
			current_city = cur_tour->visited[cur_tour->visited.size() - 1];
			
			if ((cost = G[current_city][0]) == INF) continue;

			pthread_mutex_lock(&best_tour_mutex);
			if(cur_tour->cost + cost < best_tour->cost)
			{
				*best_tour = *cur_tour; //best_tour = Clone(cur_tour);
				best_tour->cost += cost;
				best_tour->visited.push_back(0);
			}
			//cout << "qqq:" << best_tour->cost << endl;
			pthread_mutex_unlock(&best_tour_mutex);
		}
		else 
		{
			current_city = cur_tour->visited[cur_tour->visited.size() - 1];
			for (int next_city = 0; next_city < G.size(); next_city++) 
			{
				if(IsFeasible(cur_tour, G, current_city, next_city)) 
				{
					cost = G[current_city][next_city];
					AddCity(cur_tour, next_city, cost);
					
					Tour *new_tour = new Tour;
					*new_tour = *cur_tour; //new_tour = Clone(cur_tour);
					my_stack.push(new_tour);
					RemoveLastCity(cur_tour, next_city, cost);
				}
			}
		}
	}

	pthread_exit(NULL);

	cout << "best_tour = " << best_tour->cost << endl;
}

int num_cities;
vector<vector<int>> G;
vector<stack<Tour *>> v_l_stack;

void *compute(void *threadid)
{
	long taskid = (long)threadid;
	
	dfs_pthreads(v_l_stack[taskid], num_cities, G, taskid);

	return NULL;
}

void tsp_pthreads() 
{
	int n_local_queue_size, n_global_queue_size;
	int best_cost, cost;
	Tour *g_best_tour, *l_best_tour, *ref_tour;
	Tour *tour;
	
	stack<Tour *> g_stack;
	
	Initialize(num_cities, G);
	
	best_tour = new Tour;
	best_tour->cost = INF;
	
	l_best_tour = new Tour;
	l_best_tour->cost = INF;

	ref_tour = new Tour;
	ref_tour->cost = INF;

	//best_tour_mutex = new pthread_mutex_t;
	//term_mutex = new pthread_mutex_t;

	pthread_mutex_init(&best_tour_mutex, NULL);
	pthread_mutex_init(&term_mutex, NULL);
	pthread_mutex_init(&my_mutex, NULL);
	
		
	g_best_tour = new Tour[NUM_THREADS];
	for (int level = 0; level < num_cities; level++)
	{
		g_stack = PartitionTree(G, level);
		if (g_stack.size() >= NUM_THREADS)
			break;
	}

	n_local_queue_size = g_stack.size() / NUM_THREADS;

	v_l_stack.resize(NUM_THREADS);
	for (int id = 0; id < NUM_THREADS; id++)
	{
		for (int i = 0; i < n_local_queue_size; i++)
		{
			v_l_stack[id].push(g_stack.top());
			g_stack.pop();
		}
	}

	while (!g_stack.empty()) 
	{
		v_l_stack[NUM_THREADS - 1].push(g_stack.top());
		g_stack.pop();
	}

	pthread_t tids[NUM_THREADS];
	for (long i = 0; i < NUM_THREADS; i++) 
		pthread_create(&tids[i], NULL, compute, (void *)i);

	for (long i = 0; i < NUM_THREADS; i++) 
		pthread_join(tids[i], NULL);

	//pthread_exit(NULL);
	
	cout << "best_tour = " << best_tour->cost << endl;
	pthread_mutex_destroy(&best_tour_mutex);
	pthread_mutex_destroy(&term_mutex);
}



int main() 
{
	tsp_pthreads();
	//tsp_serial_v1();
	//tsp_serial_v2();

	cout << "Fin!" << endl;
	getchar();
	return 0;
}
























