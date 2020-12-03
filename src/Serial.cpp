#include "serial/Serial.h"
using namespace std;

Serial::Serial(){
	fd=-1;
}
Serial::~Serial(){
	end();
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
	PORT="";
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
std::string Serial::autoConnect(std::string HANDSHAKE_REF,speed_t BAUDRATE){
	vector<std::string> ports=scanPorts();
	for(std::string port:ports){
		begin(port.c_str(),BAUDRATE);
		if(handshake(HANDSHAKE_REF)){
			PORT=port;
			return port;
		}
	}
	return "";
}
bool Serial::handshake(std::string HANDSHAKE_REF){
	HANDSHAKE_REF+="\r\n";
	int  bytesRead = 0,len = HANDSHAKE_REF.length();
	char handshake[len];
	memset(handshake,'\x00',(len+1)*sizeof(char));
	bytesRead = read(fd,&handshake,len);
	return bytesRead == len && strcmp(HANDSHAKE_REF.c_str(),handshake) == 0;
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
std::vector <std::string> Serial::scanPorts(){
		std::vector <std::string> portsACM,portsUSB,ports;
		std::string result= exec("ls /dev/ttyACM[0-9]\n");
		if(result!=""){
			boost::split(portsACM,result,boost::is_any_of("\n"));
			portsACM.pop_back();
		}
		result= exec("ls /dev/ttyUSB[0-9]\n");
		if(result!=""){
			boost::split(portsUSB,result,boost::is_any_of("\n"));
			portsUSB.pop_back();
		}
		ports.insert(ports.begin(),portsACM.begin(),portsACM.end());
		ports.insert(ports.begin(),portsUSB.begin(),portsUSB.end());
		return ports;
}

int main(){

	Serial *s=new Serial();
	vector<std::string> ports=s->scanPorts();
	for(std::string s:ports)
		cout<<s<<endl;
	std::string responce = s->autoConnect("Quantum Ground Station",B2000000);
	// if(s->begin("/dev/ttyUSB0",B2000000))
	// 	cout<<"\n  ttyUSB0 Opened Successfully\n";
	// else
	// 	cout<<"\n  Error! in Opening ttyUSB0\n";
	if(responce=="")
			cout<<"\n No Device detected\n";
	else
		cout<<"\n"<<responce<<" handshake successful\n";
	cout<<"\n  ttyUSB0 Closed\n";
	s->end();
	return 0;
}
