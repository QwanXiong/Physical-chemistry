#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "files_handler.h"
#include "Token.h"
#include "xBS.h"
#include "freq_handler.h"
#include "rate_calculator.h"

using std::cout;
using std::string;

void execute_computation(Reader * reader);
void handle_system(Reader * reader);
void handle_calc(Reader * reader);
void handle_reagent(Reader * reader);
void handle_ts(Reader * reader);
void check_input();
void print_separator(bool nl)
{
	cout << (nl ? "" : "\n")<< "====================" << (nl ? "\n" : "");
}


double dimension_coefficient;
int use_extern_ts;
int use_extern_reagent;
int show_fr;
int M;
double T;
double E0;
string reagent_frq_filename;
string reagent_extern_dim;
string reagent_except_filename;

string ts_frq_filename;
string ts_extern_dim;
string ts_except_filename;

vector<double> temperatures;



int main()
{
	//system variables by default
	dimension_coefficient = 1.0;
    use_extern_ts = 0;
    use_extern_reagent = 0;
    show_fr = 0;
    M = 1000;
    T = 300.0;
    E0 = 1000.0;


	char* input_file_name = "input.inp";

	try {
		Reader input_file_reader(input_file_name);
		//input_file_reader.get();
		execute_computation(&input_file_reader);
		check_input();
	}
	catch(std::exception &e)
	{
		cout << e.what() << "\n";
	}


	xBS xBS_ts(M, T, E0);
    xBS xBS_prod(M, T, E0);


    freq_handler freq_prod(reagent_frq_filename.c_str(),reagent_except_filename.c_str(),M, show_fr);
    freq_prod.freq_file_handler();
    vector<double> arr ;
    
  
    while(freq_prod.get_curr_freq() < freq_prod.get_number_freq())
    {
        freq_prod.vector_maker(arr);
        xBS_prod.algorithm(arr);
         
    }	

	system("pause");


    freq_handler freq_ts(ts_frq_filename.c_str(),ts_except_filename.c_str(),M, show_fr);
    freq_ts.freq_file_handler();
    arr.clear();
  
    while(freq_ts.get_curr_freq() < freq_ts.get_number_freq())
    {
        freq_ts.vector_maker(arr);
        xBS_ts.algorithm(arr);
         
    }


    for (int i = 0; i < temperatures.size(); ++i)
    {

    	cout << "DOING CALCULATION FOR TEMPERATURE = " << temperatures[i] << "\n";
	    Rate_calculator Rate_constants(&xBS_prod,&xBS_ts,M,temperatures[i],E0 );

	    cout << "Q is: " << Rate_constants.Q_reagent << "\n";


	    printf("Temperature averaged rate constant: %e\n",Rate_constants.k_T());
	    cout << "\n";
	}

      system("pause");
   
  
	return 0;
}

void execute_computation(Reader * reader)
{

	while (true) {


		Token curr_tok = reader->get();

		//cout << curr_tok.toktype << '\n';
		//cout <<"alpha\n";

		if (curr_tok.toktype == Token::SECTION_KWD)
		{
			curr_tok = reader->get();
			switch (curr_tok.toktype){
				case Token::CALC:
					print_separator(false);
					cout <<"\nNOW READING CALC SECTION \n";
					print_separator(true);
					handle_calc(reader);
					break;

				case Token::SYSTEM:
					print_separator(false);
					cout <<"\nNOW READING SYSTEM SECTION \n";
					print_separator(true);
					handle_system(reader);
					break;

				case Token::REAGENT:
					print_separator(false);
					cout <<"\nNOW READING REAGENT SECTION \n";
					print_separator(true);
					handle_reagent(reader);
					break;

				case Token::TS:
					print_separator(false);
					cout <<"\nNOW READING TS SECTION \n";
					print_separator(true);
					handle_ts(reader);
					break;

				default:
					cout << "No such section;\n";

			}
		}
		else if (!reader->end_of_file())
		{
			throw std::runtime_error("Section keyword expected!\n");
		}
		else 
		{
			break;
		}

	}

}



