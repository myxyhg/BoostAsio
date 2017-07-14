/*********************************************************************************
* FileName:		tcp_sync_echo_client.cpp
* Author:		gehan
* Date:			07/14/2017
* Description:	The implementation of sync connect with tcp for client
**********************************************************************************/

#include "client.h"

size_t read_complete(char *buf, const error_code & err, size_t bytes)
{
	if (err)
	{
		return 0;
	}
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	return found ? 0 : 1;
}

ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 1000);
void sync_echo(std::string msg)
{
	msg += '\n';
	ip::tcp::socket sock(service);
	sock.connect(ep);
	sock.write_some(buffer(msg));
	
	char buf[1024];
	int bytes = read(sock,buffer(buf),boost::bind(read_complete,_1,_2));
	std::string copy(buf, bytes - 1);
	msg = msg.substr(0, msg.size() - 1);
	std::cout << "server echoed our " << msg << ": "
		<< (copy == msg ? "OK" : "FAIL") << std::endl;
	sock.close();
}

int main(int argc, char *argv[])
{
	char* messages[] = { "John says hi", "so does James", "Lucy just got home", "Boost.Asio is Fun!", 0 };
	boost::thread_group threads;
	for (char **msg = messages; *msg; ++msg)
	{
		threads.create_thread(boost::bind(sync_echo, *msg));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}
	threads.join_all();
	return 0;
}