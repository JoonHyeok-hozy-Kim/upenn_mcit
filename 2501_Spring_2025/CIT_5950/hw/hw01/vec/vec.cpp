#include <stdexcept>

// TODO: add the include for corresponding hpp
#include "vec.hpp"

using namespace std;

vec::vec() {
 // TODO: finish the 0-argument constructor
 length_ = 0;
 capacity_ = default_capacity;
 data_ = new string[capacity_];
} 

// TODO: finish the rest
vec::vec(size_t initial_capacity) {
	length_ = 0;
	capacity_ = initial_capacity;
	data_ = new string[capacity_];
}


vec::vec(const vec& other) {
	length_ = other.length_;
        capacity_ = other.capacity_;
	data_ = new string[capacity_];
	for (size_t i=0; i<length_; i++){
		data_[i] = other.data_[i];
	}
}


vec::~vec() {
	delete[] data_;
}


size_t vec::length() {
	return length_;
}


std::string* vec::data() {
	return data_;	
}


size_t vec::capacity() {
	return capacity_;
}


void vec::reserve(size_t new_cap) {
	if (new_cap < capacity_){
		return;
	}

	string* new_data = new string[new_cap];
	for (size_t i=0; i<length_; i++){
		new_data[i] = data_[i];
	}
	delete[] data_;
	data_ = new_data;
	capacity_ = new_cap;
}


void vec::push_back(const std::string& element) {
	if (length_ == capacity_){
		capacity_ *= 2;
		reserve(capacity_);
	}
	data_[length_++] = element;
}


std::optional<std::string> vec::pop_back() {
	if (length_ == 0){
		return nullopt;
	}
	optional<string> result = at(length_-1);
	data_[length_-1] = "";
	length_--;

	/*if (length_ < capacity_ / 2) {
		capacity_ /= 2;
		string* new_data = new string[capacity_];
		for (size_t i=0; i<length_; i++){
			new_data[i] = data_[i];
		}
		delete[] data_;
		data_ = new_data;
	}*/
	return result;
}

std::string& vec::operator[](size_t index) {
	if (index >= length_){
		throw out_of_range("Index out of range error");
	}

	return data_[index];
}


std::string& vec::at(size_t index) {
	if (index >= length_){
		throw out_of_range("Index out of range error");
	}

	return data_[index];
}


size_t vec::insert(size_t index, const std::string& element) {
	if (index < 0 || index > length_){
		throw out_of_range("Index out of range error");
	}
	if (length_ == capacity_){
		capacity_ *= 2;
		reserve(capacity_);
	}
	if (index == length_){
		push_back(element);
	} else {
		length_++;
		string* new_data = new string[capacity_];
		for (size_t i=0; i<length_; i++){
			if (i == index){
				new_data[i] = element;
			} else if (i < index) {
				new_data[i] = data_[i];
			} else {
				new_data[i] = data_[i-1];
			}
		}			
		delete[] data_;
		data_ = new_data;
	}
	return index;		
}


size_t vec::erase(size_t index) {
	if (index < 0 || index >= length_){
		throw out_of_range("Index out of range error");
	}

	if (length_-1 == index) {
		pop_back();
		return length_-1;
	} else {
		for (size_t i=0; i<length_; i++){
			if (i >= index){
				data_[i] = data_[i+1];
			}
		}
		length_--;
	}
	return index;
}



