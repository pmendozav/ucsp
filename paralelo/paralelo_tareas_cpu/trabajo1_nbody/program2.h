#pragma once

struct Tour
{
	int cost;
	int number_of_cities;
	vector<int> cities_visited;
};

typedef vector<int *> City;
typedef vector<City> Digraph;

struct InputTSP
{
	int N;
	Digraph digraph;
	Tour best_tour;
};

int CityCount(Tour tour)
{
	return tour.number_of_cities;
}

void UpadateBestTour(Tour in, Tour &out)
{
	out = in;
}

bool Feasible(Tour tour, int index)
{
	std::vector<int>::iterator it;

	it = std::find(tour.cities_visited.begin(), tour.cities_visited.end(), index);

	if (it != tour.cities_visited.end())
		return false;
	else
		return true;
}

void AddCity(Tour &tour, int index, int cost)
{
	tour.cost += cost;
	tour.number_of_cities++;
	tour.cities_visited.push_back(index);
}

void RemoveLastCity(Tour &tour, int index, int cost)
{
	std::vector<int>::iterator it;

	tour.cost -= cost;
	tour.number_of_cities--;

	it = std::find(tour.cities_visited.begin(), tour.cities_visited.end(), index);
	tour.cities_visited.erase(tour.cities_visited.begin() + *it);
}

//void dfs_recursive(Tour *tour, Tour *best_tour, Digraph digraph, City city)
void dfs_recursive(Tour *tour, Tour *best_tour, Digraph *digraph, City *city)
{
	if (CityCount(*tour) == digraph->size())
	{
		if (tour->cost < best_tour->cost)
			//*best_tour = *tour;
			memcpy(best_tour, tour, sizeof(Tour));
	}
	else
	{
		for (int i = 0; i < city->size(); i++)
		{
			if (Feasible(*tour, (*city)[i][0]))
			{
				AddCity(*tour, (*city)[i][0], (*city)[i][1]);
				dfs_recursive(tour, best_tour, digraph, &(*digraph)[(*city)[i][0]]);
				RemoveLastCity(*tour, (*city)[i][0], (*city)[i][1]);
			}
		}
	}
}

void dfs_serial_v1(InputTSP in)
{
	int 
		N;
	Tour
		tour,
		best_tour,
		current_tour;
	Digraph
		digraph;

	//initialize digraph
	int buf[12][3] = {
		{0, 1, 1  },
		{1, 0, 5  },
		{0, 3, 8  },
		{3, 0, 7  },
		{3, 2, 12 },
		{2, 3, 10 },
		{2, 1, 18 },
		{1, 2, 2  },
		{0, 2, 3  },
		{2, 0, 1  },
		{3, 1, 4  },
		{1, 3, 6  },
	};

	int buf2[2];
	//digraph = vector<City>();
	digraph.resize(4);
	for (int i=0; i < digraph.size(); i++)
	{
		buf2[0] = buf[i][1];
		buf2[1] = buf[i][2];
		digraph[buf[i][0]].push_back(buf2);
	}

	//initialize tour and best_tour
	best_tour.cost = 1 << 30;
	best_tour.number_of_cities = 1;

	//tour.cost = 0;
	tour.number_of_cities = 1;
	tour.cities_visited.push_back(0);

	dfs_recursive(&tour, &best_tour, &digraph, &(digraph[0]));

	for (int i=0; i<best_tour.cities_visited.size(); i++)
		cout << best_tour.cities_visited[i] << " ";

}

