//CLS based at intel Core i5 M430
#define CLS 2*32

#ifdef DEBUG
	#define N 4
	#define SM 2
#else
	#define N 1000
	#define SM (CLS / sizeof(double) < N ? CLS / sizeof(double) : N)
#endif // DEBUG

#define fori(count, a, b) for (count=a; count<b; count++)
#define foriStep(count, a, b, step) for (count=a; count<b; count+=step)
#define forit(L) for (auto it=L.begin(); it!=L.end(); ++it)

using namespace std;

inline void randMat(float **m)
{
	int i,j;

	fori(i, 0, N) {
		fori(j, 0, N) {
			m[i][j] = rand() % 5;
		}
	}
}
inline void randMat_v2(vector<vector<float>> &m)
{
	int i,j;

	fori(i, 0, N) {
		fori(j, 0, N) {
			m[i][j] = rand() % 5;
		}
	}
}
inline void plotMat(float **m)
{
	int i,j;

	fori(i, 0, N) {
		fori(j, 0, N) {
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
}
inline void plotMat_v2(vector<vector<float>> &m)
{
	int i,j;

	fori(i, 0, N) {
		fori(j, 0, N) {
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
}
inline void plotMat2(float *m)
{
	int i,j;
	fori(i, 0, N) {
		fori(j, 0, N) {
			cout << m[N*i + j] << " ";
		}
		cout << endl;
	}
}
inline void plotMat2_v2(vector<float> &m)
{
	int i,j;
	fori(i, 0, N) {
		fori(j, 0, N) {
			cout << m[N*i + j] << " ";
		}
		cout << endl;
	}
}
