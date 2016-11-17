#pragma once

#define echo(a) cout << #a << "=" << a << endl;
//#define chiste(mutex, flag, c) pthread_mutex_lock(&mutex); cout << flag << c << endl; pthread_mutex_unlock(&mutex);
#define chiste(mutex, flag, c) cout << flag << c << endl;;


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
		for (int j = 0; j < v[i]->visited.size(); j++)
			std::cout << v[i]->visited[j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl << "----------" << std::endl;
}
void mostrar_path (Tour* tour) {
  for (int i = 0; i < tour->visited.size(); i++) {
    cout << tour->visited[i] << " ";
  }
  cout << endl;
  cout << "Cost = " << tour->cost << endl;

}
Tour *Clone(Tour *in)
{
	Tour *out = new Tour;

	out->cost = in->cost;
	out->visited.resize(in->visited.size());

	for (int i = 0; i < in->visited.size(); i++)
		out->visited[i] = in->visited[i];

	return out;
}