#include <iostream>

using namespace std;

void call_itself(string func_name) {
	cout << "\n\n" << func_name << endl;
}

void static_cast_test() {
	call_itself(__func__);
	double pi = 3.14159;
	int rounded_pi = static_cast<int>(pi);

	cout << "pi : " << pi << endl; 
	cout << "rounded_pi : " << rounded_pi << endl; 

	return;
}

class Base {virtual void foo() {} };
class Derived: public Base {};

void dynamic_cast_test() {
	call_itself(__func__);


	Base* base_ptr = new Derived;
	Derived* derived_ptr = dynamic_cast<Derived*>(base_ptr);
}


void const_cast_test() {
	call_itself(__func__);

	const int c = 10;
	int* nc_ptr = const_cast<int*>(&c);
	cout << "c : " << c << "\t " << &c << endl;
	cout << "*nc_ptr : " << *nc_ptr << "\t " << nc_ptr << endl;

	*nc_ptr = 20;
	cout << "c : " << c << "\t " << &c << endl;
	cout << "*nc_ptr : " << *nc_ptr << "\t " << nc_ptr << endl;


}


void reinterpret_cast_test() {
	call_itself(__func__);

	int num = 42;
	char* char_ptr = reinterpret_cast<char*>(&num);

	cout << "num : " << num << endl;
	cout << "*char_ptr : " << *char_ptr << endl;

}


int main() {
	static_cast_test();
	dynamic_cast_test(); 
	const_cast_test();
	reinterpret_cast_test();

	return 0;
}
