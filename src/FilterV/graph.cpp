#include "graph.h"

VI operator+(VI &v1, VI &v2)
{
	VI res(v1.size() + v2.size());
	int sz = set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin()) - res.begin();
	res.resize(sz);
	return res;
};

VI operator-(VI &v1, VI &v2)
{
	VI res(v1.size());
	int sz = set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin()) - res.begin();
	res.resize(sz);
	return res;
};

VI operator*(VI &v1, VI &v2)
{
	VI res(v1.size());
	int sz = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin()) - res.begin();
	res.resize(sz);
	return res;
};

void Graph::dataInput(string graphname)
{
	ifstream infile;
	infile.open(graphname);
	infile >> n_u >> n_v >> m >> T;
	// cout << "n_u : " << n_u << "  n_v : " << n_v << "  m : " << m << "  T : " << T << endl;

	Node_U.reserve(n_u);
	U.reserve(n_u);
	Node_V.reserve(n_v);
	V.reserve(n_v);

	VI ne = {};
	for (I t = 0; t < T; t++)
	{
		T_U.push_back(ne);
	}

	for (I i = 0; i < n_u; i++)
	{
		U.push_back(i);

		Node n;
		n.Deg = 0;
		n.exist = 1;
		n.Sup = 0;

		VI emp = {};
		for (auto j = 0; j < T; j++)
			n.T_adj.push_back(emp);

		for (auto j = 0; j < T; j++)
			n.T_adjedge.push_back(emp);

		for (auto j = 0; j < T; j++)
			n.T_Deg.push_back(0);

		for (auto j = 0; j < T; j++)
			n.T_exist.push_back(0);

		Node_U.push_back(n);

		OT_U.push_back(0);


		U_in.push_back(0);
	}

	for (I i = 0; i < n_v; i++)
	{
		V.push_back(i);

		Node n;
		n.Deg = 0;
		n.exist = 1;
		n.Sup = 0;

		VI emp = {};
		for (auto j = 0; j < T; j++)
			n.T_adj.push_back(emp);

		for (auto j = 0; j < T; j++)
			n.T_adjedge.push_back(emp);

		for (auto j = 0; j < T; j++)
			n.T_Deg.push_back(0);

		for (auto j = 0; j < T; j++)
			n.T_exist.push_back(0);

		Node_V.push_back(n);

		OT_V.push_back(0);


		V_in.push_back(0);

		tdeg_v.push_back(0);
	}

	I u, v, t;
	I pu = -1, pv = -1;
	for (I j = 0; j < m; j++)
	{
		infile >> u >> v >> t;

		if (u == pu && v == pv)
		{
			Edge[num_e].T.push_back(t);
			Edge[num_e].T_exist[t] = 1;

			Node_U[u].T_adj[t].push_back(v);
			Node_U[u].T_adjedge[t].push_back(num_e);
			Node_U[u].T_Deg[t]++;
			Node_U[u].T_exist[t] = 1;

			Node_V[v].T_adj[t].push_back(u);
			Node_V[v].T_adjedge[t].push_back(num_e);
			Node_V[v].T_Deg[t]++;
			Node_V[v].T_exist[t] = 1;
		}
		else
		{
			num_e++;


			Edge1 e;
			e.u = u;
			e.v = v;
			e.T.push_back(t);
			for (auto k = 0; k < T; k++)
				e.T_exist.push_back(0);
			e.T_exist[t] = 1;
			e.exist = 1;
			Edge.push_back(e);


			Node_U[u].adj.push_back(v);
			Node_U[u].adjedge.push_back(num_e);
			Node_U[u].Deg++;
			Node_U[u].T_adj[t].push_back(v);
			Node_U[u].T_adjedge[t].push_back(num_e);
			Node_U[u].T_Deg[t]++;
			Node_U[u].T_exist[t] = 1;


			Node_V[v].adj.push_back(u);
			Node_V[v].adjedge.push_back(num_e);
			Node_V[v].Deg++;
			Node_V[v].T_adj[t].push_back(u);
			Node_V[v].T_adjedge[t].push_back(num_e);
			Node_V[v].T_Deg[t]++;
			Node_V[v].T_exist[t] = 1;


			pu = u;
			pv = v;
		}
	}
	num_e++;

	infile.close();
}


