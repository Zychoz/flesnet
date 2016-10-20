/**
 * @file
 * @author Dirk Hutter <hutter@compeng.uni-frankfurt.de>
 *
 */

#include "flib.h"
#include "device_operator.hpp"
#include <iostream>
#include "httpClient.hpp"
#include <unistd.h>
#include <sys/time.h>
#include <string>
#include <algorithm>

using namespace flib;

string add_double_quotes(string str) {
    string str_new = '\"' + str + '\"';
    return str_new;
}

int main() {

  HttpClient http("http://localhost:8086");
    
    
  stringstream ss;
  stringstream ss_build_info;
  string request;
    
  struct timeval timestamp;
  gettimeofday(&timestamp, NULL);
  long int ms = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
    
    
  unsigned int microseconds = 10;
    
  
    
  std::unique_ptr<pda::device_operator> dev_op(new pda::device_operator);
  std::vector<std::unique_ptr<flib_device_flesin>> flibs;

  uint64_t num_dev = dev_op->device_count();

  //std::cout << "Total number of FLIBs: " << num_dev << std::endl;

  string test = R"(this is a test \" )";
  for (size_t i = 0; i < num_dev; ++i) {
      /*
    std::cout << "*** FLIB " << i << " ***" << std::endl;
    try {
      flibs.push_back(
          std::unique_ptr<flib_device_flesin>(new flib_device_flesin(i)));
      std::cout << "Address: " << flibs.back()->print_devinfo() << std::endl;
      std::cout << "Hardware links: "
                << static_cast<unsigned>(flibs.back()->number_of_hw_links())
                << std::endl;
      std::cout << flibs.back()->print_build_info() << std::endl;
       */
    
    //TODO: undo the following line when timestamps can be made in microseconds or nanoseconds
    usleep(microseconds);
    ss << "flib_info,";
    ss << "FLIB=" << '"' << "FLIB_" << i << '"' << " ";
    try {
      flibs.push_back(
      std::unique_ptr<flib_device_flesin>(new flib_device_flesin(i)));
      ss << "Address=\"(" << flibs.back()->print_devinfo() << ")\",";
      ss << "Hardware_Links="
         << static_cast<unsigned>(flibs.back()->number_of_hw_links()) << ",";
      ss << flibs.back()->print_build_info();
      ss << ms;
      ss << endl;
      request = request + ss.str();
      ss.str("");
    } catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
  char chars[] = "\\";
    
  for (unsigned int i = 0; i < strlen(chars); ++i)
  {
    // you need include <algorithm> to use general algorithms like std::remove()
    request.erase (std::remove(request.begin(), request.end(), chars[i]), request.end());
  }

  http.putreq("/write?","db=mydb",request,"POST");
  cout << request << endl;
  return 0;
}

