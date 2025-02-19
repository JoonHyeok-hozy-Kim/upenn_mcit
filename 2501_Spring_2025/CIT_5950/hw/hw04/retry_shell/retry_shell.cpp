

// Nothing here!
// Highly recommend you look at stdin_echo.cpp for getting started.
// The lecure example code (and the specification for this assignment) are also good to read.

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h> // for fork, execvp
#include <cstring> // for strerror
#include <sys/types.h>
#include <sys/wait.h>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::getline;
using std::vector;

bool is_int(char chr){
	if (chr >= '0' && chr <= '9'){
		return true;
	}
	return false;
}


int validate_argument(int argc, char** argv){
	// 0 Retry Case
	if (argc == 1){
		return 0;
	}

	// Invalid number of input arguments (more than 1)
	if (argc != 2){
		return -1;
	}

	char* curr_char = argv[1];
	int sum = 0;
	const int DECIMAL_DIGIT = 10;

	while (*curr_char != '\0'){
		if (!is_int(*curr_char)){
			// Invalid character. Only integers!
			return -1;
		}
		sum *= DECIMAL_DIGIT;
		sum += *curr_char - '0';
		curr_char++;

	}

	return sum;
}


size_t find_first_of(const string& str, const string& dilim, size_t pos) {

         if (pos < 0 || pos >= str.size()) {
		 cerr << "[find_first_of] Index out of range" << endl; 
		 return -1;
         }

         while (pos < str.size()){
		 for (char dil : dilim){
                         if (str.at(pos) == dil){
                                 return pos;
                         }

                 }
                 pos++;
         }
         return -1;
}



vector<string> split(const string& str, const string& dilim) {
	vector<string> res {};
	size_t prev = 0;
	size_t curr = 0;

	while (prev < str.size()){
		curr = find_first_of(str, dilim, prev);
		if (curr == string::npos){
			break;
		}

		if (prev < curr) {
			res.push_back(str.substr(prev, curr - prev));
		}

		prev = curr + 1;
	}

	if (prev < str.size()){
		res.push_back(str.substr(prev, curr - prev));
	}

	return res;
}


vector<char*> convert_to_cstring(vector<string> src){
	vector<char*> res;
	for (auto& str: src){
		res.push_back(str.data());
	}
	res.push_back(nullptr);
	return res;
}

int execute_cstr(vector<char*> &cstr_vec) {
	pid_t cpid = fork();

	if (cpid == 0){
		cout.flush();
		execvp(cstr_vec.at(0), const_cast<char**>(cstr_vec.data()));
		exit(EXIT_FAILURE);
	} else if (cpid < 0) {
		cerr << "execute_cstr run, fork error." << endl;
		return EXIT_FAILURE;
	}

	int status = 0;	// Initialized because of the tidy-check!
	pid_t wpid = wait(&status);
	
	if (wpid < 0) {
		cerr << "execute_cstr run, wait error." << endl;
		return EXIT_FAILURE;
	}

	if (WIFEXITED(status)) {
		int exit_status = WEXITSTATUS(status);
		if (exit_status == 0) {
			//cout << "execute_cstr run success!" << endl;
			return EXIT_SUCCESS;
		} 
	}	

	return EXIT_FAILURE;
}


int main(int argc, char** argv){
	// Validate arguments 
	int retry_cnt = validate_argument(argc, argv);
	if (retry_cnt < 0){
		cerr << "Invalid argument error." << endl;
		return EXIT_FAILURE;
	}

	//cout << "retry_cnt : " << retry_cnt << endl;
			
	string line;

	while (true){
		cout << "$ ";

		if (!getline(cin, line)) {
			if (!cin.eof()){
				cerr << "Warning: error while reading input from user" << endl;
			}
			// ctrl + D
			cout << endl;
			break;
		} 
		
		// Explicit exit command
		if (line == "exit"){
			break;
		}

		
		//cout << "Input command : " << line << endl;
		vector<string> string_cmd = split(line, " \t\n");
		vector<char*> cstr_cmd = convert_to_cstring(string_cmd);

		// First run
		if (execute_cstr(cstr_cmd) == 0 || retry_cnt == 0) {
			continue;
		}

		// Retry
		int curr_cnt = 0;
		int run_check = -1;
		while (curr_cnt < retry_cnt) {
			cerr << "retrying..." << endl;

			run_check = execute_cstr(cstr_cmd);
			if (run_check == 0){
				break;
			}
					
			curr_cnt++;
		}

		if (run_check != 0) {
			cerr << "Failed to run program after retrying" << endl;
		}

	}

	return EXIT_SUCCESS; 
}

