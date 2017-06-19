
#include <cmath>

#include "rate_calculator.h"
#include "xBS.h" 



Rate_calculator::Rate_calculator(xBS *reagent, xBS * ts, int length, double _Tempr, double _E0): Tempr(_Tempr), E0(_E0), M(length){
	reagent_T_array =  new double [length];
	ts_T_array =  new double [length];

	for (int i = 0; i < length; ++ i)
	{
		reagent_T_array[i] = reagent->T[i];
		ts_T_array[i] = ts->T[i];
	}


	compute_Q_reagent();
	deltaE = 1;
}



Rate_calculator::~Rate_calculator(){
	delete[] reagent_T_array;
	delete[] ts_T_array;

}


double Rate_calculator::k_E(double E) //E0 - barrier, E -energy at which k is computed
{
	int counter = 0;
	double W = 0.0;
	double rho;


	if (E < E0)
		return 0.0;

	while (counter < (E-E0))
	{
		W += ts_T_array[counter];
		++counter; 
	}

	rho = reagent_T_array[(int)round(E)];
	//cout << rho << "\n";
    return W/rho/planck_h; // in s-1
	//return W/planck_h; 
	//return planck_h/rho; 


}


void Rate_calculator::compute_Q_reagent()
{
	Q_reagent = 0;
	for (int i = 0; i < M; ++i)
	{
		Q_reagent += reagent_T_array[i]*exp(-(double)i/(Tempr*k_B));
	}
	//return sum;	
}

double Rate_calculator::k_T() //E0 - barrier, E -energy at which k is computed
{

	
	double E = E0;
	double result = 0.0;
	double rho;



	/*while (counter < (E-E0))
	{
		W += T[counter];
		++counter; 
	}

	

	return W/rho/planck_h;*/

	while (E < M-1)
	{
		rho = reagent_T_array[(int)round(E)];
		result += k_E(E)*rho*exp(-E/k_B/Tempr)/Q_reagent;
		//printf("%e ", result);


		E += deltaE;
	}

	return result;
}