/*
 Serial Class
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <string>

#define DEV_INIT 'i'
#define DEV_READY 'r'
#define DEV_RECAL 'a'
#define DEV_LIGHTS 'l'
#define DEV_ABORT 'A'
#define DEV_ALIVE 'S'

#define MAX_BUFFER_READ 256

using namespace std;

class Serial
{
public:
   Serial();
   ~Serial();
   int verbose;
   void setBaud(int);
   int getBaud();
   void setPort(const char *);
   const char *getPort();
   int init();
   int getfd();
   int write_str(string);
   int write_str(const char *);
   int write_str(char);
   int read_until(char);
   int emulated;
   string readBuf();

   void randomScore();
private:
   int baudrate;
   const char *port;
   int serialfd;
   string _readbuf;
};

#endif /* __SERIAL_H__ */
