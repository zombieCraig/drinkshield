/*
  Serial Class
 */

#include "serial.h"

Serial::Serial()
{
  baudrate = B9600;
  port = strdup("/dev/ttyUSB0");
  serialfd = 0;
  verbose = 0;
  emulated = 0;
}

Serial::~Serial()
{

}

void Serial::setBaud(int baud)
{
  baudrate = baud;
}

int Serial::getBaud()
{
  return baudrate;
}

void Serial::setPort(const char *newport)
{
   if(!emulated)
	   port = newport;
}

const char *Serial::getPort()
{
   return port;
}

int Serial::getfd()
{
   return serialfd;
}

int Serial::init()
{
    struct termios toptions;
    int fd;

    if(emulated)
	return 1;

    if(port == NULL)
        return 0;

    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_drinkShieldport: Unable to open port ");
        return -1;
    }

    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_drinkShieldport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baudrate; // let you override switch below if needed
    switch(baudrate) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;

    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_drinkShieldport: Couldn't set term attributes");
        return -1;
    }

    serialfd = fd;
    return fd;
}

int Serial::write_str(const char *data)
{
   if(emulated) {
	switch(data[0]) {
	case DEV_INIT:
		_readbuf = string("drinkShield v0.2");
		break;
	case DEV_READY:
		_readbuf = "Blow!";
		break;
	case DEV_ABORT:
	case DEV_RECAL:
		// Do nothing
		break;
	default:
		if(verbose)
			cout << "Unhandled command to emulated device: " << data[0] << endl;
		return -1;
	}
	return 0;
   }

   if(serialfd <= 0)
	return 0;

   int len = strlen(data);
   int n = write(serialfd, data, len);
   if( n!=len )
	return -1;
   return 0;
}

int Serial::write_str(string data)
{
   return write_str(data.c_str());
}

int Serial::write_str(char byte)
{
    char buf[1];
    buf[0] = byte;
    buf[1] = 0;
    return write_str(buf);
}

int Serial::read_until(char byte)
{
    char b[1];

    if(serialfd <= 0 || emulated)
        return 0;
    _readbuf.clear();
    b[1] = 0;
    do {
        int n = read(serialfd, b, 1);  // read a char at a time
        if( n==-1) {
                usleep(100 * 1000);
                continue;
        }
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
	_readbuf += string(b);
    if(_readbuf.size() > MAX_BUFFER_READ) {
        cerr << "ERR: read in an abnormal amount of data without reaching terminator" << endl;
        _readbuf.clear();
        return 0;
     }
    } while( b[0] != byte );

    return 0;
}

string Serial::readBuf()
{
   return _readbuf;
}

// Generates a random score.  Used by emulate mode
void Serial::randomScore()
{
   char buf[10];
   snprintf(buf, 10, "%d", rand() % 230 + 20);
   _readbuf = buf;
}
