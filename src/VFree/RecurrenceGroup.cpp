#include "RecurrenceGroup.h"

void run(string path, int time_scale, int tu, int tv, int lmd, int group_v, int core_prune, int order_method) {
	RecurrenceGroup r(path, time_scale, tu, tv, lmd, group_v);
	clock_t tt = clock();
	if(core_prune) r.core_prune();
	r.reorder(order_method);
	r.search();
	tt = clock()-tt;
	printf( "Processing time = %0.3lf seconds\n", tt*1.0/CLOCKS_PER_SEC);
}

int main(int argc, char *argv[]) {

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	clock_t t = clock();
	clock_t t_core;

	string graphname;
	int tu;
	int tv;
	int lmd;
	string algo;

	if( argc > 1 ) {
		algo = argv[1];
		graphname = argv[2];
		int time_scale = 1;
		tu = atoi(argv[3]);
		tv = atoi(argv[4]);
		lmd = atoi(argv[5]);
		int group_v = 1;
		int core_prune = 1;
		int order_method = 1;
		string path = "../../sample_dataset/" + graphname;

		cout << graphname + "_U=" + to_string(tu) + "_V=" + to_string(tv) + "_O" + to_string(lmd) << endl << endl;

		RecurrenceGroup r(path, time_scale, tu, tv, lmd, group_v);

		auto start = chrono::steady_clock::now();
		r.core_prune();
		auto end_core = chrono::steady_clock::now();
		double Core_time = chrono::duration_cast<chrono::microseconds>(end_core - start).count();

		r.reorder(order_method);
		r.search();
		auto end = chrono::steady_clock::now();
		double RuningTime = chrono::duration_cast<chrono::microseconds>(end - start).count();


		cout << "algorithm = " << algo << endl;
		cout << "graphname = " << graphname << endl;
		cout << "U = " << tu << endl;
		cout << "V = " << tv << endl;
		cout << "O = " << lmd << endl << endl;

		cout << "Pu : " << r.cPu << endl; 
		cout << "Pv : " << r.cPv << endl; 
		cout << "Pe : " << r.cPe << endl; 

		cout << "Pu/U : " << double(r.cPu)/r.pre_nu << endl; 
		cout << "Pv/V : " << double(r.cPv)/r.pre_nv << endl; 
		cout << "Pe/E : " << double(r.cPe)/r.pre_m << endl << endl; 

		cout << "OUT_num : " << r.n_ans << endl << endl;

		cout << "ALL_time : " << RuningTime/1000000 << endl << endl;
	}

	if( argc <= 1) {

		string path = "../../data/D13";
		RecurrenceGroup r(path, 1, 10, 6, 10);
		// RecurrenceGroup r(path, 1, 4, 4, 3);

		clock_t tt = clock();
		r.core_prune();
		r.reorder();
		r.search();
		tt = clock()-tt;
		printf( "Processing time = %0.3lf seconds\n", tt*1.0/CLOCKS_PER_SEC);
	}

	t = clock() - t;



	


	return 0;
}
