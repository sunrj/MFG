#ifndef GRAPH
#define GRAPH

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <cmath>
#include <queue>
#include <iterator>
#include <bitset>
#include <math.h>
#include <unordered_map>
#include <stack>
#include <random>
#include <chrono>

#define TIMEOVER 43200

using namespace std;

typedef int I;
typedef int uI;
typedef int vI;
typedef int eI;

typedef vector<int> VI;
typedef queue<int> QI;
typedef vector<vector<int>> VVI;
typedef pair<uI, I> PII;
typedef pair<VI, VI> PVIVI;

typedef unordered_map<int, int> UMII;

static VI tdeg_u = {};
static VI tdeg_v = {};

VI operator+(VI &v1, VI &v2);
VI operator-(VI &v1, VI &v2);
VI operator*(VI &v1, VI &v2);

typedef struct Node
{
	VI adj;
	VI adjedge;
	VVI T_adj;
	VVI T_adjedge;
	I Deg;
	VI T_Deg;
	VI T_exist;
	bool exist;

	I Sup;
} Node;

typedef struct Edge1
{
	uI u;
	vI v;
	VI T;
	VI T_exist;
	bool exist;
} Edge1;

class Graph
{
public:
	double throwtime = 0;

	I cPu = 0;
	I cPv = 0;
	I cPe = 0;
	I cPPe = 0;

	double testtime = 0;

	I n_u, n_v, m, T, num_e = -1;

	I K_V, L_U, O;

	VI unit = {0};

	VI U, V;
	vector<Node> Node_U;
	vector<Node> Node_V;
	vector<Edge1> Edge;

	VI OT_V;
	VI OT_U;

	VVI OUT;
	I OUT_num = 0;


	VI U_in;
	I U_in_num = 0;

	VI V_in;
	I V_in_num = 0;


	void dataInput(string graphname);

	void Core(I t);

	void CRCore();

	void sub_CRCore(I give_du, I give_dv, I t, QI &give_del_u, QI &give_del_v);

	void Pruning_Size();

	void Update_Graph(string graphname);

	void Base(VI &U, VI &V, VI &C, VI &X, VI in_T, I now_t);

	void Update_T(vI v, VI &in_T, I &now_t);

	void Verification_new_new(VI &U, I V_size, VI &C, VI &in_T, I &now_t);

	VVI T_U;
};

#endif // !GRAPH
