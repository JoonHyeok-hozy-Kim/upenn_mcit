#include "simple_string.hpp"

simple_string from_cstring(const char* cstring) {
	simple_string result;
	const char* src = cstring;
	size_t cstring_len = 0;

	while (*src != '\0'){
		cstring_len++;
		src++;
	}
	result.len = cstring_len;

	src = cstring;
	char* data = new char[cstring_len + 1]; 
	char* dest = data;
	for (size_t i=0; i<cstring_len+1; i++){
		*dest = *src;
		dest++;
		src++;
	}
	result.data = data;

	return result;
}

simple_string duplicate(const simple_string to_copy){
	simple_string result;
	result.len = to_copy.len;

	char* data_copy = new char[to_copy.len + 1];
	char* src_data = to_copy.data;
	char* dest = data_copy;
	while (*src_data != '\0'){
		*dest = *src_data;	
		dest++;
		src_data++;
	}
	*dest = *src_data;
	result.data = data_copy;

	return result;
}

simple_string substr(const simple_string self, size_t start_index, size_t length){
	simple_string result;
	size_t new_len = length;
	if (start_index + length >= self.len || length == simple_string::npos){
		new_len = self.len - start_index;
	}
	result.len = new_len;
	
	char* new_data = new char[new_len + 1];
	char* dest = new_data;
	char* src = self.data;
	src += start_index;
	for (size_t i=0; i<new_len; i++){
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	result.data = new_data;
	return result;
}


size_t find(const simple_string self, char target) {
	char* curr = self.data;
	for (size_t i=0; i<self.len; i++){
		if (*curr == target){
			return i;
		}
		curr++;
	}
	return simple_string::npos;
		
}

bool operator==(const simple_string lhs, const simple_string rhs){
	if (lhs.len != rhs.len){
		return false;
	}
	char* ll = lhs.data;
	char* rr = rhs.data;
	while (*ll != '\0'){
		if (*ll != *rr){
			return false;
		}
		ll++;
		rr++;
	}
	return true;
}

char at(const simple_string self, size_t index){
	size_t jdx = 0;
	char* curr = self.data;
	while (jdx < index){
		curr++;
		jdx++;
	}
	return *curr;
}

void destruct(simple_string to_destroy){
	delete[] to_destroy.data;
	return;
}

std::ostream& operator<<(std::ostream& os, const simple_string to_print){
	os << "<simple_string> " << to_print.data << std::endl;
	return os;
}


