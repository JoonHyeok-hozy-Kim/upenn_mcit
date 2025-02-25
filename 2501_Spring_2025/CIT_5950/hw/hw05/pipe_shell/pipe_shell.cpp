#include <unistd.h>    // for fork()
#include <sys/types.h> // for pid_t
#include <sys/wait.h>  // for wait(), waitpid(), etc.
#include <cstdlib>  // for exit(), EXIT_SUCCESS, and EXIT_FAILURE

// libs added by hozy
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <array>

using std::cout;
using std::string;
using std::endl;
using std::cin;
using std::cerr;
using std::vector;
using std::array;
using std::pair;

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

vector<char*> convert_to_cstring(vector<string> &src){
        vector<char*> res;
        for (auto& str: src){
                res.push_back(str.data());
        }
        res.push_back(nullptr);
        return res;
}


int call_child_process(string& cmd, vector<array<int, 2>>& fds_vec, vector<int>& pid_vec, int curr_idx){
	// Indices for convenience.
	bool is_last = (curr_idx == fds_vec.size());
	// Assign pref_fds
	array<int, 2>* prev_fds = nullptr;
	if (curr_idx > 0) {
		prev_fds = &fds_vec.at(curr_idx-1);
	}

	// Assign curr_fds
	array<int, 2>* curr_fds = nullptr;
	if (!is_last) {
		curr_fds = &fds_vec.at(curr_idx);
	
	}
	
	// Pipe curr_fds
	if (curr_fds != nullptr && pipe((*curr_fds).data()) < 0) {
		cerr << "pipe error at the cmd : " << strerror(errno) << endl; 
		exit(EXIT_FAILURE);
	}

	vector<string> string_cmd = split(cmd, " \t\n");
	vector<char*> cstr_cmd = convert_to_cstring(string_cmd);
	
	// Fork a child process for the curr cmd
	pid_t pid = fork();
	pid_vec[curr_idx] = pid;

	// Child Process
	if (pid == 0) {
		//cout << cmd << " Child Process" << endl; 

		if (!is_last) {
			//cout << "Not Last!" << endl;
			close((*curr_fds).at(0));	// Close curr_fds read.
		}

		if (prev_fds != nullptr) {
			//cout << "Not First!" << endl;
			int ret1 = dup2((*prev_fds).at(0), STDIN_FILENO); // Pipe prev_fds read to STDIN
			if (ret1 < 0) {
				cerr << "dup2 ret1 at " << cmd << " : " << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			close((*prev_fds).at(0)); // Close prev_fds after dup2.
		}

		if (!is_last) {
			int ret2 = dup2((*curr_fds).at(1), STDOUT_FILENO); // dup2 curr_fds write to STDOUT
			if (ret2 < 0) {
				cerr << "dup2 ret2: " << strerror(errno) << endl;
				exit(EXIT_FAILURE);
			}
			close((*curr_fds).at(1));	// Close curr_fds after dup2.
		}

		//vector<string> string_cmd = split(cmd, " \t\n");
		//vector<char*> cstr_cmd = convert_to_cstring(string_cmd);
		
                execvp(cstr_cmd.at(0), const_cast<char**>(cstr_cmd.data()));
		cerr << strerror(errno) << endl;
                //cerr << "exec_cstr, run failure return from execvp" << endl; 
		exit(EXIT_FAILURE);
	}

	// Parent Process
	//cout << "Parent!" << endl;
	if (!is_last) {
		close((*curr_fds).at(1));
	}
	if (prev_fds != nullptr) {
		close((*prev_fds).at(0));
	}


	return EXIT_SUCCESS;	
	

}


int main() {
  // Todo: implement
  string line;
  
  while (true) {
	// Pringing $ on the terminal output.
	cout << "$ ";

	// Escape when encountered EOF.
	if (!getline(cin, line)) {
		if (!cin.eof()) {
			cerr << "Warning: error while reading input from user" << endl;
		}

		cout << endl;
		break;
	}

	// Escape when encountered "exit".
	if (line == "exit") {
		break;
	}


	// The main logic goes....

	// Split cmds with "|" operator.
	vector<string> vert_split_str = split(line, "|");
	size_t num_cmds = vert_split_str.size();

	// Generate fds vectors for each cmd in advance (for parallel running)
	vector<array<int, 2>> fds_vec {};
	vector<int> pid_vec {};
	for (int i=0; i<num_cmds; i++){
		fds_vec.push_back({-1,-1});
		pid_vec.push_back(-1);
	}
	fds_vec.pop_back(); // Pop the last fds (not used!)

	for (int i=0; i<num_cmds; i++){
		if(call_child_process(vert_split_str.at(i), fds_vec, pid_vec, i) != 0){
			//return EXIT_FAILURE;
			cerr << "call_child_process error" << endl;
		}
	}

	//cout << "fds_vec.size() : " << fds_vec.size();
	//cout << "pid_vec.size() : " << pid_vec.size();



	int status = 0;
	for (int pid: pid_vec) {
		if (waitpid(pid, &status, 0) < 0) {
			//return EXIT_FAILURE;
			cerr << "main waidpid error, pid : " << pid << endl;
		}
		//cout << "Wait for " << pid << "done!" << endl;
	}
	

  }
	
  return EXIT_SUCCESS;
}

