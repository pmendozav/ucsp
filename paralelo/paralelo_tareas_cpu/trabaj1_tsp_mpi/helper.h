#pragma once

#define echo(a) cout << #a << "=" << a << endl;
//#define chiste(mutex, flag, c) pthread_mutex_lock(&mutex); cout << flag << c << endl; pthread_mutex_unlock(&mutex);
#define chiste(mutex, flag, c) cout << flag << c << endl;;


struct Tour
{
	int visited[MAX_SIZE];
	int num_cities;
	int cost;
	Tour(int _cost = 0) : cost(_cost), num_cities(0) 
	{
		memset(visited, -1, MAX_SIZE*sizeof(int));
	}
};

struct Tour2
{
	int num_cities;
	int cost;
	int visited[MAX_SIZE];
};

Tour2 *CreateTour2(int cost = INF)
{
	Tour2 *t = new Tour2;
	t->num_cities = 0;
	t->cost = cost;
	memset(t->visited, -1, MAX_SIZE*sizeof(int));

	return t;
}

struct Input_MPI
{
	int *G_v;
	int **G_m;
	int num_cities;
};


void pause(std::string msg = "")
{
	std::cout << msg.c_str() << std::endl;
	getchar();
}

void printMat(const vector<vector<int>> &m, std::string s = "Mat")
{
	std::cout << s.c_str() << ":" << std::endl;

	for (int i = 0; i < m.size(); i++)
	{
		for (int j = 0; j < m[0].size(); j++)
		{
			std::cout << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "----------" << std::endl;
}

void printVec(const vector<int> &v, std::string s = "Vec")
{
	std::cout << s.c_str() << ":" << std::endl;

	for (int i = 0; i < v.size(); i++)
	{	
		std::cout << v[i] << " ";
	}
	std::cout << std::endl << "----------" << std::endl;
}

void printVec(const vector<Tour *> &v, std::string s = "Vec")
{
	std::cout << s.c_str() << ":" << std::endl;

	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v[i]->num_cities; j++)
			std::cout << v[i]->visited[j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl << "----------" << std::endl;
}
void mostrar_path (Tour* tour) {
	for (int i = 0; i < tour->num_cities; i++) {
    cout << tour->visited[i] << " ";
  }
  cout << endl;
  cout << "Cost = " << tour->cost << endl;

}
Tour *Clone(Tour *in)
{
	Tour *out = new Tour;

	out->cost = in->cost;
	//out->visited.resize(in->visited.size());

	for (int i = 0; i < in->num_cities; i++)
		out->visited[i] = in->visited[i];

	return out;
}
Tour2 *Clone(Tour2 *in)
{
	Tour2 *out = CreateTour2();

	out->cost = in->cost;
	//out->visited.resize(in->visited.size());

	for (int i = 0; i < in->num_cities; i++)
		out->visited[i] = in->visited[i];

	return out;
}

void AddCity(Tour *t, int city, int cost) 
{
	t->visited[t->num_cities++] = city;
	t->cost += cost;
}
void AddCity(Tour2 &t, int city, int cost) 
{
	t.visited[t.num_cities++] = city;
	t.cost += cost;
}
void AddCity(Tour2 *t, int city, int cost) 
{
	t->visited[t->num_cities++] = city;
	t->cost += cost;
}
bool IsFeasible(Tour2 &t, int **G, int from, int to) 
{
	for(int i=0; i < t.num_cities; i++) 
		if (t.visited[i] == to) return false;

	if (G[from][to] == INF) return false;

	return true;
}
bool IsFeasible(Tour2 *t, int **G, int from, int to) 
{
	for(int i=0; i < t->num_cities; i++) 
		if (t->visited[i] == to) return false;

	if (G[from][to] == INF) return false;

	return true;
}
void RemoveLastCity(Tour *t,int city, const int &cost)
{
	//t->visited.erase(t->visited.begin() + t->visited.size() - 1);

	//vector<int> asd(t->visited.begin(), t->visited.end() - 1);

	t->visited[t->num_cities-1] = -1;
	t->num_cities--;
}
void RemoveLastCity(Tour2 &t,int city, const int &cost)
{
	//t->visited.erase(t->visited.begin() + t->visited.size() - 1);

	//vector<int> asd(t->visited.begin(), t->visited.end() - 1);

	t.visited[t.num_cities-1] = -1;
	t.num_cities--;
}
void RemoveLastCity(Tour2 *t,int city, const int &cost)
{
	//t->visited.erase(t->visited.begin() + t->visited.size() - 1);

	//vector<int> asd(t->visited.begin(), t->visited.end() - 1);

	t->visited[t->num_cities-1] = -1;
	t->num_cities--;
}

void Initialize(Input_MPI &in)
{
	//int num_cities, vector<vector<int>> G;

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
	in.num_cities = atoi(pch);

	in.G_v = new int[in.num_cities*in.num_cities];
	in.G_m = new int*[in.num_cities];
	
	memset(in.G_v, INF, in.num_cities*in.num_cities*sizeof(int));
	
	for (int i = 0; i < in.num_cities; i++)
	{
		in.G_m[i] = new int[in.num_cities];
		memset(in.G_m[i], INF, in.num_cities*sizeof(int));
	}

	//for (int i = 0; i < in.num_cities; i++)
	//	for (int j = 0; j < in.num_cities; j++)
	//		in.G[i][j] = INF;

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

		in.G_v[from*in.num_cities + to] = cost;
		in.G_m[from][to] = cost;
	}
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

vector<Tour2> PartitionTree(int **G, int num_cities, int level) {
	queue<pair<Tour2*, int> > q;
	Tour2* tour = CreateTour2();
	tour->cost = 0;
	tour->visited[tour->num_cities++] = 0;

	q.push(make_pair(tour, 0));
	int times = 0;
	while (!q.empty()) {
		int q_size = q.size();
		pair<Tour2*, int> ptour_t = q.front();
		Tour2 *cur_tour = ptour_t.first;
		Tour2 *tmp_tour = CreateTour2();

		tmp_tour = Clone(cur_tour);

		int depth = ptour_t.second;
		if (depth >= level) break;
		
		q.pop();
		
		int city = cur_tour->visited[tour->num_cities - 1];
		
		for (int nbr = 0; nbr < num_cities; nbr++) {
			int cur_city = nbr;
			int w = G[city][nbr];
			if(IsFeasible(cur_tour, G, city, cur_city)) {
				Tour2* new_tour = CreateTour2();
				new_tour = Clone(cur_tour);

				AddCity(new_tour, cur_city, w);
				q.push(make_pair(new_tour, depth + 1));
			}
		}
	}

	vector<Tour2> qq;
	while (!q.empty()) {
		qq.push_back(*(q.front().first));
		//printVec(qq[qq.size() - 1]->visited);
		q.pop();
	}

	return qq;
}


void dfs_serial_v2(Tour2 *tour, Tour2 *best_tour, int num_cities, int **G) 
{
	int city, cost, root_id, next_city;
	int idx, current_city;
	stack<Tour2 *> s;

	s.push(tour);
	while (!s.empty())
	{
		Tour2 *cur_tour = s.top();
		s.pop();
		if (cur_tour->num_cities == num_cities)
		{
			cost = 0;
			idx = -1;
			current_city = cur_tour->visited[cur_tour->num_cities - 1];
			
			if ((cost = G[current_city][0]) == INF) continue;

			if(cur_tour->cost + cost < best_tour->cost)
			{
				*best_tour = *cur_tour;
				//best_tour = Clone(cur_tour);
				best_tour->cost += cost;
				best_tour->visited[best_tour->num_cities++] = 0;
			}
		}
		else {
			current_city = cur_tour->visited[cur_tour->num_cities - 1];
			for (int next_city = 0; next_city < num_cities; next_city++) 
			{
				if(IsFeasible(cur_tour, G, current_city, next_city)) 
				{
					cost = G[current_city][next_city];
					AddCity(cur_tour, next_city, cost);
					
					Tour2 *new_tour = CreateTour2();
					*new_tour = *cur_tour;
					//new_tour = Clone(cur_tour);
					s.push(new_tour);
					RemoveLastCity(cur_tour, next_city, cost);
				}
			}
		}
	}
}
