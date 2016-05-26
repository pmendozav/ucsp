#pragma once

#define G 6.673e-11 //N m2/kg2

typedef double T;

struct Vector2d
{
	T x, y;
	Vector2d() : x(.0f), y(.0f) {};
	Vector2d(T _x, T _y) : x(_x), y(_y) {};

	T norm()
	{
		return sqrt(this->x * this->x + this->y * this->y);
	}

	T norm2()
	{
		return this->x * this->x + this->y * this->y;
	}

	Vector2d operator+(const Vector2d &p2)
	{
		Vector2d out;
		out.x = this->x + p2.x;
		out.y = this->y + p2.y;
		return out;
	}

	Vector2d operator-(const Vector2d &p2)
	{
		Vector2d out;
		out.x = this->x - p2.x;
		out.y = this->y - p2.y;
		return out;
	}

	Vector2d operator*(const double &val)
	{
		Vector2d out;
		out.x = this->x * val;
		out.y = this->y * val;
		return out;
	}

	Vector2d operator/(const double &val)
	{
		Vector2d out;
		out.x = this->x / val;
		out.y = this->y / val;
		return out;
	}

	friend ostream& operator<<(ostream& os, const Vector2d& dt);
};

ostream& operator<<(ostream& os, const Vector2d& p)
{
    os << "[" << p.x << " " << p.y << "]";
    return os;
}

struct MPI_BodiesInfo
{
	vector<T> position; // 2*N
	vector<T> velocity; // 2*N
	vector<T> mass; // N
};

struct BodiesInfo
{
	vector<Vector2d> position;
	vector<Vector2d> velocity;
	vector<T> mass;
};

struct MPI_Input
{
	int N; // N-bodies
	T dt;
	T T;
	MPI_BodiesInfo mpi_bodiesInfo;
	bool isdetailed;
};

struct Input
{
	int N; // N-bodies
	T dt;
	T T;
	BodiesInfo bodiesInfo;
	bool isdetailed;
};



Input readInput(int argc, char** argv)
{
	Input in;

	if (argc < 4)
	{
		cerr << "Use: chiste.exe N dt T X" << endl;
		cerr << "- chiste.exe: program name" << endl;
		cerr << "- N: number of bodies to simulate" << endl;
		cerr << "- dt: time step" << endl;
		cerr << "- T: time limit" << endl;
		cerr << "- X: 1 (show step-by-step), 0 (only last values). Default is 0" << endl;
		exit(-1);
	}

	in.N = atoi(argv[1]);
	in.dt = atof(argv[2]);
	in.T = atof(argv[3]);
	in.bodiesInfo.mass.resize(in.N);
	in.bodiesInfo.position.resize(in.N);
	in.bodiesInfo.velocity.resize(in.N);
	
	if (argc < 5)
		in.isdetailed = false;
	else
		in.isdetailed = atoi(argv[4]);

	for (int i = 0; i < in.N; i++)
	{
		in.bodiesInfo.mass[i] = 100 + rand() % 200;
		in.bodiesInfo.position[i] = Vector2d(rand() % 10000, rand() % 10000);
		in.bodiesInfo.velocity[i] = Vector2d(rand() % 100, rand() % 100);
	}

	return in;
}

MPI_Input read_MPI_Input(int argc, char** argv)
{
	MPI_Input in;

	if (argc < 4)
	{
		cerr << "Use: chiste.exe N dt T X" << endl;
		cerr << "- chiste.exe: program name" << endl;
		cerr << "- N: number of bodies to simulate" << endl;
		cerr << "- dt: time step" << endl;
		cerr << "- T: time limit" << endl;
		cerr << "- X: 1 (show step-by-step), 0 (only last values). Default is 0" << endl;
		exit(-1);
	}

	in.N = atoi(argv[1]);
	in.dt = atof(argv[2]);
	in.T = atof(argv[3]);
	in.mpi_bodiesInfo.mass.resize(in.N);
	in.mpi_bodiesInfo.position.resize(2 * in.N);
	in.mpi_bodiesInfo.velocity.resize(2 * in.N);
	
	if (argc < 5)
		in.isdetailed = false;
	else
		in.isdetailed = atoi(argv[4]);

	for (int i = 0; i < in.N; i++)
	{
		in.mpi_bodiesInfo.mass[i] = 100 + rand() % 200;

		in.mpi_bodiesInfo.position[2 * i] = rand() % 10000;
		in.mpi_bodiesInfo.position[2 * i + 1] = rand() % 10000;

		in.mpi_bodiesInfo.velocity[2 * i] = rand() % 100;
		in.mpi_bodiesInfo.velocity[2 * i + 1] = rand() % 100;
	}

	return in;
}

Input readInput(int N)
{
	Input in;

	in.N = N;
	in.dt = 0.01;
	in.T = 100;
	in.bodiesInfo.mass.resize(N);
	in.bodiesInfo.position.resize(N);
	in.bodiesInfo.velocity.resize(N);
	in.isdetailed = true;

	for (int i = 0; i < N; i++)
	{
		in.bodiesInfo.mass[i] = 100 + rand() % 200;
		in.bodiesInfo.position[i] = Vector2d(rand() % 10000, rand() % 10000);
		in.bodiesInfo.velocity[i] = Vector2d(rand() % 100, rand() % 100);
	}

	return in;
}
void printDetailed(vector<Vector2d> force, vector<Vector2d> position, vector<Vector2d> velocity){}
void printWithoutDetail(vector<Vector2d> force, vector<Vector2d> position, vector<Vector2d> velocity){}
void print(vector<Vector2d> position, vector<Vector2d> velocity, vector<Vector2d> force, vector<T> mass)
{
	int
		N;

	N = position.size();
	for (int i = 0; i < N; i++)
	{
		cout << "----Body " << (i + 1) << ":" << endl;
		cout << "mass: " << mass[i] << endl;
		cout << "position: " << position[i] << endl;
		cout << "velocity: " << velocity[i] << endl;
		cout << "force: " << force[i] << endl;
	}
}




