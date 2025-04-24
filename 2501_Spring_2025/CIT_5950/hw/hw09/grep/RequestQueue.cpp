#include "./RequestQueue.hpp"
#include <pthread.h>
#include <optional>
#include <iostream>

using std::optional;
using std::nullopt;
using std::cout;
using std::endl;


RequestQueue::RequestQueue() {
	this->head = nullptr;
	this->closed = false;
	pthread_mutex_init(&this->lock, nullptr);
	pthread_cond_init(&this->cond, nullptr);
}

RequestQueue::~RequestQueue() {
	QueueNode* curr = this->head;
	QueueNode* next = nullptr;

	while (curr != nullptr) {
		next = curr->next;
		delete curr;
		curr = next;
	}
}

bool RequestQueue::add(grep_request val) {
	pthread_mutex_lock(&this->lock); // lock
	if (this->closed) {
		pthread_mutex_unlock(&this->lock); // unlock
		return false;
	}

	QueueNode* new_node = new QueueNode();
	new_node->next = nullptr;
	new_node->value = val;


	if (this->length_unsafe() == 0) {
		this->head = new_node;
	} else {
		QueueNode* curr = this->head;
		while (curr->next != nullptr) {
			curr = curr->next;
		}
		curr->next = new_node;
	}

	//this->broadcast();
	pthread_cond_signal(&this->cond);
	pthread_mutex_unlock(&this->lock); // unlock

	return true;
}

void RequestQueue::close() {
	pthread_mutex_lock(&this->lock); // lock
	this->closed = true;

	//this->broadcast(); // broadcast all waiting threads.
	pthread_cond_signal(&this->cond);
	pthread_mutex_unlock(&this->lock); // unlock
}

optional<grep_request> RequestQueue::remove() {
	if (this->length_unsafe() == 0) {
		return nullopt;
	}

	pthread_mutex_lock(&this->lock); // lock
	QueueNode* prev = this->head;
	grep_request res = prev->value;
	this->head = prev->next;	
	delete prev;
	pthread_mutex_unlock(&this->lock); // unlock
	return res;
}

optional<grep_request> RequestQueue::wait_remove() {
	
	pthread_mutex_lock(&this->lock); // lock

	while (this->length_unsafe() == 0 && !this->closed) { 
		//cout << "RequestQueue::wait_remove() wait!" << endl;
		pthread_cond_wait(&this->cond, &this->lock); // wait if no request
	}

	if (this->length_unsafe() == 0 && this->closed) {
		//cout << "RequestQueue::wait_remove() return nullopt" << endl;
		pthread_mutex_unlock(&this->lock); // unlock;
		return nullopt;
	} 

	QueueNode* prev = this->head;
	grep_request res = prev->value;
	this->head = prev->next;	
	delete prev;
	pthread_mutex_unlock(&this->lock); // unlock

	//cout << "RequestQueue::wait_remove() wake and return " << res.fname << endl;
	return res;
}

int RequestQueue::length() {
	int res = 0;

	pthread_mutex_lock(&this->lock); // lock
	QueueNode* curr = this->head;
	while (curr != nullptr) {
		++res;
		curr = curr->next;
	}
	pthread_mutex_unlock(&this->lock); // unlock

	//cout << "RequestQueue::length()" << endl;
	return res;
}

int RequestQueue::length_unsafe() {
	int res = 0;

	QueueNode* curr = this->head;
	while (curr != nullptr) {
		++res;
		curr = curr->next;
	}

	return res;
}

void RequestQueue::broadcast() {
	//cout << "RequestQueue::add broadcast!" << endl;
	pthread_cond_broadcast(&this->cond); // wake all waiting threads!
}
