#define MAXN 100
#define db(a) cout << #a << "=" << a << endl;
#define db2(a, b) cout << #a << "=" << a << " " << #b << " = " << b << endl;
#define ROOT_PATH 0
using namespace std;

#include <cstdlib>
#include <vector>

using namespace std;

struct Tour 
{
  vector<int> A; //A[MAXN];
  int city;
  int cost;
  Tour(int _city = 0,int _cost = 0) : city(_city), cost(_cost){ }
};

vector<pair<int,int> > G[MAXN];
int n, num_cities;
Tour *best_tour,*tour;

void AddCity(Tour *t, const int &city, const int &cost)
{
  t->A[t->city++] = city;
  t->cost += cost;
}
bool Best_tour(const Tour *t) 
{
  return (t->cost < best_tour->cost);
}
bool IsFeasible(Tour *t, int city) 
{
  for(int i = 0; i < t->city; i++)
    if (t->A[i] == city) return false;
  return true;
}
void RemoveLastCity(Tour *tour, int city, int w)
{
  tour->A[tour->city - 1] = 0;
  tour->cost -= w;
  tour->city--;
}
void update_best_tour(Tour *tour)
{
  memcpy(best_tour, tour, sizeof(Tour));
}
int city_count(Tour *tour)
{
  return tour->city;
}
void TSP(Tour *tour) 
{
	int w = 0, idx = -1, cur_city;
	int city;

	if(city_count(tour) == num_cities)
	{
		w = 0;
		idx = -1;
		cur_city = tour->A[tour->city - 1];
		for (int i = 0; i < G[cur_city].size(); i++)
		{
			if (G[cur_city][i].first == ROOT_PATH)
			{
				idx = i; 
				break;
			}
		}

		w = G[cur_city][idx].second;
		
		AddCity(tour, ROOT_PATH, w);
		
		if(Best_tour(tour))
			update_best_tour(tour);
		
		RemoveLastCity(tour, ROOT_PATH, w);
	} 
	else 
	{
		city = tour->A[tour->city - 1];
		for(int nbr=0; nbr < G[city].size(); nbr++)
		{
			cur_city = G[city][nbr].first;
			w = G[city][nbr].second;
			if(IsFeasible(tour, cur_city))
			{
				AddCity(tour, cur_city, w);
				TSP(tour);
				RemoveLastCity(tour,cur_city, w);
			}
		}
	}
}

int program2() {
  scanf("%d %d\n",&n, &num_cities);
  // inicializar nodos
  fill(G,G+n,vector<pair<int,int> >());
  int a,b,w;
  // leyendo el grafo
  for(int i=0;i<n;i++) {
    scanf("%d %d %d\n",&a,&b,&w);
    G[a].push_back(make_pair(b,w));
  }
  
  tour = new Tour;
  best_tour = new Tour;
  best_tour->cost = 1 << 30;
  tour->cost = 0;
  tour->A[0] = ROOT_PATH;
  tour->city = 1;
  
  TSP(tour);
  printf("best_tour->cost %d\n", best_tour->cost);
  
  return 0;
}