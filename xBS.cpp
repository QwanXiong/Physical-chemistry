#include "xBS.h"
#include <iostream>

using std::cout;


xBS::xBS(int _M, double _Tempr, double _E0): M(_M),Tempr(_Tempr), E0(_E0)
{
	AT = new double[M];
	T = new double[M];
	AT[0] = 1.0;
	T[0] = 1.0;

	for(int i =1; i < M; ++i)
	{
		T[i] = 0;
		AT[i] = 0;
	}

	deltaE = 1.0;
}

void xBS::algorithm(vector <double> &levels)
{

	number_of_levels = levels.size();
	int * R = new int[number_of_levels];
	for (int k =0; k < number_of_levels; ++k)
	{
		R[k] = round(levels[k]/deltaE);
		//printf ("%d \n", number_of_levels);

	}

	for (int k = 0; k < number_of_levels; ++k)
	{
		for (int I =0; I < M-R[k]; ++I)
		{
			AT[R[k]+I] += T[I];
		}
	}

	for (int I = 0; I < M; ++I)
		T[I] = AT[I];
		
	delete [] R;

}

void xBS::printT()
{
	FILE* Tfile=fopen("Tarray.txt","w");
	for (int i = 0; i < M; ++i)
	{
		fprintf(Tfile,"%f\n",T[i]);
	}
}


void xBS::get_T(double * _T)
{
	//_T = T;
	for (int i = 0; i < M; ++i)
	{
		_T[i] = T[i];
	}


}
double xBS::get_sum()
{
	sum = 0;
	for (int i = 0; i < M; ++i)
	{
		sum += T[i];
	}
	return sum;	
}



double xBS::get_sum_T()
{
	return sum_T;
}








xBS::~xBS()
{
	delete [] AT;
	delete [] T;
}