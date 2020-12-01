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

class Serial{

public:
  Serial();
  ~Serial();
  bool begin(const char*,speed_t BAUDRATE);
  void end();
  void flushI();
  void flushO();
  void flushIO();
  bool handshake(std::string);

private:
  std::string exec(const char*);
  int fd;
};

#endif
