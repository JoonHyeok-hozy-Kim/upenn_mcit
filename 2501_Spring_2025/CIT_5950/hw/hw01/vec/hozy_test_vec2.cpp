#include "./vec.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv){
	vec original;
	for (size_t i = 0; i < 100; ++i) {
	    original.push_back("Item" + std::to_string(i));
 	 }

  	// Create three chained copies
  	vec copy1(original);
  	vec copy2(copy1);
  	vec copy3(copy2);

  	// Modify original
  	original.push_back("gamma");

	copy1.push_back("delta");
	copy2.at(0) = "changed";
	copy3.push_back("epsilon");

  vec large_vec;
  for (size_t i = 0; i < 100; ++i) {
    std::string large_string(1000, 'A' + (i % 26));
    large_vec.push_back(large_string);
  }
  vec large_copy(large_vec);

  bool identical = true;
  for (size_t i = 0; i < large_vec.length(); ++i) {
    if (large_copy.at(i) != large_vec.at(i)) {
      identical = false;
      break;
    }
  }


   vec large_preallocated(200);
  for (size_t i = 0; i < 150; ++i) {
    large_preallocated.push_back("BigItem" + std::to_string(i));
  }

  vec large_preallocated_copy(large_preallocated);
  identical = true;
  for (size_t i = 0; i < 150; ++i) {
    if (large_preallocated_copy.at(i) != large_preallocated.at(i)){
      identical = false;
    }
  }
	if (identical){
		cout << "used for reason lol" << endl;
	}
	
	cout << "So far so good!" << endl;
}
