#ifndef RECURRENCEGROUP_H_
#define RECURRENCEGROUP_H_

#include <string>
#include <vector>
#include <cstring>
#include <climits>
#include <algorithm>
#include <utility>
#include <cstdio>
#include <cmath>
#include <set>
#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>

#define MAXST 1024
#define D(x) ((x)<nu?tv:tu)

using namespace std;


class RecurrenceGroup {
public:
	RecurrenceGroup(string path, int time_scale, int tu, int tv, int lmd, bool group_v = true);	
	int cPu = 0;
	int cPv = 0;
	int cPe = 0;
	int pre_nu, pre_nv, pre_m;
	int n_ans; //number of answers

private:
	int n, nu, nv, nt, m;
	int n_old, nu_old, nv_old, nt_old, m_old;
	int tu, tv, lmd;
	vector<vector<vector<int> > > g;
	vector<int> id_old;

private: //used for core_prune
	vector<vector<int> > deg;
	vector<int> sup;

private: //used for search
	vector<vector<int> > cnt;
	vector<int> cnt_t; //number of valid t on nodes 0 to nu-1
	vector<int> ans; //current answer on nodes 0 to nu-1
	vector<bool> in_ans; //size nu
	vector<vector<int> > cand; //candidates of 0 to nu-1
	vector<int> cand_v; //candidates of nodes nu tp n
	vector<int> cand_u; //tmp candidates of 0 to nu-1
	vector<vector<int> > cand_t; //candidates of 0 to nt
	vector<int> last; //size nu
	int nowt; //used for counting




private:
	int get_num_cnt(string path);
	bool get_temporal_edge(char *line, int &a, int &b, int &t, int num_cnt);
	void core_prune(int t, int x);
	void search(int p);

public:
	void core_prune();
	void reorder(int method = 1); // 0: keep original order  1: ascending degree
	void search();
};

RecurrenceGroup::RecurrenceGroup(string path, int time_scale, int tu, int tv, int lmd, bool group_v) {
	// cout << tu << " " << tv << " " << lmd << endl;
	// if(path[path.size()-1] != '/') path = path + "/";
	FILE *fin = fopen((path + ".txt").c_str(), "r" );
	char line[MAXST];
	int a, b, t, num_cnt = get_num_cnt(path), cnt = 0, mint = -1, maxt = 0;
	vector<pair<pair<int,int>,int> > el;

	if(group_v) swap(tu, tv);
	this->tu = tu;
	this->tv = tv;
	this->lmd = lmd;

	nu = 0; nv = 0;
	// printf("Loading text, path = %s, num_cnt = %d...\n", path.c_str(), num_cnt);
	while(fgets( line, MAXST, fin)) {
		if(!get_temporal_edge(line, a, b, t, num_cnt)) continue;
		if(group_v) swap(a,b);
		el.push_back(make_pair(make_pair(a, b),t));
		nu = max(nu, a+1);
		nv = max(nv, b+1);
		// if((++cnt) % 10000000 == 0) printf("%d lines finished\n", cnt);
		if(mint == -1 || t < mint) mint = t;
		maxt = max(t, maxt);
	}
	n = nu + nv;
	nt = (maxt-mint)/time_scale+1;
	g.resize(nt);
	for(int t = 0; t < nt; ++t) g[t].resize(nu+nv);

	for(int i = 0; i < (int) el.size(); ++i) {
		int a = el[i].first.first, b = el[i].first.second, t = el[i].second;
		t = (t-mint)/time_scale;
		b += nu;
		g[t][a].push_back(b);
		g[t][b].push_back(a);
	}

	m = 0;
	for(int t = 0; t < nt; ++t)
		for(int i = 0; i < n; ++i) {
			if(g[t][i].size() == 0) continue;
			sort(g[t][i].begin(), g[t][i].end());
			int p = 1;
			for(int j = 1; j < (int) g[t][i].size(); ++j)
				if(g[t][i][j-1] != g[t][i][j]) g[t][i][p++] = g[t][i][j];
			g[t][i].resize( p ); m += p;
		}

	id_old.resize(n,-1);
	for(int i = 0; i < n; ++i) id_old[i] = i;
	// printf( "Graph loaded, nu = %d, nv = %d, nt = %d, m = %d, cnt = %d\n", nu, nv, nt, m/2, cnt );
	pre_nu = nu;
	pre_nv = nv;
	pre_m = m/2;

	fclose(fin);
}

