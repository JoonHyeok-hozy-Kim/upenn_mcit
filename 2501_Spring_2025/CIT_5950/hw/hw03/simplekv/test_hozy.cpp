#include "catch.hpp"
#include "SimpleKV.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using simplekv::SimpleKV;
using simplekv::value_type_info;
using namespace std;

TEST_CASE("Construction & Destruction", "[basic]") {
	auto kv = new SimpleKV();
	REQUIRE(kv != nullptr);
	delete kv;
}

TEST_CASE("sset/sget string value") {
	SimpleKV kv {};
	kv.sset("n1","k1","v");
	kv.sset("n1","k2","x");
	kv.sset("n1","k2","v");
	kv.sset("n2","k1","v");
	
	for (string nspace: kv.namespaces()){
//		cout << "namespace : " << nspace << endl;
		for (string key: kv.keys(nspace)){
//			cout << "key : " << key << endl;
			REQUIRE(kv.sget(nspace, key) == "v");
		}
	}
}

TEST_CASE("vector insert test"){
	vector<int> v{};
	for (int i=0; i<3; i++){
		v.insert(v.begin(), i);
	}

	REQUIRE(v[0] == 2);
	REQUIRE(v[1] == 1);
	REQUIRE(v[2] == 0);

}

TEST_CASE("lpush, llen, members, lset, lpop test") {
	SimpleKV kv {};
	int n = 3;
	for (int i=0; i<n; i++){
		REQUIRE(kv.lpush("n1","k1",to_string(i)) == true);
	}

	auto M = kv.lmembers("n1", "k1");
	REQUIRE(M.has_value() == true);
	if (M.has_value()){
		auto MM = M.value();

		REQUIRE(MM.size() == n);	
	}

	kv.lset("n1", "k1", 1, "300");
	M = kv.lmembers("n1", "k1");
	REQUIRE(M.value()[1] == "300");

	auto popped = kv.lpop("n1", "k1");
	REQUIRE(popped.value() == "2");
	M = kv.lmembers("n1", "k1");
	REQUIRE(M.value().size() == 2);


}

TEST_CASE("rpush, rpop test") {
	SimpleKV kv {};
	int n = 4;
	for (int i=0; i<n; i++){
		REQUIRE(kv.rpush("n1","k1",to_string(i)) == true);
	}

	auto LL = kv.lmembers("n1", "k1");
	REQUIRE(LL.value()[0] == "0");

	kv.rpush("n1", "k1", "1000");
	LL = kv.lmembers("n1", "k1");
	REQUIRE(LL.value()[4] == "1000");
	
	auto popped = kv.rpop("n1", "k1");
	REQUIRE(popped.value() == "1000");
}

bool set_checker(vector<string> v1, vector<string> v2){
	if (v1.size() != v2.size()){
		return false;
	}
	
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	
	for (size_t i=0; i<v1.size(); i++){
		if (v1[i] != v2[i]){
			return false;
		}
	}

	return true;
}

TEST_CASE("lunion, linter, ldiff test"){
	SimpleKV kv {};
	kv.rpush("n1","k1","a");
	kv.rpush("n1","k1","b");
	kv.rpush("n1","k1","c");

	kv.lpush("n2","k2","x");
	kv.lpush("n2","k2","b");
	kv.lpush("n2","k2","z");

	auto v1 = kv.lmembers("n1","k1").value();	
	auto v2 = kv.lmembers("n2","k2").value();

	auto lunion_test = kv.lunion("n1","k1","n2","k2").value();
	vector<string> lunion_answer {"a","b","c","x","z"};

	REQUIRE(set_checker(lunion_test, lunion_answer) == true);

	auto linter_test = kv.linter("n1","k1","n2","k2").value();
	vector<string> linter_answer {"b"};
	REQUIRE(set_checker(linter_test, linter_answer) == true);	

	auto ldiff_test = kv.ldiff("n1","k1","n2","k2").value();
	vector<string> ldiff_answer {"a","c"};
	REQUIRE(set_checker(ldiff_test, ldiff_answer) == true);




}
