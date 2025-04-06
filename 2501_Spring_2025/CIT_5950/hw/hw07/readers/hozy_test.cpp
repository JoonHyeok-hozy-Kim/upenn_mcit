#include <iostream>

#include "./BufferedFileReader.hpp"
#include "./BufferChecker.hpp"
#include "./SimpleFileReader.hpp"
//#include "catch.hpp"
#include <errno.h>
#include <fstream>
#include <string>
#include <sys/select.h>
#include <unistd.h>

using namespace std;

static constexpr const char *kHelloFileName = "./test_files/Hello.txt";
static constexpr const char *kByeFileName = "./test_files/Bye.txt";
static constexpr const char *kLongFileName = "./test_files/war_and_peace.txt";
static constexpr const char *kGreatFileName = "./test_files/mutual_aid.txt";




using namespace std;

void open_close_test() {
	SimpleFileReader S("no_text.txt");

	cout << S.good() << endl;

	S.open_file("./test_files/Bye.txt");
	cout << S.good() << endl;

	//char readS = S.get_char();
	//cout << readS << endl;
	while (true) {
		char R = S.get_char();
		if (R == EOF) {
			break;
		}
		cout << R;
	}
	cout << endl;

	S.close_file();
	cout << S.good() << endl;
}

void get_chars_test() {
	SimpleFileReader S("./test_files/Hello.txt");
	cout << S.good() << endl;

	while (true) {
		auto ret = S.get_chars(5);
	        if (ret.has_value()) {
			cout << ret.value() << ", " << S.tell() << endl;
		} else {
			break;
		}
	}
	cout << S.good() << endl;
	       		       
	S.rewind();
	cout << S.good() << endl;
	auto ret = S.get_chars(8);
	cout << ret.value() << endl;
	cout << S.good() << endl;
	
}

void mimic_001() {
      // file contents
      string kHelloContents{};
      string kByeContents{};
      string kLongContents{};
      string kGreatContents{};
   
      // flil the above strings with the actual contents of the file
      ifstream hello_ifs(kHelloFileName);
      kHelloContents.assign((std::istreambuf_iterator<char>(hello_ifs)),
                            (std::istreambuf_iterator<char>()));
      ifstream bye_ifs(kByeFileName);
      kByeContents.assign((std::istreambuf_iterator<char>(bye_ifs)),
                          (std::istreambuf_iterator<char>()));
      ifstream long_ifs(kLongFileName);
      kLongContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                           (std::istreambuf_iterator<char>()));
      ifstream great_ifs(kGreatFileName);
      kGreatContents.assign((std::istreambuf_iterator<char>(great_ifs)),
                            (std::istreambuf_iterator<char>()));
   
      // Hello test case
      SimpleFileReader sf(kHelloFileName);
      string contents;
      string next;
      optional<string> opt;
      size_t n = 1;
   
      for (size_t i = 0; i < kHelloContents.length(); i += n, n++) {
        opt = sf.get_chars(n);
        //REQUIRE(opt.has_value());
        next = opt.value();
        contents += next;
        if (i + n <= kHelloContents.length()) {
	  cout << "Pt1 " << contents << " / good : " << sf.good() << endl;
          //REQUIRE(sf.good());
        } else {
	  cout << "Pt2 " << contents << " / good : " << sf.good() << endl;
          //REQUIRE_FALSE(sf.good());
        }
      }
   
}

void buffered_open_close_test() {
	BufferedFileReader b1(kHelloFileName);
	while (b1.good()) {
		cout << "char : " << b1.get_char() << " / tell : " << b1.tell() << endl;
	}
	//BufferedFileReader b2{std::move(b1)};
	//cout << b2.get_char() << endl;

	b1.rewind();
	while (b1.good()) {
		cout << b1.get_char() << endl;
	}
}

void token_test() {
	BufferedFileReader b1(kByeFileName);
	while (b1.good()) {
		auto token = b1.get_token();
		if (token.has_value()) {
			cout << token.value() << endl;
		}
	}

	cout << "------------------------" << endl;

	b1.rewind();
	while (b1.good()) {
		auto token = b1.get_token(",");
		if (token.has_value()) {
			cout << token.value() << endl;
		}
	}
	/*
	BufferedFileReader b2; 
	b2 = b1;
	b2.rewind();
	while (b2.good()) {
		auto token = b2.get_token();
		if (token.has_value()) {
			cout << token.value() << endl;
		}
	}
	*/
}

