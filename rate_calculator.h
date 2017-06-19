#ifndef RATE_CALCULATOR_H
#define RATE_CALCULATOR_H

#include "xBS.h" 

class Rate_calculator
{
private:
	double * reagent_T_array;
	double * ts_T_array;
	double Tempr;
	double E0;
	int M;
	
	static const double planck_h = 3.33589E-11; // cm-1 * s
	static const double k_B = 0.69503; // cm-1 * K-1
	double deltaE;




public:
	Rate_calculator(xBS *reagent, xBS * ts, int length, double _Tempr, double _E0);
	~Rate_calculator();
	double k_E(double E0);
	double k_T();
	void compute_Q_reagent();

	double Q_reagent;


};



#endif
