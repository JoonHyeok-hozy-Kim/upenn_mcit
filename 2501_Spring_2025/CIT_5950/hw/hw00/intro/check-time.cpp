#include "./simple_string.hpp"
#include "./ctypes.hpp"

#include <iostream>  // for cerr, cout, and endl

using namespace std;

int failure(simple_string* ss){
  destruct(*ss);
  cout << "INVALID" << endl;
  return EXIT_FAILURE;
}

int main(int argc, char* argv[]) {
  // checks that the correct number of command line args are provided
  if (argc != 2) {
    cerr << "Usage: ./check-time <string_to_check>" << endl;
    return EXIT_FAILURE;
  }

  simple_string input = from_cstring(argv[1]);

  // you probably don't need this line  ....
  // cout << input.data << endl;
  // cout << input << endl;

  // TODO: THE REST
  // Validate with length
  if (!(input.len == 4 || input.len == 5)){
	  return failure(&input);
  }

  // Find the position of ':'
  size_t colon_idx = find(input, ':');
  if (colon_idx == simple_string::npos || !(colon_idx == 1 || colon_idx == 2)){
	  return failure(&input);
  }

  // Validate digit values
  // Minutes
  if (!(isdigit(at(input, colon_idx+1)) && (isdigit(at(input, colon_idx+2))))){
	  return failure(&input);
  } else if ( (int) (at(input, colon_idx+1) - '0') >= 6){
	  return failure(&input);
  }

  // Hours
  if (!(isdigit(at(input, colon_idx-1)))){
	  return failure(&input);
  }
  int one_digit = (int) (at(input, colon_idx-1) - '0');
  
  if (input.len == 5){
	  if (at(input, colon_idx+3) != '\0'){
		  return failure(&input);
	  }
	  if (!(isdigit(at(input, colon_idx-2)))){
		  return failure(&input);
	  }
          
	  int ten_digit = (int) (at(input, colon_idx-2) - '0');
	  if (ten_digit > 2){
		  return failure(&input);
	  } else if (ten_digit == 2 && one_digit > 3) {
		 return failure(&input);
	  } 
  }
  
  cout << "VALID" << endl;
  destruct(input);
  return EXIT_SUCCESS;
}