void RecurrenceGroup::search() {
	nowt = 0;
	cnt.resize(nt);
	for(int t = 0; t < nt; ++t) cnt[t].resize(n,0);

	cnt_t.resize(nu,0);

	ans.resize(nu);
	in_ans.resize(nu,false);

	cand.resize(nu+1);
	for(int x = 0; x < nu; ++x) cand[0].push_back(x);

	cand_t.resize(nu+1);
	for(int t = 0; t < nt; ++t) cand_t[0].push_back(t);

	last.resize(nu,0);

	n_ans = 0;

	if(nu > 0) search(0);

	// printf( "\nNumber of recurrence groups = %d\n", n_ans);
}

void RecurrenceGroup::search(int p) {
	for(int i = 0; i < (int) cand[p].size(); ++i) {
		int u = cand[p][i];
		ans[p] = u;
		in_ans[u] = true;

		cand[p+1].clear();
		cand_t[p+1].clear();
		cand_u.clear();

		for(int j = 0; j < (int) cand_t[p].size(); ++j) {
			int t = cand_t[p][j];
			cand_v.clear();
			for(int k = 0; k < (int) g[t][u].size(); ++k) {
				int v = g[t][u][k];
				++cnt[t][v];
				if(cnt[t][v] == p+1) cand_v.push_back(v);
			}
			if(cand_v.size() < tv) continue;
			cand_t[p+1].push_back(t);
			++nowt;
			for(int k = 0; k < (int) cand_v.size(); ++k) {
				int v = cand_v[k];
				for(int l = 0; l < (int) g[t][v].size(); ++l) {
					int uu = g[t][v][l];
					if(in_ans[uu]) continue;
					if(last[uu] != nowt) {cnt[t][uu] = 1; last[uu] = nowt;}
					else ++cnt[t][uu];
					if(cnt[t][uu] == tv) {
						if(cnt_t[uu] == 0) cand_u.push_back(uu);
						++cnt_t[uu];
					}
				}
			}
		}

		bool is_maximal = true;
		for(int j = 0; j < (int) cand_u.size(); ++j) {
			int uu = cand_u[j];
			if(cnt_t[uu] < lmd) {cnt_t[uu] = 0; continue;}
			cnt_t[uu] = 0;
			if(uu < u) is_maximal = false;
			else cand[p+1].push_back(uu);
		}

		if(p+1+cand[p+1].size() >= tu && cand_t[p+1].size() >= lmd) {
			sort(cand[p+1].begin(), cand[p+1].end());
			search(p+1);
			if(cand[p+1].size() == 0 && is_maximal) {
				++n_ans;
				// if(n_ans % 100 == 0) printf( "[%d]", n_ans );
				//output the answer here if necessary
				//printf( "Group #%d:", n_ans );
				//for(int j = 0; j <= p; ++j) printf( " %d", id_old[ans[j]]);
				//printf( "\n" );
			}
		}

		for(int j = 0; j < (int) cand_t[p].size(); ++j) {
			int t = cand_t[p][j];
			for(int k = 0; k < g[t][u].size(); ++k) --cnt[t][g[t][u][k]];
		}

		in_ans[u] = false;
	}
}

void RecurrenceGroup::core_prune() {
	sup.resize(nu,0);
	deg.resize(nt);
	for(int t = 0; t < nt; ++t) {
		deg[t].resize(n);
		for(int x = 0; x < n; ++x) {
			deg[t][x] = (int) g[t][x].size();
			if(x<nu && deg[t][x]) ++sup[x];
		}
	}

	for(int t = 0; t < nt; ++t)
		for(int x = 0; x < n; ++x)
			if((deg[t][x] > 0 && deg[t][x] < D(x)) || (x < nu && sup[x] > 0 && sup[x] < lmd))
				core_prune(t,x);

	m_old = m; m = 0;
	int n_error = 0;
	sup.clear();
	sup.resize(nu,0);

	for(int t = 0; t < nt; ++t) {
		int _m = 0;
		for(int x = 0; x < n; ++x) {
			if(deg[t][x] == 0) {g[t][x].clear(); continue;}
			int p = 0;
			for(int i = 0; i < (int) g[t][x].size(); ++i) {
				int y = g[t][x][i];
				if(deg[t][y] > 0) g[t][x][p++] = y;
			}
			g[t][x].resize(p);
			_m += p;
			if(p < D(x)) ++n_error;
			if(x < nu) ++sup[x];
		}
		if(_m % 2 != 0) ++n_error;
		m += _m;
	}

	for(int x = 0; x < nu; ++x) if(sup[x] > 0 && sup[x] < lmd) ++n_error;

	// printf( "Core prune finished, m = %d, n_error = %d\n", m/2, n_error );
	cPe = pre_m - m/2;
}