void Graph::Core(I t)
{
	QI del_u, del_v;

	for (I i = 0; i < n_u; i++)
	{
		if (Node_U[i].T_Deg[t] < K_V && Node_U[i].T_exist[t])
		{
			del_u.push(i);
			Node_U[i].T_exist[t] = 0;
		}
	}

	for (I i = 0; i < n_v; i++)
	{
		if (Node_V[i].T_Deg[t] < L_U && Node_V[i].T_exist[t])
		{
			del_v.push(i);
			Node_V[i].T_exist[t] = 0;
		}
	}

	uI du;
	vI dv;
	while (!del_u.empty() || !del_v.empty())
	{
		while (!del_u.empty())
		{
			du = del_u.front();
			del_u.pop();
			for (I i = 0; i < Node_U[du].T_adj[t].size(); i++)
			{
				vI nv = Node_U[du].T_adj[t][i];
				if (Edge[Node_U[du].T_adjedge[t][i]].T_exist[t])
				{
					Edge[Node_U[du].T_adjedge[t][i]].T_exist[t] = 0;
					cPe++;
				}
				if (--Node_V[nv].T_Deg[t] < L_U && Node_V[nv].T_exist[t])
				{
					del_v.push(nv);
					Node_V[nv].T_exist[t] = 0;
				}
			}
		}

		while (!del_v.empty())
		{
			dv = del_v.front();
			del_v.pop();
			for (I i = 0; i < Node_V[dv].T_adj[t].size(); i++)
			{
				uI nu = Node_V[dv].T_adj[t][i];
				if (Edge[Node_V[dv].T_adjedge[t][i]].T_exist[t])
				{
					Edge[Node_V[dv].T_adjedge[t][i]].T_exist[t] = 0;
					cPe++;
				}
				if (--Node_U[nu].T_Deg[t] < K_V && Node_U[nu].T_exist[t])
				{
					del_u.push(nu);
					Node_U[nu].T_exist[t] = 0;
				}
			}
		}
	}
}

void Graph::CRCore()
{
	QI del_u, del_v;

	for (I u = 0; u < n_u; u++)
	{
		if (!Node_U[u].exist)
			continue;
		for (I t = 0; t < T; t++)
			if (Node_U[u].T_exist[t] == 1)
				Node_U[u].Sup++;
		if (Node_U[u].Sup == 0)
		{
			Node_U[u].exist = 0;
			del_u.push(u);
		}
	}


	for (I v = 0; v < n_v; v++)
	{
		if (!Node_V[v].exist)
			continue;
		for (I t = 0; t < T; t++)
			if (Node_V[v].T_exist[t] == 1)
				Node_V[v].Sup++;
		if (Node_V[v].Sup < O)
		{
			Node_V[v].exist = 0;
			del_v.push(v);
		}
	}


	uI du;
	vI dv;
	while (!del_u.empty() || !del_v.empty())
	{
		while (!del_u.empty())
		{
			du = del_u.front();
			del_u.pop();
			for (I t = 0; t < T; t++)
			{
				if (Node_U[du].T_exist[t])
				{
					Node_U[du].T_exist[t] = 0;
					sub_CRCore(du, -1, t, del_u, del_v);
				}
			}
		}

		while (!del_v.empty())
		{
			dv = del_v.front();
			del_v.pop();
			for (I t = 0; t < T; t++)
			{
				if (Node_V[dv].T_exist[t])
				{
					Node_V[dv].T_exist[t] = 0;
					sub_CRCore(-1, dv, t, del_u, del_v);
				}
			}
		}
	}
}

void Graph::sub_CRCore(I give_du, I give_dv, I t, QI &give_del_u, QI &give_del_v)
{
	QI del_u, del_v;
	if (give_du != -1)
		del_u.push(give_du);
	if (give_dv != -1)
		del_v.push(give_dv);

	uI du;
	vI dv;
	while (!del_u.empty() || !del_v.empty())
	{
		while (!del_u.empty())
		{
			du = del_u.front();
			del_u.pop();
			for (I i = 0; i < Node_U[du].T_adj[t].size(); i++)
			{
				vI nv = Node_U[du].T_adj[t][i];
				if (Edge[Node_U[du].T_adjedge[t][i]].T_exist[t])
				{
					Edge[Node_U[du].T_adjedge[t][i]].T_exist[t] = 0;
					cPe++;
				}
				if (--Node_V[nv].T_Deg[t] < L_U && Node_V[nv].T_exist[t])
				{
					del_v.push(nv);
					Node_V[nv].T_exist[t] = 0;
					if (--Node_V[nv].Sup < O && Node_V[nv].exist)
					{
						give_del_v.push(nv);
						Node_V[nv].exist = 0;
					}
				}
			}
		}

		while (!del_v.empty())
		{
			dv = del_v.front();
			del_v.pop();
			for (I i = 0; i < Node_V[dv].T_adj[t].size(); i++)
			{
				uI nu = Node_V[dv].T_adj[t][i];
				if (Edge[Node_V[dv].T_adjedge[t][i]].T_exist[t])
				{
					Edge[Node_V[dv].T_adjedge[t][i]].T_exist[t] = 0;
					cPe++;
				}
				if (--Node_U[nu].T_Deg[t] < K_V && Node_U[nu].T_exist[t])
				{
					del_u.push(nu);
					Node_U[nu].T_exist[t] = 0;
					if (--Node_U[nu].Sup == 0 && Node_U[nu].exist)
					{
						give_del_u.push(nu);
						Node_U[nu].exist = 0;
					}
				}
			}
		}
	}
}

