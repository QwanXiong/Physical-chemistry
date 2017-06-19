#include "freq_handler.h"



freq_handler::freq_handler(const char * _filename, const char* _except, int _M, int _show_freq): M(_M), show_freq(_show_freq)
{
	 filename = new char[64];
	 except = new char[64];
	strcpy(filename, _filename);
	strcpy(except, _except);

	input_file = fopen(filename,"r");
	except_file = fopen(except,"r");
	curr_frequency = 0;
}

void freq_handler::freq_file_handler()
{
	//считываем количество частот
	fscanf(input_file,"%d",&number_of_freq);
	
	int curr_except;
	double curr_freq;
	bool skipped = false;
	// массив, в котором на месте "выколотых" частот стоят false
	double * except_array  = new double[number_of_freq];

	//заполняем массив true-элементами
	for (int i = 0; i < number_of_freq; ++i)
	{
		except_array[i] = true;
	}

    /*for (int i = 0; i < number_of_freq; ++i)
	{
		fscanf(except,"%lf",curr_except);
	}*/

	//for(;;)
	//fscanf(except_file,"%d",&curr_except);

	printf("Skipped frequencies:\n");
	while(fscanf(except_file,"%d",&curr_except) == 1)
	{	
		
		//if (feof(except_file)) break;
		skipped = true;

		printf("%d  ", curr_except);
		except_array[curr_except] = false;

	}
	if (!skipped)
		printf("None\n");
	else 
		printf("\n");

	/*for (int i = 0; i < number_of_freq; ++i)
	{
		//printf("%d  ", except_array[i]);
		//printf("%s\n",  except_array[i] ? "true" : "false");
	}	*/

	//int counter = 0;
	for (int i = 0;fscanf(input_file,"%lf",&curr_freq) == 1; ++i)
	//while (fscanf(input_file,"%lf",&curr_freq) == 1 )
	{
		
		//++counter;
		
		//printf("%f\n",curr_freq);
		//if (feof(input_file))
		//	break;
		if(!except_array[i]) continue;

		
		
		//printf("%f\n",curr_freq);
		
		frequencies.push_back(curr_freq);
		
		
	}
	number_of_freq = frequencies.size();
	/*printf("%d\n",frequencies.size());
	for (int i = 0; i < frequencies.size(); ++i)
	{
		printf("%f\n",frequencies[i]);
		//printf("%s\n",  except_array[i] ? "true" : "false");
	}	*/

	printf("Number of frequencies used in calculation:\n");
	printf("%d\n",number_of_freq);

	if (show_freq == 1)
	{
		printf("Frequencies used in calculation:\n");
		for (int i = 0; i < number_of_freq; ++i)
		{
			printf("%f\n",frequencies[i]);
		}
	}

	delete [] except_array;




}

void freq_handler::vector_maker(vector <double> &levels)
{
	int number_of_levels = (int) M/frequencies[curr_frequency];
		//printf ("%d \n", number_of_levels);
	//vector<double> levels(number_of_levels);
	levels.resize(number_of_levels);

	for (int k =0; k < number_of_levels; ++k)
		levels[k] = frequencies[curr_frequency]*(k+1);
	++curr_frequency;

	//return levels;
}

int freq_handler::get_curr_freq()
{
	return curr_frequency;
}

int freq_handler::get_number_freq()
{
	return number_of_freq;
}

freq_handler::~freq_handler()
{
	delete[] filename;
	delete[] except;

	fclose(except_file);
	fclose(input_file);

}