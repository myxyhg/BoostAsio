/*********************************************************************************
* FileName:		udp_sync_echo_client.cpp
* Author:		gehan
* Date:			07/15/2017
* Description:	The implementation of sync connect with udp for client
**********************************************************************************/

#include "client.h"

ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"),1000);

void sync_echo(std::string msg)
{
	ip::udp::socket sock(service, ip::udp::endpoint(ip::udp::v4(), 0));
	sock.send_to(buffer(msg), ep);
	char buff[1024];
	ip::udp::endpoint sender_ep;
	int bytes = sock.receive_from(buffer(buff),sender_ep);
	std::string copy(buff, bytes);
	std::cout << "server echoed our " << msg << ": "
		<< (copy == msg ? "OK" : "FAIL") << std::endl;
	sock.close();
}

int main(int argc, char* argv[]) {
	// connect several clients
	char* messages[] = { "John says hi", "so does James", "Lucy just got home", 0 };
	boost::thread_group threads;
	for (char ** message = messages; *message; ++message) {
		threads.create_thread(boost::bind(sync_echo, *message));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}
	threads.join_all();
}