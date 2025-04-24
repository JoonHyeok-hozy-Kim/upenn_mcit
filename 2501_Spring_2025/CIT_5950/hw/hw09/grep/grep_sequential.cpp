#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <unordered_set>
#include "./grep_request.hpp"

// You can add more includes if you want

using namespace std;

// Given a grep_request:
// 1. Prints out the grep_request
// 2. Prints out each line in the specified file that contains all 
//    words that are in the query.
//    e.g. if the query is {"hello.txt", {"hi", "bye"}}
//    then it prints all lines of hello.txt that contains
//    both the word "hi" and "bye"
// 
// The lines that are printed should be printed
// in the same order they show up in the file.
//
// If there are no lines that contain all words
// in the query, then nothing should be printed out
// after printing the request.
void grep(const grep_request& request);

// Reads a query from the console
// and returns it. Automatically retries
// if no system error or EOF was encountered, but
// user query was an invalid format.
//
// returns nullopt when the EOF (ctrl + d)
// is read. Meaning that the overall
// program should now terminate..
// returns some value if the reading was successful
optional<grep_request> read_query();

// you've implemented similar functions before
// We ask that you do so again
// the resulting function should return all tokens
// in a string split on whitespace. There should be
// no empty tokens in the result vector.
vector<string> split(const string& input);

// this function is given to you
int main() {
  auto opt = read_query();
  while (opt.has_value()) {
    grep(opt.value());
    opt = read_query();
  }

  return EXIT_SUCCESS;
}

// We've given you part of it to get you started
vector<string> split(const string& input) {
  vector<string> tokens;
  size_t pos = input.find_first_of(" \r\t\n");
  size_t start = 0;
  while (pos != string::npos) {
          if (pos - start > 0) {
                  tokens.push_back(input.substr(start, pos-start));
          }
          start = pos+1;
          pos = input.find_first_of(" \r\t\n", start+1);
  }

  if (start < input.size()) {
          tokens.push_back(input.substr(start, input.size()-start));
  }

  return tokens;
}

void grep(const grep_request& request) {
  std::cout << request.to_string() << std::endl;
  std::ifstream file(request.fname);
  // You probably want the above lines
  // you may want to take some inspiration from read_query
  // - how you can tread a stream like a bool in a condition
  // - using the getline function
  if (!file.is_open()) {
	 cerr << "Cannot open file : " << request.fname << endl; 
  }

  string line;
  while (getline(file, line)) {
		  vector<string> line_vec = split(line);
	  	  unordered_set<string> line_set(line_vec.begin(), line_vec.end());

		  bool skip = false;
		  for (auto word: request.queries) {
			  if (!line_set.contains(word)) {
				  skip = true;
				  break;
			  }
		  }
		  if (!skip) {
			  cout << line << endl;
		  }
  }
		
  return;
}

// this function is given to you
optional<grep_request> read_query() {
  grep_request request;

  string line;
  getline(cin, line);

  while (cin) {
    // tokenize the string
    vector<string> tokens = split(line);

    // parse tokens into a query

    if (tokens.size() > 1) {
      request.fname = tokens.at(0);
      tokens.erase(tokens.begin());
      request.queries = std::move(tokens);
      return request;
    }
    cerr << "Parsing Error: expected a file name followed by a one or more";
    cerr << " tokens to search for" << endl;

    // try reading line again
    getline(cin, line);
  }

  return nullopt;
}
