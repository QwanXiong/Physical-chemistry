#pragma once
#include <vector>
#include <cmath>
#include <cstring>
#include <cstdio>
using std::vector;

class freq_handler
{
private: 
	char* except;
	char * filename;
	//вектор, содержащий частоты осцилляторов
	vector<double> frequencies;
	FILE * input_file;
	FILE* except_file;
	//количество частот, после вызова freq_file_handler соответствует реально используемому
	int number_of_freq;
	int M;
	int curr_frequency;
	int show_freq;


public:
	freq_handler(const char*, const char*,int, int _show_freq);
	void freq_file_handler();
	void vector_maker(vector<double> &);
	int get_curr_freq();
	int get_number_freq();
	~freq_handler();


};