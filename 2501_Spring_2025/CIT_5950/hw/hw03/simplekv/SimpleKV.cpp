#include "./SimpleKV.hpp"

using namespace std;
using list = vector<string>;

namespace simplekv {

// General Operations
vector<string> SimpleKV::namespaces() {
  vector<string> res {};

  if (!simple_namespace.empty()){
	  for (auto [key, value]: simple_namespace){
	  	res.push_back(key);
  	  };
  }

  return res;
}


vector<string> SimpleKV::keys(const string& nspace) {
  vector<string> res {};

  if (simple_namespace.contains(nspace)){
	  for (pair<string, variant<string, list>> kv_pair: simple_namespace[nspace]){
		res.push_back(kv_pair.first);
  	  }
  }

  return res;
}


bool SimpleKV::ns_exists(const string& nspace) {
  return simple_namespace.contains(nspace);
}


bool SimpleKV::key_exists(const string& nspace, const string& key) {
  if (!ns_exists(nspace)) {
	  return false;
  }
  return simple_namespace[nspace].contains(key);
}


value_type_info SimpleKV::type(const std::string& nspace,
                               const std::string& key) {
  if (key_exists(nspace, key)){
        auto var = simple_namespace[nspace][key];
	if (holds_alternative<string>(var)){
	  	return value_type_info::string;
	} 
	return value_type_info::list;
	
  }
  return value_type_info::none;
}


bool SimpleKV::del(const string& nspace, const string& key) {
  if (key_exists(nspace, key)){
	simple_namespace[nspace].erase(key);
	if (simple_namespace[nspace].empty()){
		simple_namespace.erase(nspace);
	}
	return true;
  }
  return false;
}


// string operations
optional<string> SimpleKV::sget(const string& nspace, const string& key) {
  if (key_exists(nspace, key)){
	auto var = simple_namespace[nspace][key];
	if (holds_alternative<string>(var)){
		return get<string>(var);
	}
  }
  return nullopt;
}


void SimpleKV::sset(const string& nspace,
                    const string& key,
                    const string& value) {
	if (!ns_exists(nspace)){
		simple_namespace[nspace] = unordered_map<string, variant<string, list>> {{key, value}};
	} else {
		simple_namespace[nspace][key] = value;
	}	
}


// list operations
ssize_t SimpleKV::llen(const string& nspace, const string& key) {
  auto var = simple_namespace[nspace][key];
  if (holds_alternative<list>(var)){
	  return (ssize_t) get<list>(var).size();
  }
  return -1;
}


bool SimpleKV::validate_idx(const string& nspace,
		const string& key,
		size_t idx){
	return idx >= 0 && idx < llen(nspace, key);
}


optional<string> SimpleKV::lindex(const string& nspace,
                                  const string& key,
                                  size_t index) {
  if (key_exists(nspace, key) && validate_idx(nspace, key, index)){
	  auto var = simple_namespace[nspace][key];
	  // Not using get_if<list>(var), because we are returning nulopt!
	  if (holds_alternative<list>(var)){
		  return get<list>(var)[index];
	  }
  }

  return nullopt;
}

optional<vector<string>> SimpleKV::lmembers(const string& nspace,
                                            const string& key) {
  if (key_exists(nspace, key)){
	  auto var = simple_namespace[nspace][key];
	  if (holds_alternative<list>(var)){
		  vector<string> res {};
		  auto curr_vector = get<list>(var);
		  for (string val: curr_vector){
			  res.push_back(val);
		  }
		  return res;
	  }
  }
		  	
  return nullopt;
}

bool SimpleKV::lset(const string& nspace,
                    const string& key,
                    size_t index,
                    const string& value) {
  if (key_exists(nspace, key) && validate_idx(nspace, key, index)) {
	  auto& var = simple_namespace[nspace][key];
	  if (holds_alternative<list>(var)){
		  list& curr_list = get<list>(var);
		  curr_list[index] = value;
                  return true;
          }
	
  }

  return false;
}

bool SimpleKV::lpush(const string& nspace,
                     const string& key,
                     const string& value) {
  if (!key_exists(nspace, key)){
          simple_namespace[nspace][key] = list{};
  }

  auto& var = simple_namespace[nspace][key]; // To modify list in varinat, it should be a ref.!!!
  if (holds_alternative<list>(var)){
	  auto& curr_list = get<list>(var);
	  curr_list.insert(curr_list.begin(), value);
	  return true;
  }

  return false;
}


optional<string> SimpleKV::lpop(const string& nspace, const string& key) {
  if (key_exists(nspace, key) && llen(nspace, key) > 0){
          auto& var = simple_namespace[nspace][key];
          if (holds_alternative<list>(var)){
                  auto& curr_list = get<list>(var);
		  string res = curr_list.front(); // Copy value for the return.
                  curr_list.erase(curr_list.begin());

		  if (curr_list.empty()){
			del(nspace, key);	
		  } 
                  return res;
          }
  }

  return nullopt;
}


bool SimpleKV::rpush(const string& nspace,
                     const string& key,
                     const string& value) {
  if (!key_exists(nspace, key)){
	  simple_namespace[nspace][key] = list {};
  }

  auto& var = simple_namespace[nspace][key];
  if (holds_alternative<list>(var)){
	  list& curr_list = get<list>(var);
	  curr_list.push_back(value);
          return true;
  }

  return false;
}


optional<string> SimpleKV::rpop(const string& nspace, const string& key) {
  if (key_exists(nspace, key) && llen(nspace, key) > 0){
          auto& var = simple_namespace[nspace][key];
          if (holds_alternative<list>(var)){
                  list& curr_list = get<list>(var);
                  string res = curr_list.back();
                  curr_list.pop_back();

		  if (curr_list.empty()){
			del(nspace, key);
		  } 

                  return res;
          }

  }
  return nullopt;
}

optional<vector<string>> SimpleKV::lunion(const string& nspace1,
                                          const string& key1,
                                          const string& nspace2,
                                          const string& key2) {
  if (type(nspace1, key1) == value_type_info::string || type(nspace2, key2) == value_type_info::string) {
	  return nullopt;
  }

  unordered_set<string> temp_set {};
  if (key_exists(nspace1, key1) && llen(nspace1, key1) > 0){
	auto lmembers1 = lmembers(nspace1, key1);
	if (lmembers1.has_value()){
		for (string val: lmembers1.value()){
			temp_set.insert(val);
		}
	}
  }

  if (key_exists(nspace2, key2) && llen(nspace2, key2) > 0){
	auto lmembers2 = lmembers(nspace2, key2);
	if (lmembers2.has_value()){
		for (string val: lmembers2.value()){
			temp_set.insert(val);
		}
	}
  }

  if (!temp_set.empty()){
	vector<string> res {};
	for (string val: temp_set){
	       res.push_back(val);	
	}
	return res;
  }

  return nullopt;
}

optional<vector<string>> SimpleKV::linter(const string& nspace1,
                                          const string& key1,
                                          const string& nspace2,
                                          const string& key2) {
  if (type(nspace1, key1) == value_type_info::string || type(nspace2, key2) == value_type_info::string) {
	  return nullopt;
  }

  unordered_set<string> temp_set {};
  vector<string> res {};

  if (key_exists(nspace1, key1) && llen(nspace1, key1) > 0){
	auto lmembers1 = lmembers(nspace1, key1);
	if (lmembers1.has_value()){
		for (string val: lmembers1.value()){
			if (!temp_set.contains(val)){
				temp_set.insert(val);
			}
		}
	}
  }

  if (!temp_set.empty() && key_exists(nspace2, key2) && llen(nspace2, key2) > 0){
	auto lmembers2 = lmembers(nspace2, key2);
	if (lmembers2.has_value()){
		for (string val: lmembers2.value()){
			if (temp_set.contains(val)){
				res.push_back(val);
				temp_set.erase(val);
			}
		}
	}
  }
  
  if (!res.empty()){
	  return res;
  }

  return nullopt;
}

optional<vector<string>> SimpleKV::ldiff(const string& nspace1,
                                         const string& key1,
                                         const string& nspace2,
                                         const string& key2) {
  if (type(nspace1, key1) == value_type_info::string || type(nspace2, key2) == value_type_info::string) {
	  return nullopt;
  }


  unordered_set<string> temp_set {};
  vector<string> res {};

  if (key_exists(nspace1, key1) && llen(nspace1, key1) > 0){
	auto lmembers1 = lmembers(nspace1, key1);
	if (lmembers1.has_value()){
		for (string val: lmembers1.value()){
			temp_set.insert(val);
		}
	}
  } 
  
  

  if (key_exists(nspace2, key2) && llen(nspace2, key2) > 0){
	auto lmembers2 = lmembers(nspace2, key2);
	if (lmembers2.has_value()){
		for (string val: lmembers2.value()){
			if (temp_set.contains(val)){
				temp_set.erase(val);	
			}
		}
	}
  }
  if (!temp_set.empty()) {
	for (string val: temp_set){
		res.push_back(val);
	}
}

  return res;
}

}  // namespace simplekv