void Graph::Pruning_Size()
{
	cPu = 0;
	cPv = 0;
	cPPe = 0;
	VI pU = {};
	VI pV = {};
	for (auto u = 0; u < n_u; u++)
	{
		I flag = 1;
		for (auto t = 0; t < T; t++)
		{
			if (Node_U[u].T_exist[t])
			{
				flag = 0;
				break;
			}
		}
		if (flag || Node_U[u].exist == 0)
		{
			Node_U[u].exist = 0;
			cPu++;
			pU.push_back(u);

			for (auto t = 0; t < T; t++)
			{
				Node_U[u].T_exist[t] = 0;
			}
		}
	}
	U = U - pU;

	for (auto v = 0; v < n_v; v++)
	{
		I flag = 1;
		for (auto t = 0; t < T; t++)
		{
			if (Node_V[v].T_exist[t])
			{
				flag = 0;
				break;
			}
		}
		if (flag || Node_V[v].exist == 0)
		{
			Node_V[v].exist = 0;
			cPv++;
			pV.push_back(v);

			for (auto t = 0; t < T; t++)
			{
				Node_V[v].T_exist[t] = 0;
			}
		}
	}
	V = V - pV;

	for (auto e = 0; e < num_e; e++)
	{
		I flag = 1;
		for (auto t = 0; t < T; t++)
		{
			if (Edge[e].T_exist[t])
			{
				flag = 0;
				break;
			}
		}
		if (flag || Edge[e].exist == 0)
		{
			Edge[e].exist = 0;
			cPPe++;

			for (auto t = 0; t < T; t++)
			{
				Edge[e].T_exist[t] = 0;
			}
		}
	}
}

void Graph::Update_Graph(string graphname)
{
	for (I i = 0; i < n_u; i++)
	{
		Node_U[i].adj = {};
		Node_U[i].Deg = 0;
		Node_U[i].adjedge = {};
		for (auto j = 0; j < T; j++)
			Node_U[i].T_adj[j] = {};
		for (auto j = 0; j < T; j++)
			Node_U[i].T_adjedge[j] = {};
		for (auto j = 0; j < T; j++)
			Node_U[i].T_Deg[j] = 0;
	}

	for (I i = 0; i < n_v; i++)
	{
		Node_V[i].adj = {};
		Node_V[i].Deg = 0;
		Node_V[i].adjedge = {};
		for (auto j = 0; j < T; j++)
			Node_V[i].T_adj[j] = {};
		for (auto j = 0; j < T; j++)
			Node_V[i].T_adjedge[j] = {};
		for (auto j = 0; j < T; j++)
			Node_V[i].T_Deg[j] = 0;
	}





	ifstream infile;
	infile.open(graphname);
	infile >> n_u >> n_v >> m >> T;

	I u, v, t;
	num_e = -1;
	I pu = -1, pv = -1;
	for (I j = 0; j < m; j++)
	{
		infile >> u >> v >> t;

		if (!Node_U[u].T_exist[t] || !Node_V[v].T_exist[t] || !Node_U[u].exist || !Node_V[v].exist)
		{
			if (u != pu || v != pv)
			{
				num_e++;
				pu = u;
				pv = v;
			}

			continue;
		}


		if (u == pu && v == pv)
		{
			if (Node_U[u].adj.size() == 0 || Node_U[u].adj.back() != v)
			{
				Node_U[u].adj.push_back(v);
				Node_U[u].adjedge.push_back(num_e);
				Node_U[u].Deg++;
			}
			Node_U[u].T_adj[t].push_back(v);
			Node_U[u].T_adjedge[t].push_back(num_e);
			Node_U[u].T_Deg[t]++;

			if (Node_V[v].adj.size() == 0 || Node_V[v].adj.back() != u)
			{
				Node_V[v].adj.push_back(u);
				Node_V[v].adjedge.push_back(num_e);
				Node_V[v].Deg++;
			}
			Node_V[v].T_adj[t].push_back(u);
			Node_V[v].T_adjedge[t].push_back(num_e);
			Node_V[v].T_Deg[t]++;
		}
		else
		{
			num_e++;

			if (Node_U[u].adj.size() == 0 || Node_U[u].adj.back() != v)
			{
				Node_U[u].adj.push_back(v);
				Node_U[u].adjedge.push_back(num_e);
				Node_U[u].Deg++;
			}
			Node_U[u].T_adj[t].push_back(v);
			Node_U[u].T_adjedge[t].push_back(num_e);
			Node_U[u].T_Deg[t]++;


			if (Node_V[v].adj.size() == 0 || Node_V[v].adj.back() != u)
			{
				Node_V[v].adj.push_back(u);
				Node_V[v].adjedge.push_back(num_e);
				Node_V[v].Deg++;
			}
			Node_V[v].T_adj[t].push_back(u);
			Node_V[v].T_adjedge[t].push_back(num_e);
			Node_V[v].T_Deg[t]++;


			pu = u;
			pv = v;
		}
	}
	num_e++;

	infile.close();
}




