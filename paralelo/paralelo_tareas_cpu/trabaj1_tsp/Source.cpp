#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <queue>
#include <sstream>
#include <omp.h>

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

void dfs_serial_v1(Tour *t, Tour *best_tour, int num_cities, vector<vector<int>> G) 
{
	int city, cost, root_id, current_city, next_city;

	if(t->visited.size() == num_cities) 
	{
		cost = 0;
		root_id = -1;
		current_city = t->visited[t->visited.size() - 1];
		
		if ((cost = G[current_city][0]) == INF) return;

		if(t->cost + cost < best_tour->cost)
		{
			*best_tour = *t;
			best_tour->cost += cost;
			best_tour->visited.push_back(0);
		}
	} 
	else 
	{
		current_city = t->visited[t->visited.size() - 1];
		for(int next_city = 0; next_city < G.size(); next_city++)
		{
			if(IsFeasible(t, G, current_city, next_city))
			{
				cost = G[current_city][next_city];
				AddCity(t, next_city, cost);
				dfs_serial_v1(t, best_tour, num_cities, G);
				RemoveLastCity(t,next_city, cost);
			}
		}
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

vector<Tour *> PartitionTree(vector<vector<int>> G, int level) {
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

	vector<Tour *> qq;
	while (!q.empty()) {
		qq.push_back(q.front().first);
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

void tsp_serial_v1()
{
	int num_cities;
	Tour *best_tour;
	Tour *tour;
	vector<vector<int>> G;
	
	Initialize(num_cities, G);

	tour = new Tour;
	best_tour = new Tour;
	best_tour->cost = 666;
	tour->cost = 0;
	tour->visited.push_back(0);

	dfs_serial_v1(tour, best_tour, num_cities, G);

	cout << "Total Cost = " << best_tour->cost << endl;
}
void tsp_serial_v2() 
{
	int num_cities;
	Tour *best_tour;
	Tour *tour;
	vector<vector<int>> G;

	Initialize(num_cities, G);

	tour = new Tour;
	tour->cost = 0;
	tour->visited.push_back(0);

	best_tour = new Tour;
	best_tour->cost = INF;

	dfs_serial_v2(tour, best_tour, num_cities, G);

	cout << "Total Cost = " << best_tour->cost << endl;
}
void tsp_omp_v1() 
{
	int nThreads, id;
	int num_cities;
	int n_local_queue_size, n_global_queue_size;
	int best_cost, cost;
	Tour *g_best_tour, *l_best_tour, *tmp_best_tour, *ref_tour;
	Tour *tour, *best_tour;
	vector<vector<int>> G;
	vector<Tour *> g_stack, l_stack;
	omp_lock_t my_lock;

	Initialize(num_cities, G);

	//printMat(G);
	//cout << "kkkkkkkkkkkkkk" << endl;
	//vector<Tour *> q = PartitionTree(G, 2);
	//for (int i = 0; i < q.size(); i++) {
	//	mostrar_path(q[i]);
	//}



	best_tour = new Tour;
	best_tour->cost = INF;
	
	l_best_tour = new Tour;
	l_best_tour->cost = INF;

	ref_tour = new Tour;
	ref_tour->cost = INF;

	tmp_best_tour = new Tour;
	tmp_best_tour->cost = INF;

	omp_set_num_threads(3);
	omp_init_lock(&my_lock);

	#pragma	omp parallel private(tour, l_stack, l_best_tour, tmp_best_tour, id)
	{
		Tour *l_best_tour2 = new Tour;
		l_best_tour2->cost = INF;

		id = omp_get_thread_num();

		#pragma omp master
		{
			nThreads = omp_get_num_threads();

			g_best_tour = new Tour[nThreads];
			for (int level = 0; level < num_cities; level++)
			{
				g_stack = PartitionTree(G, level);
				if (g_stack.size() >= nThreads)
					break;
			}

			//if (g_stack.size() < nThreads)
			//	omp_set_num_threads(g_stack.size());

			//cout << "Stack from Master" << endl;
			//for (int i = 0; i < g_stack.size(); i++) {
			//	mostrar_path(g_stack[i]);
			//}
			//cout << "------" << endl;
			//echo(g_stack.size());
			//pause("master");

		}
//		pause("\notro\n");
		#pragma omp barrier

		n_local_queue_size = g_stack.size() / nThreads;
		
		if (id != nThreads - 1)
			l_stack = vector<Tour *>(g_stack.begin() + id * n_local_queue_size, 
									 g_stack.begin() + (id + 1) * n_local_queue_size);
		else
			l_stack = vector<Tour *>(g_stack.begin() + id * n_local_queue_size, g_stack.end());

		//#pragma omp critical
		//{
		//	cout << "Local Stack id = " << id << endl;
		//	for (int i=0; i<l_stack.size(); i++)
		//	{
		//		for (int j=0; j<l_stack[i]->visited.size(); j++)
		//			cout << l_stack[i]->visited[j] << " ";
		//		cout << endl;
		//	}
		//	c
		//}

//		#pragma omp barrier

		for (int i = 0; i < l_stack.size(); i++)
		{
			tmp_best_tour = Clone(ref_tour);
			tour = l_stack[i];

			dfs_serial_v2(tour, tmp_best_tour, num_cities, G);
		
			if (tmp_best_tour->cost < l_best_tour2->cost)
				//memcpy(l_best_tour, tmp_best_tour, sizeof(Tour));
					l_best_tour2 = Clone(tmp_best_tour);
		}
		
		g_best_tour[id] = *l_best_tour2;

		//use critial inside a lock work too
		//#pragma omp critical
		{
			if (l_best_tour2->cost < best_tour->cost)
			{
				omp_set_lock(&my_lock);
				best_tour = Clone(l_best_tour2);
				omp_unset_lock(&my_lock);
			}
		}
	}

	cout << "Min cost tour" << best_tour->cost << endl;
}



int main() 
{
	tsp_omp_v1();
	//tsp_serial_v1();
	//tsp_serial_v2();

	cout << "Fin!" << endl;
	getchar();
	return 0;
}