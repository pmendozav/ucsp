
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

