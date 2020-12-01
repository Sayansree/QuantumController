#include "serial/Serial.h"
using namespace std;

Serial::Serial(){
	fd=-1;
}
Serial::~Serial(){
	end();
}
std::string Serial::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

bool Serial::begin(const char *port,speed_t BAUDRATE){
	if(fd>-1) end();
	fd = open(port,O_RDWR | O_NOCTTY);
	if(fd ==-1)	return false;
	termios SerialPortSettings;
	tcgetattr(fd, &SerialPortSettings);
	cfsetspeed(&SerialPortSettings,BAUDRATE);
	SerialPortSettings.c_cflag &= ~(PARENB|CSTOPB|CSIZE|CRTSCTS);   // No Parity,stop bits 1, clear data bit mask,Turn off hardware based flow control (RTS/CTS).
	SerialPortSettings.c_cflag |=  CS8|CREAD|CLOCAL;   //Set the data bits = 8 , turn on receiver
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY | INLCR | ICRNL | IGNCR);
	SerialPortSettings.c_oflag &= ~(ONLCR | OCRNL);
	SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG | IEXTEN);
	SerialPortSettings.c_cc[VMIN]  = 24; /* Read 10 characters */
	SerialPortSettings.c_cc[VTIME] = 1; //timout
	tcsetattr(fd,TCSANOW,&SerialPortSettings);
	return true;
}
void Serial::end(){
	close(fd);
	fd=-1;
}
void Serial::flushI(){
	tcflush(fd,TCIFLUSH);
}
void Serial::flushO(){
	tcflush(fd,TCOFLUSH);
}
void Serial::flushIO(){
	tcflush(fd,TCIOFLUSH);
}
bool Serial::handshake(std::string HANDSHAKE_REF){
	HANDSHAKE_REF+="\r\n";
	int  bytesRead = 0,len = HANDSHAKE_REF.length();
	char handshake[len];
	memset(handshake,'\x00',(len+1)*sizeof(char));
	bytesRead = read(fd,&handshake,len);
	return bytesRead == len && strcmp(HANDSHAKE_REF.c_str(),handshake) == 0;
}

int main(){
	Serial *s=new Serial();
	if(s->begin("/dev/ttyUSB0",B2000000))
		cout<<"\n  ttyUSB0 Opened Successfully\n";
	else
		cout<<"\n  Error! in Opening ttyUSB0\n";
	if(s->handshake("Quantum Ground Station"))
		cout<<"\n Device Detected\n";
	else
		cout<<"\n No Device detected\n";
	// for(char c:handshake)
	// 	cout<<(int)c<<endl;
	// tcflush(fd,TCIOFLUSH);
	cout<<"\n  ttyUSB0 Closed\n";
	s->end();
	return 0;
}
