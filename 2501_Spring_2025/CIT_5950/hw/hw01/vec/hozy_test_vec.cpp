#include <iostream>
#include "vec.hpp"

using namespace std;

void vec_printer(vec& v){
	cout << "[";
	for (size_t i=0; i<v.length(); i++){
		cout << v[i];
		if (i < v.length()-1){
			cout << ", ";
		}
	}
	cout << "]";
	return;
}

void vec_stat_printer(vec& v){
	cout << "[vector object summary]" << endl;
	cout << "length   : " << v.length() << endl;
	cout << "capacity : " << v.capacity() << endl;
	cout << "vector : ";
	vec_printer(v);
	cout << endl;
}

int main(int argc, char** argv){
	// Constructor 0-arg Test
	vec v = vec();	
	//vec_stat_printer(v);
	
	// Constructor 1-arg Test
	vec v3 = vec(3);
	//vec_stat_printer(v3);

	// cout << v[0] << endl;	// Error!
	v.push_back("3");
	v.push_back("4");
	v.push_back("5");
	v.push_back(to_string(6));
	//vec_stat_printer(v);	
	
	vec v2 = vec();
	for (int i=0; i<11; i++){
		v2.push_back(to_string(i+100));
	}
	//vec_stat_printer(v2);
	
	for (int i=0; i<7; i++){
		v2.pop_back();
		vec_stat_printer(v2);
	}


	v2.insert(2, "x");
	v2.insert(2, "y");
	v2.insert(v2.length(), "z");
	//v2.insert(v2.length()+1, "w");
	vec_stat_printer(v2);
	
	for (int i=0; i<8; i++){
		v2.erase(0);
		vec_stat_printer(v2);
	}
}