void Graph::Base(VI &U, VI &V, VI &C, VI &X, VI in_T, I now_t)
{
	if ((double)clock() / CLOCKS_PER_SEC - throwtime > TIMEOVER)
		throw string("TIMEOVER");

	
	if (U.size() < L_U || V.size() + C.size() < K_V)
		return;

	I flag = 0;
	VI doing(C);


	I flag_check = 1;


	for (auto v : doing)
	{
		unit[0] = v;
		C = C - unit;
		VI V_1(V + unit);
		VI U_1(U * Node_V[v].adj);

		U_in_num++;
		for(auto u : U_1)
		{
			U_in[u] = U_in_num;
		}



		VI new_in_T(in_T);
		I new_now_t = now_t;
		Update_T(v, new_in_T, new_now_t);

		VI r_C = {};
		r_C = Filter(C, new_in_T, new_now_t);
		VI C_1(C - r_C);


		VI r_X = {};
		r_X = Filter(X, new_in_T, new_now_t);
		VI X_1(X - r_X);


		V_in_num++;
		for (auto v : V_1)
		{
			V_in[v] = V_in_num;
		}

		Verification_new_new(U_1, V_1.size(), C_1, new_in_T, new_now_t);
		Verification_new_new(U_1, V_1.size(), X_1, new_in_T, new_now_t);


		if (C_1.size() == 0)
		{
			if (X_1.size() == 0)
			{ 
				if (V_1.size() >= K_V)
				{
					OUT.push_back(V_1);
					OUT_num++;
				}
			}
		}
		else
		{
			Base(U_1, V_1, C_1, X_1, new_in_T, new_now_t);
		}
		unit[0] = v;
		X = X + unit;
	}
}


void Graph::Update_T(vI v, VI &in_T, I &now_t)
{
	for (I t = 0; t < T; t++)
	{
		if (in_T[t] != now_t)
			continue;

		I count_D = 0;
		for (auto u : Node_V[v].T_adj[t])
		{
			if (U_in[u] == U_in_num)
				count_D++;
		}
		if (count_D >= L_U)
		{
			in_T[t]++;
		}
	}
	now_t++;
}


VI Graph::Filter(VI &C, VI &in_T, I &now_t)
{
	VI r_C = {};
	for (auto v : C)
	{
		I count_T = 0;

		for (I t = 0; t < T; t++)
		{
			if (in_T[t] != now_t)
				continue;
			I count_D = 0;
			for (auto u : Node_V[v].T_adj[t])
			{
				if (U_in[u] == U_in_num)
					count_D++;
			}
			if (count_D >= L_U)
			{
				++count_T;
			}
		}

		if (count_T < O)
			r_C.push_back(v);
	}
	return r_C;
}


void Graph::Verification_new_new(VI &U, I V_size, VI &C, VI &in_T, I &now_t)
{
	VI r_C = {};


	VI T_count(T);
	for (I t = 0; t < T; t++)
	{
		T_U[t].clear();
	}

	for (auto u : U)
	{
		for (I t = 0; t < T; t++)
		{
			if (in_T[t] != now_t)
				continue;

			I sum = 0;
			for (auto v : Node_U[u].T_adj[t])
			{
				if (V_in[v] == V_in_num)
					if (++sum == V_size)
					{
						T_count[t]++;
						T_U[t].push_back(u);
						break;
					}
			}
		}
	}

	for (auto v : C)
	{
		I sum = 0;

		for (I t = 0; t < T; t++)
		{
			if (T_count[t] < L_U)
				continue;

			if ((Node_V[v].T_adj[t] * T_U[t]).size() >= L_U)
			{
				sum++;
			}
		}

		if (sum < O)
			r_C.push_back(v);
	}

	C = C - r_C;
}








