#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <optional>
#include "./grep_request.hpp"
#include "./RequestQueue.hpp"
#include "./catch.hpp"

using namespace std;

vector<string> split(const string& input) {
  vector<string> tokens;
  size_t pos = input.find_first_of(" \r\t\n");
  size_t start = 0;
  while (pos != string::npos) {
	  //cout << "start : " << start << ", pos : " << pos << endl;
	  if (pos - start > 1) {
		  tokens.push_back(input.substr(start, pos-start));
	  } 
	  start = pos+1;
	  pos = input.find_first_of(" \r\t\n", start+1);
  }

  if (start < input.size()-1) {
	  tokens.push_back(input.substr(start, input.size()-start));
  }

  return tokens;
}

void split_tester() {
	string str1 = string("abc def");

	cout << "str1:" << str1 << ":";
	for (auto word : split(str1)) {
		cout << word << ",";
	}
	cout << endl;

	str1 = string("abc def ");	
	cout << "str1:" << str1 << ":";
	for (auto word : split(str1)) {
		cout << word << ",";
	}
	cout << endl;

	str1 = string(" abc def ");	
	cout << "str1:" << str1 << ":";
	for (auto word : split(str1)) {
		cout << word << ",";
	}
	cout << endl;

	str1 = string("     abc\t\t\r def\n\n ");	
	cout << "str1:" << str1 << ":";
	for (auto word : split(str1)) {
		cout << word << ",";
	}
	cout << endl;


}

void grep(const grep_request& request) {
  std::cout << request.to_string() << std::endl;
  std::ifstream file(request.fname);
  // You probably want the above lines
  // you may want to take some inspiration from read_query
  // - how you can tread a stream like a bool in a condition
  // - using the getline function
  if (!file.is_open()) {
         cerr << "Cannot open file : " << request.fname << endl;
  }

  string line;
  while (getline(file, line)) {
                  vector<string> tokens = split(line);
                  for (auto w: tokens) {
                          cout << w << "/";
                  }
                  cout << endl;
  }


  // TODO
}

void grep_tester() {
	vector<string> Q{};
	Q.push_back("a");
	
	grep_request gr{
		"./test_files/Bye.txt",
		//const_cast<vector<string>>(Q),
		Q,
	};

	//grep(const_cast<grep_request>(Q));
	grep(const_cast<grep_request&>(gr));



}

// this function is given to you
optional<grep_request> read_query() {
  grep_request request;

  string line;
  getline(cin, line);

  while (cin) {
    // tokenize the string
    vector<string> tokens = split(line);

    // parse tokens into a query

    if (tokens.size() > 1) {
      request.fname = tokens.at(0);
      tokens.erase(tokens.begin());
      request.queries = std::move(tokens);
      return request;
    }
    cerr << "Parsing Error: expected a file name followed by a one or more";
    cerr << " tokens to search for" << endl;

    // try reading line again
    getline(cin, line);
  }

  return nullopt;
}

void read_query_test() {
	auto opt = read_query();
	while (opt.has_value()) {
		cout << opt->to_string() << endl;
		opt = read_query();
	}
}

/////////////////////////////////////////////////////////////////////////////////////

struct ThreadArg {
  RequestQueue* queue;
  int& num_read;
  int& num_write;
  pthread_mutex_t& rw_lock;
};

static void* read_grep_requests(void* queue);
static void* read_closed(void* queue);

const grep_request kOne {"one", {"k"}};
const grep_request kZero {"zero", {}};
const grep_request kNegative {"./negative", {"No", "Negative", "str"}};
const grep_request kPi {"./pi", {"Phi", "Life", "Cypher"}};
const grep_request kRs {"./RuneLite", {"In", "The", "End", "it", "doesn't", "even", "matter"}};

bool operator==(const grep_request& lhs, const grep_request& rhs) {
  if (lhs.fname != rhs.fname) {
    return false;
  }
  if (lhs.queries.size() != rhs.queries.size()) {
    return false;
  }
  for (std::size_t i = 0; i < lhs.queries.size(); i++) {
    if (lhs.queries.at(i) != rhs.queries.at(i)) {
      return false;
    }
  }
  return true;
}

