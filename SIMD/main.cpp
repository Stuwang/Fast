#include <cstring>
#include <fstream>
#include <iostream>

#include "MyString.h"

#include <boost/date_time.hpp>

using namespace std;

const char* data =
    "12345678901234567890123456789012345678901234567890123456789012345678901234"
    "567890";

template <class Fun>
size_t test(const char* str, size_t times, const Fun& f) {
  auto start = boost::posix_time::microsec_clock::local_time();
  for (size_t i = 0; i < times; i++) {
    f();
  }
  auto end = boost::posix_time::microsec_clock::local_time();
  auto d = end - start;
  cout << str << d << endl;
  return d.total_nanoseconds();
}

int main() {
  size_t times = 10;
  typedef char* CharPointer;

  fstream file("aaa.txt");
  std::vector<std::string> vstr;
  while (true) {
    std::string data;
    file >> data;
    if (!data.length()) {
      break;
    }
    vstr.push_back(data);
  }
  cout << "Test String Count " << vstr.size() << endl;

  std::vector<size_t> v1(times * vstr.size());
  std::vector<size_t> v2(times * vstr.size());

  int index = 0;
  size_t mine = test("Mine ", times, [&]() {
    for (size_t i = 0; i < vstr.size(); i++) {
      size_t l = Strlen(vstr[i].data());
      v1[index++] = l;
    }
  });

  index = 0;
  size_t sys = test("System ", times, [&]() {
    for (size_t i = 0; i < vstr.size(); i++) {
      size_t l = ::strlen(vstr[i].data());
      v1[index++] = l;
    }
  });

  std::cout << sys * 1.0 / mine << std::endl;
  std::cin.get();
}