#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <queue>
#include <sstream>

using namespace std;
#define INF 1 << 30

struct Tour
{
	vector<int> visited;
	int cost;
	Tour(int _cost = 0) : cost(_cost) {}
};

void AddCity(Tour *t, int city, int cost) 
{
	t->visited.push_back(city);
	t->cost += cost;
}
bool IsFeasible(Tour *t, vector<vector<int>> G, int from, int to) 
{
	for(int i=0;i < t->visited.size(); i++) 
		if (t->visited[i] == to) return false;

	if (G[from][to] == INF) return false;

	return true;
}
void RemoveLastCity(Tour *t,int city, const int &cost)
{
	t->visited.erase(t->visited.begin() + t->visited.size() - 1);
	t->cost -= cost;
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
					s.push(new_tour);
					RemoveLastCity(cur_tour, next_city, cost);
				}
			}
		}
	}
}

void Initialize(int &n, int &num_cities, vector<vector<int>> &G)
{
	ifstream  
		f("in.in");
	stringstream 
		buf;
	char 
		s[100];
	int 
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

	G.resize(n);
	for (int i = 0; i < n; i++)
		G[i].resize(n);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
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

//void Terminated(stack<int> mystack, 
//				int threads_in_cond_wait,
//				stack<int> &new_stack)
//{
//	if (mystack.size() >= 2 && threads_in_cond_wait > 0 && new_stack.size() == 0)
//	{
//		#pragma omp critical
//		{
//			if (threads_in_cond_wait > 0 && new_stack.size() == 0)
//			{
//				
//			}
//		}
//	
//	}
//}

queue<Tour *> PartitionTree(int nThreads, vector<vector<int>> G) {
	int cost;
	queue<Tour *> q;
	Tour *tour = new Tour;
	tour->cost = 0;
	tour->visited.push_back(0);

	q.push(tour);
	int times = 0;
	while (!q.empty()) {
		int q_size = q.size();
		Tour *cur_tour = q.front();
		q.pop();
		if (q_size >= nThreads) {
			break;
		}
		else {
			int current_city = cur_tour->visited[cur_tour->visited.size() - 1];
			for (int next_city = 0; next_city < G.size(); next_city++) {
				if (IsFeasible(cur_tour, G, current_city, next_city))
				{
					cost = G[current_city][next_city];
					AddCity(cur_tour, next_city, cost);

					Tour *new_tour = new Tour;
					memcpy(new_tour, cur_tour, sizeof(Tour));
					q.push(new_tour);
					RemoveLastCity(cur_tour, next_city, cost);
				}
			}
		}
	}
	return q;
}

void tsp_serial_v1()
{
	int n, num_cities;
	Tour *best_tour;
	Tour *tour;
	vector<vector<int>> G;
	
	Initialize(n, num_cities, G);

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
	int n, num_cities;
	Tour *best_tour;
	Tour *tour;
	vector<vector<int>> G;

	Initialize(n, num_cities, G);

	tour = new Tour;
	tour->cost = 0;
	tour->visited.push_back(0);

	best_tour = new Tour;
	best_tour->cost = INF;

	dfs_serial_v2(tour, best_tour, num_cities, G);

	cout << "Total Cost = " << best_tour->cost << endl;
}
void tsp_serial_v3() 
{




	int n, num_cities;
	Tour *best_tour;
	Tour *tour;
	vector<vector<int>> G;

	Initialize(n, num_cities, G);

	tour = new Tour;
	tour->cost = 0;
	tour->visited.push_back(0);

	best_tour = new Tour;
	best_tour->cost = INF;

	dfs_serial_v2(tour, best_tour, num_cities, G);

	cout << "Total Cost = " << best_tour->cost << endl;
}



int main() 
{
	//tsp_serial_v1();
	//tsp_serial_v2();

	cout << "Fin!" << endl;
	getchar();
	return 0;
}