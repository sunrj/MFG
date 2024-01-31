#include "graph.h"
#include <ctime>

int main(int argc, char *argv[])
{
	string algo, graphname, graphfile;
	I K_V, L_U, O;
	algo=argv[1];
	graphname=argv[2];
	L_U=atoi(argv[3]);
	K_V=atoi(argv[4]);
	O=atoi(argv[5]);

	// cout << algo << "  " << graphname << "  L_U=" << L_U << "  K_V=" << K_V << "  O=" << O << endl;

	graphfile="../../sample_dataset/" + graphname + ".txt";

	Graph g;

	g.K_V = K_V;
	g.L_U = L_U;
	g.O = O;

	g.dataInput(graphfile);
	// cout << "InputDone" << endl;


	auto start_core = chrono::steady_clock::now();
	for (auto t = 0; t < g.T; t++)
	{
		g.Core(t);
	}
	auto end_core = chrono::steady_clock::now();
	double Core_time = chrono::duration_cast<chrono::microseconds>(end_core - start_core).count();

	g.Pruning_Size();
	g.Update_Graph(graphfile);


	auto start_crcore = chrono::steady_clock::now();
	g.CRCore();
	auto end_crcore = chrono::steady_clock::now();
	double CRCore_time = chrono::duration_cast<chrono::microseconds>(end_crcore - start_crcore).count();


	g.Pruning_Size();
	g.Update_Graph(graphfile);
	


	string s = "-";
	VI em1 = {}, em2 = {};
	VI in_T(g.T);
	auto start = chrono::steady_clock::now();
	try{
		g.Base(g.U, em1, g.V, em2, in_T, 0);
	}
	catch (string s1){
		s = s1;
	}
	auto end = chrono::steady_clock::now();
	double RuningTime = chrono::duration_cast<chrono::microseconds>(end - start).count();


	cout<<endl<<s<<endl<<endl;
	
	cout << "algorithm = " << algo << endl;
	cout << "graphname = " << graphname << endl;
	cout << "L_U = " << L_U << endl;
	cout << "K_V = " << K_V << endl;
	cout << "O = " << O << endl << endl;

	cout << "uvPu : " << g.cPu << endl; 
	cout << "uvPv : " << g.cPv << endl; 
	cout << "uvPe : " << g.cPe << endl; 

	cout << "uvPu/U : " << double(g.cPu)/g.n_u << endl; 
	cout << "uvPv/V : " << double(g.cPv)/g.n_v << endl; 
	cout << "uvPe/E : " << double(g.cPe)/g.m << endl << endl;  

	cout << "OUT_num : " << g.OUT_num << endl << endl;

	cout << "ALLRuningTime: " << RuningTime/1000000 + Core_time/1000000 + CRCore_time/1000000 << endl << endl;


	return 0;
}