ostream& operator<<(ostream& lhs, const grep_request& rhs) {
  return lhs << rhs.to_string();
}

///////////////////////////////////////////////////////////////////////////////////

void mimic_test001() {
  // lock used for both num_write num_read and cerr
  pthread_mutex_t rw_lock;
  int num_read = -1;
  int num_write = 0;

  RequestQueue* q = new RequestQueue();
  pthread_mutex_init(&rw_lock, nullptr);

  ThreadArg* arg = new ThreadArg({q, num_read, num_write, rw_lock});

  pthread_t reader;
  pthread_create(&reader, nullptr, read_grep_requests, arg);


  // first test that the reader thread sleeps/waits
  // when it tries to read and there is nothing on the queue

  // sleep for a bit to give a chance for reader to start
  // and to try and wait_remove from q
  sleep(1);

  /*
  // ensure that the reader thread
  // has started and not read anything
  pthread_mutex_lock(&rw_lock);
  while (num_read != 0) {
    pthread_mutex_unlock(&rw_lock);
    sleep(1);
    pthread_mutex_lock(&rw_lock);
  }
  pthread_mutex_unlock(&rw_lock);

  // add and sleep so that reader
  // has a chance to read
  REQUIRE(q->add(kPi));
  sleep(1);

  pthread_mutex_lock(&rw_lock);
  num_write = 1;
  if (num_read != 1) {
    cerr << "wait_remove seemingly doesn't notice the addition of";
    cerr << "a grep_request to the queue. Possible deadlock or not implemented";
    cerr << endl;
  }
  REQUIRE(1 == num_read);
  pthread_mutex_unlock(&rw_lock);

  // next, test the case where there are already
  // values in the queue when the reader calls wait_remove.
  pthread_mutex_lock(&rw_lock);
  REQUIRE(q->add(kOne));
  REQUIRE(q->add(kNegative));
  REQUIRE(q->add(kZero));
  REQUIRE(q->add(kRs));
  num_write += 4;
  pthread_mutex_unlock(&rw_lock);

  // sleep for a bit to give a chance for reader to start
  // and to try and wait_remove from q
  sleep(3);

  int secs_waited;
  for (secs_waited = 3; secs_waited < 30; secs_waited += 3) {
    pthread_mutex_lock(&rw_lock);
    if (num_read == 5) {
      pthread_mutex_unlock(&rw_lock);
      break;
    }
    pthread_mutex_unlock(&rw_lock);
    sleep(3);
  }

  // make sure that we didn't time out
  // waiting for the reader thread to
  // read 4 more times.
  REQUIRE(secs_waited < 30);

  bool* reader_result;
  pthread_join(reader, reinterpret_cast<void**>(&reader_result));

  // make sure the reader had no errors
  REQUIRE(*reader_result);

  delete reader_result;
  delete q;
  delete arg;
  */
}

int main() {
	//split_tester();
	//grep_tester();
	//read_query_test();
	mimic_test001();


	return EXIT_SUCCESS;
}


void* read_grep_requests(void* arg) {
  ThreadArg* args = static_cast<ThreadArg*>(arg);
  RequestQueue* q = args->queue;
  int& num_read = args->num_read;
  int& num_write = args->num_write;
  pthread_mutex_t& rw_lock = args->rw_lock;

  optional<grep_request> value;

  // will be returned to make sure everything worked
  // form the perspective of the reader
  bool* result = new bool(true);

  pthread_mutex_lock(&rw_lock);
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kPi) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kPi << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  while (true) {
    pthread_mutex_lock(&rw_lock);
    if (num_write == 5) {
      pthread_mutex_unlock(&rw_lock);
      break;
    }
    pthread_mutex_unlock(&rw_lock);
    sleep(1);
  }


  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kOne) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kOne << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kNegative) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kNegative << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kZero) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kZero << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kRs) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kRs << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  return result;
}