void RecurrenceGroup::reorder(int method) {
	vector<pair<int,int> > score(nu);
	vector<int> id(n);
	vector<bool> used(n,false);
	vector<bool> valid(nt,false);

	id_old.resize(n,-1);
	nu_old = nu; nv_old = nv; n_old = n; nt_old = nt;

	for(int x = 0; x < nu_old; ++x) score[x] = make_pair(0,x);
	for(int t = 0; t < nt_old; ++t) {
		for(int x = 0; x < nu_old; ++x)
			if(g[t][x].size() > 0) {
				if(method == 1) score[x].first += g[t][x].size();
				else score[x].first = 1;
				valid[t] = true;
			}
		for(int x = nu_old; x < n_old; ++x)
			if(g[t][x].size() > 0) used[x] = true;
	}

	sort(score.begin(), score.end());

	nu = 0;
	for(int i = 0; i < nu_old; ++i)
		if(score[i].first > 0) {
			id_old[nu] = score[i].second;
			id[score[i].second] = nu++;
		} else id[score[i].second] = -1;

	nv = 0;
	for(int x = nu_old; x < n_old; ++x)
		if(used[x]) {
			id_old[nu + nv] = x;
			id[x] = nu + (nv++);
		} else id[x] = -1;

	nt = 0;
	for(int t = 0; t < nt_old; ++t) if(valid[t]) ++nt;

	n = nu + nv;
	vector<vector<vector<int> > > gg(nt);
	for(int t = 0; t < nt; ++t) gg[t].resize(n);

	nt = 0;
	for(int t = 0; t < nt_old; ++t)
		if(valid[t]) {
			for(int x = 0; x < n; ++x){
				for(int i = 0; i < (int) g[t][id_old[x]].size(); ++i)
					gg[nt][x].push_back(id[g[t][id_old[x]][i]]);
				sort(gg[nt][x].begin(), gg[nt][x].end());
			}
			nt++;
		}

	g.swap(gg);

	// printf( "Recorder finished! nu = %d, nv = %d, nt = %d\n", nu, nv, nt );
	cPu = pre_nu - nu;
	cPv = pre_nv - nv;
}

void RecurrenceGroup::core_prune(int t, int x) {
	deg[t][x] = 0;
	for(int i = 0; i < (int) g[t][x].size(); ++i) {
		int y = g[t][x][i];
		if(deg[t][y] > 0) {
			--deg[t][y];
			if(deg[t][y] < D(y)) core_prune(t, y);
		}
	}

	if(x<nu && sup[x]>0) {
		--sup[x];
		if(sup[x] < lmd) {
			sup[x] = 0;
			for(int t = 0; t < nt; ++t)
				if(deg[t][x] > 0)
					core_prune(t, x);
		}
	}
}

bool RecurrenceGroup::get_temporal_edge(char *line, int &a, int &b, int &t, int num_cnt) {
	if( !isdigit(line[0]) ) return false;
	vector<char*> v_num;
	int len = (int) strlen(line);
	for( int i = 0; i < len; ++i )
		if( !isdigit(line[i]) && line[i] != '.') line[i] = '\0';
		else if(i == 0 || !line[i-1]) v_num.push_back(line+i);
	if( (int) v_num.size() != num_cnt ) return false;
	sscanf( v_num[0], "%d", &a );
	sscanf( v_num[1], "%d", &b );
	sscanf( v_num[num_cnt-1], "%d", &t );
	return true;
}

int RecurrenceGroup::get_num_cnt(string path) {
	FILE *fin = fopen( (path + ".txt").c_str(), "r" );
	char line[MAXST];
	int cnt = 0, min_cnt = 100;

	while(fgets( line, MAXST, fin ) && cnt < 10) {
		if(!isdigit(line[0])) continue;
		vector<char*> v_num;
		int len = (int) strlen(line);
		for(int i = 0; i < len; ++i)
			if(!isdigit(line[i]) && line[i] != '.') line[i] = '\0';
			else if(i == 0 || !line[i-1]) v_num.push_back(line+i);
		if( (int) v_num.size() < 2 ) continue;
		min_cnt = min(min_cnt, (int) v_num.size());
		++cnt;
	}
	fclose(fin);
	return min_cnt;
}
#endif /* RECURRENCEGROUP_H_ */