void handle_system(Reader * reader)
{
	Token curr_tok = reader->get();

	while (curr_tok.toktype == Token::TEXT)
	{
		if (curr_tok.str == "use_extern_ts")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #SYSTEM SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING USE_EXTERN_TS VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					if (curr_tok.str == "true")
					{
						use_extern_ts = 1;
					}
					else if (curr_tok.str == "false")
					{
						use_extern_ts = 0;
					}
					else 
						throw std::runtime_error("UNDEFINED SEQUENCE IN USE_EXTERN_TS VARIABLE. GOT "+curr_tok.str + "\n");
				}

			}

			cout  << "USE ADDITIONAL FILES FOR TRANSITION STATE: "<< use_extern_ts<< '\n';
		}
		else if (curr_tok.str == "use_extern_r")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #SYSTEM SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING USE_EXTERN_R VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					if (curr_tok.str == "true")
					{
						use_extern_reagent = 1;
					}
					else if (curr_tok.str == "false")
					{
						use_extern_reagent = 0;
					}
					else 
						throw std::runtime_error("UNDEFINED SEQUENCE IN USE_EXTERN_R VARIABLE. GOT "+curr_tok.str + "\n");
				}

			}

			cout  << "USE ADDITIONAL FILES FOR REAGENT: "<< use_extern_reagent<< '\n';
		}
		else if (curr_tok.str == "show_fr")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #SYSTEM SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING SHOW_FR VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					if (curr_tok.str == "true")
					{
						show_fr = 1;
					}
					else if (curr_tok.str == "false")
					{
						show_fr = 0;
					}
					else 
						throw std::runtime_error("UNDEFINED SEQUENCE IN SHOW_FR VARIABLE. GOT "+curr_tok.str + "\n");
				}

			}

			cout  << "SHOW FREQUENCIES: "<< show_fr<< '\n';
		}
		else if (curr_tok.str == "units")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #SYSTEM SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING UNITS VARIZBLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					if (curr_tok.str == "cm")
					{
						dimension_coefficient = 1.0;
					}
					else if (curr_tok.str == "ev")
					{
						dimension_coefficient = 1/8065.73;
					}
					else 
						throw std::runtime_error("UNDEFINED SEQUENCE IN UNITS VARIABLE. GOT "+curr_tok.str + "\n");
				}

			}

			cout  << "UNITS: "<< dimension_coefficient << '\n';
		}
		else
			throw std::runtime_error("unexpected option in #system section!\n");

	//	cout << "str: "<< curr_tok.str << "\n";

		curr_tok = reader->get();
	}
	if (curr_tok.toktype == Token::SECTION_KWD)
	{
		curr_tok = reader->get();
		if (curr_tok.toktype != Token::END)
			throw std::runtime_error("in #system seciotn: #end statement is expected!\n");
	}
	else
		throw std::runtime_error("IN #SYSTEM SECTION: UNEXPECTED SEQUENCE!\n");

}

void handle_calc(Reader * reader)
{
	Token curr_tok = reader->get();

	while (curr_tok.toktype == Token::TEXT)
	{
		if (curr_tok.str == "m")
		{
			//cout << "dval = " << curr_tok.dval << "\n";
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #CALC SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::NUMBER)
					throw std::runtime_error("WHILE READING M VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					M = (int) curr_tok.dval;
				}

			}

			cout  << "M: "<< M << '\n';
		}		
		else if (curr_tok.str == "t")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #CALC SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				/*while (curr_tok.toktype != Token::SEMICOLON);
				{
					curr_tok = reader->get();
					if (curr_tok.toktype != Token::NUMBER)
						throw std::runtime_error("WHILE READING T VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
					else
					{
						T = curr_tok.dval;
						temperatures.push_back(curr_tok.dval);
					}
				} */

				curr_tok = reader->get();
				while (curr_tok.toktype == Token::NUMBER)
				{
					//if (curr_tok.toktype != Token::NUMBER)
					//	throw std::runtime_error("WHILE READING T VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
					//else
					//{
					//	T = curr_tok.dval;
						temperatures.push_back(curr_tok.dval);
						curr_tok = reader->get();
						//cout << "word\n";

					//}
				}


			}

			if (temperatures.empty())
				throw std::runtime_error("WHILE READING T VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");

			cout  << "Temperatures: ";

			for (int i = 0; i < temperatures.size(); ++i)
				cout << temperatures[i] << " ";
			cout << "\n";
			continue;
		}
		else if (curr_tok.str == "e0")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #CALC SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::NUMBER)
					throw std::runtime_error("WHILE READING E0 VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					E0 = curr_tok.dval;
				}

			}

			cout  << "E0: "<< E0 << '\n';
		}
		else
			throw std::runtime_error("UNEXPECTED OPTION IN #CALC SECTION!\n");

	//	cout << "str: "<< curr_tok.str << "\n";

		curr_tok = reader->get();
	}
	if (curr_tok.toktype == Token::SECTION_KWD)
	{
		curr_tok = reader->get();
		if (curr_tok.toktype != Token::END)
			throw std::runtime_error("IN #CALC SECTION: #END STATEMENT IS EXPECTED!\n");
	}
	else
		throw std::runtime_error("IN #CALC SECTION: #END STATEMENT IS EXPECTED!\n");


}