void mimic002() {

   // file contents
   string kHelloContents{};
   string kByeContents{};
   string kLongContents{};
   string kGreatContents{};

   // flil the above strings with the actual contents of the file
   ifstream hello_ifs(kHelloFileName);
   kHelloContents.assign((std::istreambuf_iterator<char>(hello_ifs)),
                         (std::istreambuf_iterator<char>()));
   ifstream bye_ifs(kByeFileName);
   kByeContents.assign((std::istreambuf_iterator<char>(bye_ifs)),
                       (std::istreambuf_iterator<char>()));
   ifstream long_ifs(kLongFileName);
   kLongContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                        (std::istreambuf_iterator<char>()));
   ifstream great_ifs(kGreatFileName);
   kGreatContents.assign((std::istreambuf_iterator<char>(great_ifs)),
                         (std::istreambuf_iterator<char>()));

   // Hello test case
   BufferedFileReader bf(kHelloFileName);
   BufferChecker bc(bf);
   string contents;
   char c;
   for (size_t i = 0; i < kHelloContents.length(); i++) {
     //cout << "bf.tell() >= 0 : " << (bf.tell() >= 0) << endl; 
     //cout << "i == static_cast<size_t>(bf.tell())) : " << (i == static_cast<size_t>(bf.tell())) << endl;
     c = bf.get_char();
     cout << "In loop, c : " << c << endl;
     contents += c;
     //cout << "bf.good() : " << bf.good() << endl;
     //cout << "bc.check_char_errors(c, i) : " << bc.check_char_errors(c, i) << endl;
   }
   //REQUIRE(kHelloContents == contents);
   c = bf.get_char();
   cout << "EOF == c : " << (EOF == c) << endl;
   //REQUIRE(EOF == c);
   //REQUIRE_FALSE(bf.good());

}

static bool verify_token(const string &actual, const string &expected_contents,
                         const string &delims, off_t *offset) {
  off_t off = *offset;
  string expected = expected_contents.substr(off, actual.length());
  if (actual != expected) {
    *offset = off;
    return false;
  }

  off += actual.length();
  if (off >= static_cast<off_t>(expected_contents.length())) {
    // eof reached
    *offset = off;
    return true;
  }

  if (delims.find(expected_contents[off]) == string::npos) {
    *offset = off + 1;
    return false;
  }

  off++;

  *offset = off;
  return true;
}

void mimic003() {
	
   string token{};
   optional<string> opt{};
   string delims{"\t "};
   off_t offset{0};
   // file contents
   string kHelloContents{};
   string kLongContents{};

   // flil the above strings with the actual contents of the file
   ifstream hello_ifs(kHelloFileName);
   kHelloContents.assign((std::istreambuf_iterator<char>(hello_ifs)),
                         (std::istreambuf_iterator<char>()));
   ifstream long_ifs(kLongFileName);
   kLongContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                        (std::istreambuf_iterator<char>()));
   BufferedFileReader bf(kHelloFileName);
   BufferChecker bc(bf);

   while (bf.good()) {
     opt = bf.get_token(delims);
     //REQUIRE(opt.has_value());
     token = opt.value();
     cout << "token : " << token << endl;
     cout << "check_token_errors : " << bc.check_token_errors(token, offset) << endl;
     cout << "verify_token(token, kHelloContents, delims, &offset) : " << verify_token(token, kHelloContents, delims, &offset) << endl;
     //REQUIRE(offset == static_cast<off_t>(bf.tell()));
     cout << "offset : " << offset << ", " << "static_cast<off_t>(bf.tell()) : " << static_cast<off_t>(bf.tell()) << endl; 
   }

   //REQUIRE(static_cast<off_t>(kHelloContents.length()) == offset);

   offset = 0;
   bf.open_file(kLongFileName);
   while (bf.good()) {
     opt = bf.get_token(delims);
     //REQUIRE(opt.has_value());
     token = opt.value();
     //REQUIRE_FALSE(bc.check_token_errors(token, offset));
     //REQUIRE(verify_token(token, kLongContents, delims, &offset));
     //REQUIRE(offset == static_cast<off_t>(bf.tell()));
   }

   //REQUIRE(static_cast<off_t>(kLongContents.length()) == offset);
   opt = bf.get_token();
   //REQUIRE_FALSE(opt.has_value());

}

void mimic004() {

   string kGreatContents{};
   ifstream long_ifs(kGreatFileName);
   kGreatContents.assign((std::istreambuf_iterator<char>(long_ifs)),
                        (std::istreambuf_iterator<char>()));

   // use new so we can control when it is destructed
   BufferedFileReader* bf = new BufferedFileReader(kGreatFileName);

   BufferedFileReader& bf0 = *bf;
   BufferChecker bc0(bf0);

   int fd = bc0.fd();
   cout << "fd : " << fd << endl;
   auto opt = bf0.get_token();
   //REQUIRE(opt.has_value());
   //REQUIRE(opt.value() == "Project");

   auto arr = bc0.buffer();

   // move assign a new bf
   BufferedFileReader bf1(kHelloFileName);
   bf1 = std::move(bf0);
   
   BufferChecker bc1(bf1);

   //REQUIRE(fd == bc1.fd());

}

int main() {
	//open_close_test();	
	//get_chars_test();
	//mimic_001();
	//buffered_open_close_test();
	//token_test();
	//mimic002();
	//mimic003();
	mimic004();




	return EXIT_SUCCESS;
}
