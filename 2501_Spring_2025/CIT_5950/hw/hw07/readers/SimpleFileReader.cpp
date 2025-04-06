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
#include <array>
//#include <iostream>

#include "./SimpleFileReader.hpp"

using std::optional;
using std::string;
using std::array;
using std::nullopt;

SimpleFileReader::SimpleFileReader(const string& fname) {
	this->fd_ = -1;
	this->good_ = false;
	this->open_file(fname);
}

SimpleFileReader::~SimpleFileReader() {
	this->close_file();
}


void SimpleFileReader::open_file(const string& fname) {
	if (this->fd_ >= 0) {
		close(this->fd_);
		this->good_ = false;
	}
	this->fd_ = open(fname.data(), O_RDONLY);
	if (this->fd_ >= 0) {
		this->good_ = true;
	}
}


void SimpleFileReader::close_file() {
	if (this->fd_ >= 0) {
		close(this->fd_);
		this->good_ = false;
	}
}


char SimpleFileReader::get_char() {
	if (!this->good()) {
		return EOF;
	}
	array<char, 1> buf{};
	ssize_t ret = read(this->fd_, buf.data(), 1);
	if (ret == 1) {
		return buf.at(0);
	}

	if (this->fd_ < 0 || lseek(this->fd_, 0, SEEK_CUR) >= lseek(this->fd_, 0, SEEK_END)){
		this->good_ = false;
	} 
	return EOF;
}


optional<string> SimpleFileReader::get_chars(size_t n) {
	if (!this->good()) {
		return nullopt;
	}

	string buf(n, '\0');

	ssize_t ret = read(this->fd_, buf.data(), n);
	//std::cout << "ret length : " << ret << std::endl;
	if (ret == 0) {
		this->good_ = false;
		return nullopt;
	} else if (ret < n) {
		this->good_ = false;
	}

	buf.resize(ret);
		
	return buf;
}


int SimpleFileReader::tell() const {
	if (this->fd_ < 0) {
		return -1;
	}
	return lseek(this->fd_, 0, SEEK_CUR);	
}


void SimpleFileReader::rewind() {
	if (this->fd_ < 0) {
		return;
	}
	this->good_ = true;
	lseek(this->fd_, 0, SEEK_SET);
}


bool SimpleFileReader::good() const {
	return this->good_;
}


