#include <iostream>
#include "./hash_table.hpp"

using namespace std;

int square_bracket_test() {
	cout << "square_bracket_test()" << endl;

	hash_table H {};

	string& h = H["a"];

	cout << "->" << h << "<-" << endl; 

	H["b"] = "32";
	cout << H["b"] << endl;

	cout << H.at("b") << endl;

	//cout << H.at("c") << endl;
	if (H.contains("b")) {
		cout << "contains 1 correct" << endl;
	} else {
		cout << "contains 1 failed" << endl;
	}

	if (!H.contains("c")) {
		cout << "contains 2 correct" << endl;
	} else {
		cout << "contains 2 failed" << endl;
	}

	string keyc = "c";
	if (!H.erase(keyc)) {
		cout << "erase 1 correct" << endl;
	} else {
		cout << "erase 1 failed" << endl;
	}

	string keyb = "b";
	if (H.erase(keyb) && !H.contains("b")) {
		cout << "erase 2 correct" << endl;
	} else {
		cout << "erase 2 failed" << endl;
	}




	return EXIT_SUCCESS;
}

void iterator_test() {
	hash_table H = hash_table(2);

	for (char c = 'A'; c!='z'+1; c++) {
		string S = "";
		S += c;
		//cout << S << endl;
		H[S] = "v";
	}

	auto it = H.begin();	
	cout << (*it).first << endl;

	auto& itt = it;	
	cout << (*itt).first << endl;

}

void at_test() {
	hash_table H {};
	for (char ch = 'A'; ch != 'z'+1; ch++) {
		string new_key = "";
		new_key += ch;
		string new_val = "";
		new_val += ch+1;
		H[new_key] = new_val;
	}

	for (char ch = 'A'; ch != 'z'+1; ch++) {
                string check_key = "";
		check_key += ch;
		string& check_val = H.at(check_key);
		cout << check_val << ", ";
	}
	cout << endl;

	//H.at("1");
	if (H.contains("a")) {
		cout << "yes" << endl;
	}
	if (!H.contains("1")) {
		cout << "yes" << endl;
	}

	string erase_key = "a";
	H.erase(erase_key);
	if (!H.contains("a")) {
		cout << "yes" << endl;
	}
}

void mimic_test() {
	int i;
	hash_table table(300);

	for (i=0; i<100; i++) {
		string key = to_string(i);
		string payload = to_string(i+1);
		table[key] = payload;
	}

	table.print_buckets();
	auto it2 = table.begin();


		
	for (i=0; i<100; i++) {
		cout << "In mimic_test(), i=" << i << endl;
		kv_pair kv;
		kv = *it2;
		cout << " < " << kv.first << ", " << kv.second << " >" << endl;

		++it2;
	}	
	

}

int main() {

	//square_bracket_test();
	//iterator_test();

	//at_test();
	mimic_test();
	return EXIT_SUCCESS;
}
