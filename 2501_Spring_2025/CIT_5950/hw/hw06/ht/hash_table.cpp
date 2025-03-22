#include "./hash_table.hpp"
#include <stdexcept>

using namespace std;

// provided for you
// just hashes the string to get a hash code and then
// modulo's by the number of buckets to decide which bucket
// it should go into
size_t hash_table::key_to_bucket_num(const string& key) {
  return hash<string>{}(key) % (this->buckets_.size());
}

/////////////////////////////////////////////////////////////////////////////
// Part 1a: Basic Accessor functions
/////////////////////////////////////////////////////////////////////////////

string& hash_table::operator[](const string& key) {
  // you should keep this code.
  // Once you implement load_factor() and rehash()
  // this resizes the hash table when tehre are too many elements
  if (this->load_factor() >= hash_table::MAX_LOAD_FACTOR) {
    this->rehash(this->buckets_.size() * 2);
  }

  // calculate which bucket this key belongs to
  size_t bucket_num = key_to_bucket_num(key);
  // get the corresponding bucket
  auto& bucket = this->buckets_.at(bucket_num);


  // TODO find the key if it exists
  // and either return the corresponding value
  // or perform the insertion
  list<kv_pair>::iterator itr = bucket.begin();
  while (itr != bucket.end()) { 
	kv_pair& curr_pair = *itr;
	if (curr_pair.first == key) {
		return curr_pair.second;
	}
	itr++;
  }
  
  kv_pair new_pair {key, ""};
  auto inserted = bucket.insert(itr, new_pair);
  this->size_++;

  //cout << "key : " << key << ", bucket_num : " << bucket_num << endl;
  return inserted->second;

}

size_t hash_table::size() const {
  return this->size_;
}

// TODO: implement the remaining hash table functions
std::string& hash_table::at(const std::string& key) {
	size_t bucket_num = key_to_bucket_num(key);
	auto& bucket = this->buckets_.at(bucket_num);
	
	//for (auto itr = bucket.begin(); itr != bucket.end(); ++itr) {
	for (auto& curr_pair: bucket) {
		if (curr_pair.first == key){
			return curr_pair.second;
		}
	}
	
	throw out_of_range("Key not found.");
}



bool hash_table::contains(const std::string& key) {
	size_t bucket_num = key_to_bucket_num(key);
	auto& bucket = this->buckets_.at(bucket_num);
	
	for (auto& curr_pair: bucket) {
		if (curr_pair.first == key){
			return true;
		}
	}
	
	return false;
}


bool hash_table::erase(std::string& key) {
	size_t bucket_num = key_to_bucket_num(key);
	auto& bucket = this->buckets_.at(bucket_num);
	auto itr = bucket.begin();
	
	while (itr != bucket.end()) {
		if ((*itr).first == key){
			bucket.erase(itr);
			this->size_--;
			return true;
		}
		itr++;
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////
// Part 1b: Resising/rehashing functions
/////////////////////////////////////////////////////////////////////////////

float hash_table::load_factor() {
  // you may find static_cast<float> useful to convert size_t
  // or other types to float. For example:
  //
  // size_t x = 5;
  // float y = static_cast<float>(x);
  float res = static_cast<float>(this->size_) / static_cast<float>(buckets_.size());
  return res;
}

void hash_table::rehash(size_t count) {
	//cout << "rehash : " << this->buckets_.size() << " -> " << count << endl;
	if (count <= this->buckets_.size()) {
		return; 
	}

	list<kv_pair> temp_list {};
	for (auto& bucket: this->buckets_) {
		temp_list.splice(temp_list.end(), bucket);
	}

	this->buckets_.resize(count);	
	
	auto itr = temp_list.begin();
	while (itr != temp_list.end()) {
		size_t bucket_num = key_to_bucket_num((*itr).first);
		auto& bucket = this->buckets_.at(bucket_num);
		bucket.splice(bucket.end(), temp_list, itr++); 
	}
	
	/*	
	cout << "--------< Rehash Print >--------------" << endl;
	int bucket_cnt = 0;
	for (auto bucket: this->buckets_) {
		cout << "[Bckt" << bucket_cnt++ << "] : ";
		for (auto pair: bucket) {
			cout << "<" << pair.first << ", " << pair.second << "> ";
		}
		cout << endl;
	}
	*/


}

/////////////////////////////////////////////////////////////////////////////
// Part 2: Implementing the iterator.
// Only need to implement:
// - iterator::operator++()  "next"
// - iterator::opeartor*()   "get"
/////////////////////////////////////////////////////////////////////////////

hash_table::iterator& hash_table::iterator::operator++() {
	hash_table::iterator& res = *this;

	if (res == ht_.end()) {
		return res;
	}

	list_iter_++;	

	// After reaching the curr_bucket's end, skip consecutive empyt buckets. 
	while (list_iter_ == ht_.buckets_[bucket_num_].end()) {
		bucket_num_++;
		if (bucket_num_ == ht_.buckets_.size()) {
			bucket_num_--;
			break;
		}
		list_iter_ = ht_.buckets_[bucket_num_].begin();
	}


	return res;
}


kv_pair& hash_table::iterator::operator*() {
	if (this->bucket_num_ == this->ht_.buckets_.size() - 1 
			&& this->list_iter_ == this->ht_.buckets_[this->bucket_num_].end()){
		throw out_of_range("End of the hash table");
	}

	kv_pair& res = *(this->list_iter_);

	return res; 
}

// provded constructor
hash_table::iterator::iterator(hash_table& table) : ht_(table) {
  if (ht_.buckets_.empty()) {
    throw invalid_argument("Trying to construct an iterator from an invalid hash table");
  }

  bucket_num_ = 0;
  for (auto& bucket : ht_.buckets_) {
    if (!bucket.empty()) {
      list_iter_ = bucket.begin();
      break;
    }
    bucket_num_ += 1;
  }

  if (bucket_num_ == ht_.buckets_.size()) {
    bucket_num_ -= 1;
    list_iter_ = ht_.buckets_.back().begin();
  }
}

// provded function
hash_table::iterator hash_table::begin() {
  //return hash_table::iterator(*this);
  return {*this};
}

// provded function
hash_table::iterator hash_table::end() {
  hash_table::iterator res(*this);

  res.bucket_num_ = this->buckets_.size() - 1;
  res.list_iter_ = this->buckets_.at(res.bucket_num_).end();
  return res;
}

// provded function
bool operator==(const hash_table::iterator& lhs, const hash_table::iterator& rhs) {
  if (&lhs.ht_ != &rhs.ht_) {
    throw invalid_argument("Cannot compare iterators to two separate hash tables");
  }

  if (lhs.bucket_num_ != rhs.bucket_num_) {
    return false;
  }

  return lhs.list_iter_ == rhs.list_iter_;
}




































