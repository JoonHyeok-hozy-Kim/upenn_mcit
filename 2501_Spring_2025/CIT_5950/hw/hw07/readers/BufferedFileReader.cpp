/*
 * Copyright ©2025 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIT 5950 for use solely during Spring Semester 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <unordered_set>

#include "BufferedFileReader.hpp"

using std::optional;
using std::string;
using std::nullopt;
using std::cout;
using std::endl;
using std::unordered_set;

BufferedFileReader::BufferedFileReader(const std::string& fname)
	: curr_length_(-1),
	  curr_index_(-1),
	  fd_(-1),
	  good_(false) {
	this->open_file(fname);
}


BufferedFileReader::~BufferedFileReader() {
	if (this->good()) {
		this->close_file();
	}
	//cout << "dtor" << endl;
}


BufferedFileReader::BufferedFileReader(BufferedFileReader&& other) 
	: curr_length_(other.curr_length_), 
	  curr_index_(other.curr_index_), 
	  buffer_(std::move(other.buffer_)), 
	  fd_(other.fd_), 
	  good_(other.good_) {
	  
	//cout << "move, fd : " << this->fd_ << endl;
	other.curr_length_ = -1;
	other.curr_index_ = -1;
	other.fd_ = -1;
	other.good_ = false;
}


BufferedFileReader& BufferedFileReader::operator=(BufferedFileReader&& other) {
	//cout << "opeator=" << endl;
	//cout << "this->fd_ : " << this->fd_ << " / other.fd_ : " << other.fd_ << endl;
	//cout << "this != other : " << (this != &other) << endl;
	if (this != &other) {
		this->curr_length_ = other.curr_length_;	
		this->curr_index_ = other.curr_index_;
		this->fd_ = other.fd_;
		this->good_ = other.good_;
		this->buffer_ = std::move(other.buffer_);
	}
	//cout << "this->fd_ : " << this->fd_ << " / other.fd_ : " << other.fd_ << endl;

	other.curr_length_ = -1;
	other.curr_index_ = -1;
	other.fd_ = -1;
	other.good_ = false;
		
	return *this;
}


void BufferedFileReader::open_file(const std::string& fname) {
	if (this->good()) {
		this->close_file();
		this->good_ = false;
	}

	this->fd_ = open(fname.data(), O_RDONLY);
	
	if (this->fd_ >= 0) {
		this->good_ = true;
		//cout << "file opened" << endl;
		//off_t end_offset = lseek(this->fd_, 0, SEEK_END);
		//cout << "EOF offset : " << end_offset << endl;
	}
}


void BufferedFileReader::close_file() {
	close(this->fd_);
	this->fd_ = -1;
	this->good_ = false;
}


void BufferedFileReader::fill_buffer(){
	//cout << "fill_buffer() called" << endl;
	if (!this->good()) {
		return;
	}

	ssize_t ret = read(this->fd_, this->buffer_.data(), this->BUF_SIZE);
	//cout << "ret : " << ret << endl;
	if (ret <= 0) {
		this->good_ = false;
		return;
	}

	this->curr_length_ = ret;
	this->curr_index_ = 0;

	size_t eof_idx = 0;
	while (eof_idx < ret) {
		char curr_char = this->buffer_.at(eof_idx);
		//if (this->buffer_.at(eof_idx) == EOF) {
		//if (curr_char == '\0' || curr_char == EOF) {
		//if (curr_char == '\0') { 
		if (curr_char == EOF) {
			//cout << "EOF found at " << eof_idx << endl;
			break;
		}
		//cout << "buffer.at(" << eof_idx << ") : " << this->buffer_.at(eof_idx) << endl;
		eof_idx++;
	}

	if (eof_idx < BUF_SIZE) {
		this->curr_length_ = eof_idx;
	}
	
	/*
	cout << "curr_length_ : " << this->curr_length_ << endl;
	cout << "Buffer filled : [ ";
	for (size_t idx=0; idx<this->curr_length_; idx++) {
		cout << this->buffer_.at(idx) << ",";
	}
	if (this->curr_length_ < BUF_SIZE) {
		cout << "<EMPTY BUFFER> ";
	}

	cout << "]" << endl;
	*/	
}


char BufferedFileReader::get_char() {
	if (!this->good()) {
		return EOF;
	}

	if (this->curr_index_ == this->curr_length_) {
		this->fill_buffer();
	}

	//cout << "get_char(), curr_index_ : " << this->curr_index_ << ", curr_length_ : " << curr_length_ << endl;
	if (!this->good()) {
		return EOF;
	}
	return this->buffer_.at(this->curr_index_++);
}


bool contains_delim(unordered_set<char>& delim_set, char chr) {
	if (delim_set.contains(chr)) {
		return true;
	}
	return false;
}

std::optional<std::string> BufferedFileReader::get_token(const std::string& delims) {
	if (!this->good()){ 
		return nullopt;
	}

	string res;
	res.reserve(10);
	unordered_set<char> delim_set(delims.begin(), delims.end());

	while (this->good()) {
		char curr_char = this->get_char();
		if (curr_char == EOF) {
			break;
		} else if (contains_delim(delim_set, curr_char)) {
	       		break; 
		}
		res.push_back(curr_char);
	}

	return res;
}


int BufferedFileReader::tell() const {
	if (this->fd_ < 0) {
		return -1;
	}
	return lseek(this->fd_, 0, SEEK_CUR) - this->curr_length_ + this->curr_index_;
	//return lseek(this->fd_, 0, SEEK_CUR);
}


void BufferedFileReader::rewind() {
	if (this->fd_ < 0) {
		return;
	}

	this->good_ = true;
	lseek(this->fd_, 0, SEEK_SET);
	this->curr_length_ = -1;
	this->curr_index_ = -1;
}


bool BufferedFileReader::good() const {
	return this->good_;
}


// one provided function since this one has funky syntax
// it is just a wrapper around the good function though.
BufferedFileReader::operator bool() const {
  return this->good();
}
