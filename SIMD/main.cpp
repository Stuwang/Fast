#include <iostream>
#include <fstream>
#include <cstring>

#include "MyString.h"

#include <boost/date_time.hpp>

using namespace std;

const char * data = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";

template<class Fun>
size_t test(const Fun& f,size_t times){
  auto start = boost::posix_time::microsec_clock::local_time();
  for ( size_t i = 0; i < times; i++ )
  {
    f();
  }
  auto end = boost::posix_time::microsec_clock::local_time();
  auto d = end - start;
  cout << d << endl;
  return d.total_nanoseconds();
}

int main(){
  
  size_t times =  1000;
  typedef char * CharPointer;
 
  fstream file("aaa.txt");
  std::vector<std::string> vstr;
  while ( true ){
    std::string data;
    file >> data;
    if ( !data.length() ){
      break;
    }
    vstr.push_back(data);
  }
  cout << vstr.size() << endl;

  std::vector<size_t> v1(times * vstr.size());
  std::vector<size_t> v2(times * vstr.size());

  int index = 0;
  size_t mine = test([&](){
    for ( size_t i = 0; i < vstr.size(); i++ )
    {
      size_t l = Strlen(vstr[i].data());
      v1[index++] = l;
    }
  }, times);

  index = 0;
  size_t sys = test([&](){
    for ( size_t i = 0; i < vstr.size(); i++ )
    {
      size_t l = ::strlen(vstr[i].data());
      v1[index++] = l;
    }
  }, times);
  
  std::cout << sys*1.0 / mine << std::endl;
  std::cin.get();
}