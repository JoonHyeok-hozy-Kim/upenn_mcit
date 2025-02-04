#include "./simple_string.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

simple_string::simple_string() {
  // TODO: finish the zero arg constructor
  data_ = new char[1];
  data_[0] = '\0';
  len_ = 0;
}


// TODO: the other functions
simple_string::simple_string(const char* cstring) {
	size_t new_len = 0;
	const char* ch = cstring;
	while (*ch != '\0') {
		ch++;
		new_len++;
	}
	
	data_ = new char[new_len + 1];
	len_ = new_len;
	for (size_t i=0; i<len_+1; i++){
		data_[i] = cstring[i];
	}			
}


simple_string::simple_string(const simple_string& other){
	len_ = other.len();
	data_ = new char[len_+1];
	const char* oo = other.data();
	for (size_t i=0; i<len_+1; i++){
		data_[i] = oo[i];
	}
}

simple_string::~simple_string(){
	// cout << "[dtor] " << data_ << endl;
	delete[] data_;
}


const char* simple_string::data() const {
	const char* data_copy = data_;
	return data_copy;
}


size_t simple_string::len() const {
	return len_;
}


simple_string simple_string::substr(size_t start_index, size_t length) const {
	if (start_index >= len_){
		throw out_of_range("[Error] Start index larger than the length.");
	}	

	size_t sub_len;
	if (length == simple_string::npos){
		sub_len = len_;
	} else { 
		sub_len = length; 
	} 

	if (start_index + sub_len > len_) {
		sub_len -= (start_index + sub_len) - len_;
	}
	
	char* sub = new char[sub_len+1];
	for (size_t i=0; i<sub_len; i++){
		sub[i] = data_[start_index + i];
	}
	sub[sub_len] = '\0';

	simple_string result = simple_string(sub);
	delete[] sub;
	return result;
}

bool simple_string::operator==(const simple_string& other) const {
	if (len_ != other.len()){
		return false;
	}

	for (size_t i=0; i<len_; i++){
		if (data_[i] != other.data()[i]) {
			return false;
		}
	}
	return true;
}


char& simple_string::at(size_t index) {
	if (len_ == simple_string::npos || index < 0 || index >= len_){
		throw out_of_range("[Error] Index out of range");
	}
	return data_[index];
}


char& simple_string::operator[](size_t index) {
	if (len_ == simple_string::npos || index < 0 || index >= len_){
		throw out_of_range("[Error] Index out of range");
	}

	return data_[index];
}



size_t simple_string::find(char target) const {
	for (size_t i=0; i<len_; i++){
		if (data_[i] == target){
			return i;
		}
	}
	return simple_string::npos;
}



size_t simple_string::find_first_of(const simple_string& chars, size_t pos) const {
	if (pos < 0 || pos >= len_) {
		throw out_of_range("[Error] Index out of range");
	}

	while (pos < len_){
		const char* char_data = chars.data();
		while (*char_data != '\0'){
			if (data_[pos] == *char_data){
				return pos;
			}
			char_data++;
		
		//for (const char& c: chars.data()){
		//	if (data_[pos] == c){
		//		return pos;
		//	}
		}		
		pos++;
	}
	return simple_string::npos;
}



vector<simple_string> simple_string::split(const simple_string& delims) const {
	vector<simple_string> result {};
	size_t prev = 0, curr;

	while (prev < len_){
		curr = find_first_of(delims, prev);
		if ( curr == simple_string::npos ) {
			break;
		}
		
		if (prev < curr) {
			result.push_back(substr(prev, curr-prev));
		}

		prev = curr + 1;
	}
	
	if (prev < len_) {
		result.push_back(substr(prev, len_-prev));
	}

	return result;	
}


