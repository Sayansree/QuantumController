#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <string>
#include <string.h>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <vector>
#include <boost/algorithm/string.hpp>

class Serial{

public:
  Serial();
  ~Serial();
  bool begin(std::string, speed_t);
  void end();
  void flushI();
  void flushO();
  void flushIO();
  std::string autoConnect(std::string, speed_t);
  std::vector <std::string> scanPorts();
  bool handshake(std::string);


private:

  std::string exec(const char*);
  std::string PORT;
  int fd;
};

#endif
