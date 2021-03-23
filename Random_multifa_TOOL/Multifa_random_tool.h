#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iterator>
#include <list>
#include <vector>
#include <algorithm>
#include <map>
#include <typeinfo>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>
#include <sys/time.h>
#include <sstream>
#include <numeric>
#include <getopt.h>
#include <unordered_map>

using namespace std;
int length = 500;
int n_seq = 200;
string JASPAR_FILE;


class matrix_class {

	private: //field definition

		string matrix_name;
		string tf_name;
		vector<vector<double>> matrix;
		vector<double> col_sum;		


		void read_JASPAR(string);
		void find_col_sum();
		void print_debug_matrix();

	public:
		matrix_class(string JASPAR_FILE){

			read_JASPAR(JASPAR_FILE);
			print_debug_matrix();
			find_col_sum();

		}
};


void command_line_parser(int, char **);
void display_help(); 						//Display help function
char random_number();
char from_n_to_base(int);
bool is_file_exist(string, string);