void handle_reagent(Reader * reader)
{
	Token curr_tok = reader->get();

	while (curr_tok.toktype == Token::TEXT)
	{
		if (curr_tok.str == "frq")
		{
			//cout << "dval = " << curr_tok.dval << "\n";
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #REAGENT SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING FRQ VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					reagent_frq_filename = curr_tok.str;	
				}

			}

			cout  << "FILENAME OF REAGENT FREQUENCIES: "<< reagent_frq_filename << '\n';
		}		
		else if (curr_tok.str == "extern")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #REAGENT SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING EXTERN VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					reagent_extern_dim = curr_tok.str;
				}

			}

			cout  << "DIMENSION OF ADDITIONAL FILES FOR REAGENT: "<< reagent_extern_dim << '\n';
		}
		else if (curr_tok.str == "except")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #REAGENT SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING EXTERN VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					reagent_except_filename = curr_tok.str;
				}

			}

			cout  << "FILENAME OF EXCLUDED FREQUENCIES FOR REAGENT: "<< reagent_except_filename << '\n';
		}
		else
			throw std::runtime_error("UNEXPECTED OPTION IN #REAGENT SECTION!\n");

	//	cout << "str: "<< curr_tok.str << "\n";

		curr_tok = reader->get();
	}
	if (curr_tok.toktype == Token::SECTION_KWD)
	{
		curr_tok = reader->get();
		if (curr_tok.toktype != Token::END)
			throw std::runtime_error("IN #REAGENT SECTION: #END STATEMENT IS EXPECTED!\n");
	}
	else
		throw std::runtime_error("IN #REAGENT SECTION: UNEXPECTED SEQUENCE!\n");

}

void handle_ts(Reader * reader)
{
	Token curr_tok = reader->get();

	while (curr_tok.toktype == Token::TEXT)
	{
		if (curr_tok.str == "frq")
		{
			//cout << "dval = " << curr_tok.dval << "\n";
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #TS SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING FRQ VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					ts_frq_filename = curr_tok.str;	
				}

			}

			cout  << "FILENAME OF TS FREQUENCIES: "<< ts_frq_filename << '\n';
		}		
		else if (curr_tok.str == "extern")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #TS SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING EXTERN VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					ts_extern_dim = curr_tok.str;
				}

			}

			cout  << "DIMENSION OF ADDITIONAL FILES FOR TS: "<< ts_extern_dim << '\n';
		}
		else if (curr_tok.str == "except")
		{
			curr_tok = reader->get();
			if (curr_tok.toktype != Token::EQUAL)
				throw std::runtime_error("AFTER VARIABLE IN #TS SECTION '=' IS EXPECTED!\n");
			else 
			{
				//cout << "units found\n";
				curr_tok = reader->get();
				if (curr_tok.toktype != Token::TEXT)
					throw std::runtime_error("WHILE READING EXTERN VARIABLE, EXPECTED VALUE IS NOT FOUND!\n");
				else
				{
					ts_except_filename = curr_tok.str;
				}

			}

			cout  << "FILENAME OF EXCLUDED FREQUENCIES FOR TS: "<< ts_except_filename << '\n';
		}
		else
			throw std::runtime_error("UNEXPECTED OPTION IN #TS SECTION!\n");

	//	cout << "str: "<< curr_tok.str << "\n";

		curr_tok = reader->get();
	}
	if (curr_tok.toktype == Token::SECTION_KWD)
	{
		curr_tok = reader->get();
		if (curr_tok.toktype != Token::END)
			throw std::runtime_error("IN #TS SECTION: #END STATEMENT IS EXPECTED!\n");
	}
	else
		throw std::runtime_error("IN #TS SECTION: UNEXPECTED SEQUENCE!\n");

}


void check_input()
{
	if ((use_extern_reagent == 1)&& reagent_extern_dim.empty())
		throw std::runtime_error("\nDIMENSION OF ADDITIONAL FILES  FOR REAGENT IS NOT SPECIFIED\n");
	if ((use_extern_ts == 1) && ts_extern_dim.empty())
		throw std::runtime_error("\nDIMENSION OF ADDITIONAL FILES  FOR TRANSITION STATE IS NOT SPECIFIED\n");
	//cout <<"\n" << use_extern_ts "\n";

}