void* read_closed(void* arg) {
  ThreadArg* args = static_cast<ThreadArg*>(arg);
  RequestQueue* q = args->queue;
  RequestQueue* second_q = q + 1; // since we created two queues.
  int& num_read = args->num_read;
  int& num_write = args->num_write;
  pthread_mutex_t& rw_lock = args->rw_lock;

  optional<grep_request> value;

  // will be returned to make sure everything worked
  // form the perspective of the reader
  bool* result = new bool(true);

  // should get it once fine from calling wait_remove
  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kPi) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kPi << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  // loop till num_write is 4, to indicate that parent has written 3 more things
  // and closed the queue.
  while (true) {
    pthread_mutex_lock(&rw_lock);
    if (num_write == 4) {
      pthread_mutex_unlock(&rw_lock);
      break;
    }
    pthread_mutex_unlock(&rw_lock);
    sleep(1);
  }

  // at this point, the queue should be closed but there are three values in it.
  // get one with wait_remove, another with remove and the last with wait_remove.
  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kOne) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kOne << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kRs) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kRs << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  value = q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kPi) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kPi << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  // should get nullopt when we call wait_remove in the future since it is now closed
  for (int i = 0; i < 3; i++) {
    value = q->wait_remove();
    pthread_mutex_lock(&rw_lock);
    if (value.has_value()) {
      *result = false;
      cerr << "Incorrect value from returned from call to wait_remove()";
      cerr << "from the reader thread" << endl;
      cerr << "\tExpected: nullopt" << endl;
      cerr << "\tActual: ";
      if (value.has_value()) {
        cerr << value.value() << endl;
      } else {
        cerr << "nullopt" << endl;
      }
    }
    pthread_mutex_unlock(&rw_lock);
  }

  // should get nullopt when we call remove in the future since it is now closed
  for (int i = 0; i < 3; i++) {
    value = q->remove();
    pthread_mutex_lock(&rw_lock);
    if (value.has_value()) {
      *result = false;
      cerr << "Incorrect value from returned from call to remove()";
      cerr << "from the reader thread" << endl;
      cerr << "\tExpected: nullopt" << endl;
      cerr << "\tActual: ";
      if (value.has_value()) {
        cerr << value.value() << endl;
      } else {
        cerr << "nullopt" << endl;
      }
    }
    pthread_mutex_unlock(&rw_lock);
  }

  // test a few more cases with the second queue

  // increment num_read to indicate we are ready for the next queues
  pthread_mutex_lock(&rw_lock);
  num_read += 1;
  pthread_mutex_unlock(&rw_lock);

  // get a value from the queue fine
  value = second_q->wait_remove();
  pthread_mutex_lock(&rw_lock);
  if (!value.has_value() || value != kRs) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: " << kRs << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  num_read += 1;

  // ensure only 1 value has been written so far
  // for this test we count "close" as a write
  if (num_write != 1) {
    cerr << "TEST ERROR: value of num_write should be 1, parent should not have written to queue yet." << endl;
    cerr << "Contact Travis unless you modified the tests" << endl;
    pthread_mutex_unlock(&rw_lock);
    *result = false;
    return result;
  }

  // call remove on an empty (but not closed) queue and get nullopt
  value = second_q->remove();

  if (value.has_value()) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: nullopt" << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }
  pthread_mutex_unlock(&rw_lock);

  // call wait_remove and block since the queue is empty (but not closed)
  // we should get woken up by the call to close()
  // in the producer thread.
  value = second_q->wait_remove();

  pthread_mutex_lock(&rw_lock);
  if (value.has_value()) {
    *result = false;
    cerr << "Incorrect value from returned from call to wait_remove()";
    cerr << "from the reader thread" << endl;
    cerr << "\tExpected: nullopt" << endl;
    cerr << "\tActual: ";
    if (value.has_value()) {
      cerr << value.value() << endl;
    } else {
      cerr << "nullopt" << endl;
    }
  }

  if (num_write != 2) {
    cerr << "child returned from wait_remove without parent calling close." << endl;
    *result = false;
  }

  // set this num_read to -1 to indicate that we have terminated.
  num_read = -1;
  pthread_mutex_unlock(&rw_lock);

  return result;
}
