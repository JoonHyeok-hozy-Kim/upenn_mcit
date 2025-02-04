#include <iostream>
#include <cstring>
#include <cstdlib>
#include "./simple_string.hpp"
using namespace std;

int main(){
	// ctor 1
	simple_string S = simple_string();
	cout << S.len() << endl;

	// ctor 2
	simple_string S2 = simple_string("abcde");
	cout << S2.len() << " / " << S2.data() << endl;

	// ctor 3
	simple_string S3 = simple_string(S2);
	cout << S3.len() << " / " << S3.data() << endl;

	// substr tests
//	simple_string sub1 = S2.substr(5, 3);
	simple_string sub2 = S2.substr(2, 3);
	cout << sub2.len() <<  " / " << sub2.data() << endl;
	simple_string sub3 = S2.substr(2, 10);
	cout << sub3.len() << " / " << sub3.data() << endl;

	cout << (S2 == S3) << endl;
	cout << (S2 == sub2) << endl;

//	cout << S2.at(-3) << S2.at(100) << endl;
	cout << S2.at(0) << endl;
	cout << S2[0] << endl;

	cout << S2.find('c') << " / " << S2.find('z') << endl;

	cout << S2.find_first_of("cd", 0) << endl;
	cout << S2.find_first_of("cd", 3) << endl;
	cout << S2.find_first_of("cd", 4) << endl;

	
	vector<simple_string> splt1 = S2.split("ac");
	for (simple_string& ss: splt1){
		cout << ss.data() << endl;
	}
	
	vector<simple_string> splt2 = S2.split("be");
	for (auto& ss: splt2){
		cout << ss.data() << endl;
	}

	char* s1 = strdup("Seamus");
	simple_string ss1(s1);

	cout << ss1.len() << endl;
	
}
