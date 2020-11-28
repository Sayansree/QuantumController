#include <iostream>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */
using namespace std;
int main()
{
	int fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY);
	if(fd ==-1){
		cout<<"\n  Error! in Opening ttyUSB0\n";
		return 0;
	}
	cout<<"\n  ttyUSB0 Opened Successfully\n";
	termios SerialPortSettings;
	tcgetattr(fd, &SerialPortSettings);
	cfsetspeed(&SerialPortSettings,B2000000);
	SerialPortSettings.c_cflag &= ~(PARENB|CSTOPB|CSIZE|CRTSCTS);   // No Parity,stop bits 1, clear data bit mask,Turn off hardware based flow control (RTS/CTS).
	SerialPortSettings.c_cflag |=  CS8|CREAD|CLOCAL;   //Set the data bits = 8 , turn on receiver
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY | INLCR | ICRNL | IGNCR);
	SerialPortSettings.c_oflag &= ~(ONLCR | OCRNL);
	SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG | IEXTEN);
	SerialPortSettings.c_cc[VMIN]  = 10; /* Read 10 characters */  
	SerialPortSettings.c_cc[VTIME] = 5; //timout
	tcsetattr(fd,TCSANOW,&SerialPortSettings);   
	char handshake[24];                
	int  bytes_read = 0;                                   
	bytes_read = read(fd,&handshake,24);
	//cout<<std::to_str(handshake);
	tcflush(fd,TCIOFLUSH);
	cout<<"\n  ttyUSB0 Closed\n";
	close(fd);

	return 0;
}