#include "Multifa_random_tool.h"


int main(int argc, char *argv[]){

	command_line_parser(argc, argv);					//Parser function called to handle aguments
	string sequence;
	ofstream outfile;
	outfile.open("random_multifasta.fasta");

	for(int j=0; j<n_seq; j++){
		
		sequence.clear();
		
		for(int i=0; i<length; i++){

			char base = random_number();
			sequence = sequence + base;
		}

		outfile << ">random sequence " << j << " containing " << length << " bases." << endl;
		outfile << sequence << endl;
		outfile << endl;
	}

	matrix_class M(JASPAR_FILE);
}

char random_number(){

	const gsl_rng_type * T;
	gsl_rng *r;

	gsl_rng_env_setup();
	struct timeval tv;		//Generate a seed depending on time to get every time different random number
	gettimeofday(&tv, 0);
	unsigned long mySeed = tv.tv_sec + tv.tv_usec;

	T = gsl_rng_default;
	r = gsl_rng_alloc(T);
	gsl_rng_set(r, mySeed);
		
	int r_number = gsl_rng_uniform_int(r,4);
	
	char base = from_n_to_base(r_number);

	gsl_rng_free(r);

	return 	base;
}

char from_n_to_base (int n){

	char base;
			switch(n){

				case 0:

					base = 'A';
					break;

				case 1:

					base = 'T';
					break;

				case 2:

					base = 'C';
					break;

				case 3:

					base = 'G';
					break;

				default:
					
					cout << "Generation of random sequence failed. Please check the random generation code.!" << endl;
					exit(1);
					break;
			}
			return base;
}

void matrix_class::read_JASPAR(string JASPAR_FILE){			//Function to read JASPAR PWM file, extract values and create a matrix class

	cout << "Reading JASPAR MATRIX file and extracting values...\n";

	ifstream file(JASPAR_FILE);					//opening JASPAR PWM file
	string line;							
	while(getline(file,line)){					//For each line of the file do:

		if(line[0]=='>'){					//If line start with ">"
			istringstream mystream(line);			
			mystream >> matrix_name >> tf_name;			//Extract the first two words and put into matrix_name string variable and tf_name string variable
		}

		else{							//Else, if line does not start with ">"
			line.erase(0,line.find('[') +1);		//Take line charachters after "["...
			line.erase(line.find(']'));			//...and line charachters before "]"
			vector<double> baseQ;				//Initializing baseQ vector of double
			istringstream mystream(line);			//Splitting the line in words
			for (double num; mystream >> num;){		//Put every word(number of matrix), ricorsively, in double variable num
				baseQ.emplace_back(num);		//Put every number(num) in baseQ vector
			}
			matrix.emplace_back(baseQ);			//Put baseQ vector (corrisponding to matrix line values) in our matrix

		}

	}
	file.close();						//Closing file
}

void matrix_class::find_col_sum(){

	vector<double> col_sum;						//Vector of columns sum
	double sum = 0;							//Sum initialized as 0
	cout << "\nCOL SUM: ";

	for (unsigned int i = 0; i < matrix[0].size(); i++) {			//From 0 to number of columns of line 0
		for (unsigned int j = 0; j < 4; j++){				//From 0 to 4 (line number)

			sum = sum + matrix[j][i];			//Calculate the sum of columns
		}
		cout << sum << " ";
		col_sum.emplace_back(sum);				//Put the column sum in vector col_sum
		sum = 0;						//Restore the sum to 0 for the next column
	}
}

/////////////////////////////////////// DEBUG ////////////////////////////////////


void matrix_class::print_debug_matrix(){		//Print matrix function

	cout << "\n" << matrix_name << " " << tf_name << ":" << endl;

	for(unsigned int i=0; i < matrix.size(); i++){
		for(unsigned int j=0; j<matrix[i].size(); j++){

			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}
/////////////////////////////////////// PARSER ///////////////////////////////////

void command_line_parser(int argc, char** argv){
	
	const char* const short_opts = "hl:n:j:";

	//Specifying the expected options
	const option long_opts[] ={
		{"help",      no_argument, nullptr,  'h' },
		{"length",      required_argument, nullptr,  'l' },
		{"jaspar",   required_argument, nullptr,  'j' },
		{"nseq",   required_argument, nullptr,  'n' },
	};

	while (true)
	{
		const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

		if (-1 == opt)
			break;


		switch (opt) {
			case 'h' : display_help();
				   break;
			case 'l' : length = stoi(optarg); 
				   break;
			case 'n' : n_seq = stoi(optarg); 
				   break;
			case 'j' : JASPAR_FILE = string(optarg);
				   is_file_exist(JASPAR_FILE, "--jaspar || -j");
				   break;
			case '?': // Unrecognized option
			default:
				   display_help();
				   break;
		}
	}
}

bool is_file_exist(string fileName, string buf){		//Input files existence control

	struct stat check;
	int regular_check, existing_check;
	const char * C_fileName = fileName.c_str();
	existing_check = stat(C_fileName, &check );

	regular_check = S_ISREG( check.st_mode );

	if ( regular_check == 0 || existing_check != 0) {
		cerr <<"ERROR: "<< buf << " file does not exist!\n"<< endl;
		display_help();
		exit(1);	
	}
	return 0;
}

void display_help() 						//Display help function
{
	cerr << "\n --help || -h show this message" << endl;
	cerr << "\n --length || -l <number> to insert the length of Multifasta sequences (DEFAULT: 500)" << endl;
	cerr << "\n --nseq || -n <number> to insert the number of Multifasta sequences (DAFAULT: 200)" << endl;
	cerr << endl;

	exit(EXIT_SUCCESS);
}