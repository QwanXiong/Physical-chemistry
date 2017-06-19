#pragma once
#include <vector>
#include <cmath>
#include <cstdio>
using std::vector;

class xBS
{
private:
	int number_of_levels;
	int M;
	double Tempr;
	
	double deltaE;
	double E0;
	double sum;
	double sum_T;
	static const double planck_h = 3.33589E-11; // cm-1 * s
	static const double k_B = 0.69503; // cm-1 * K-1



public:
	xBS(int _M, double _Tempr, double _E0);
	double* AT;
	double* T;
	void algorithm(vector <double> &);
	int get_M()
	{
		return M;
	}
	void printT();

	void get_T(double *);

	double get_sum();
	double get_sum_T();
	//double k_E(double E, double*);
	void compute_sum_T();
	//double k_T(double, double* );
	~xBS();
	
};