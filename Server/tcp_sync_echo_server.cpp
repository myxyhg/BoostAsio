/*********************************************************************************
* FileName:		tcp_sync_echo_server.cpp
* Author:		gehan
* Date:			07/14/2017
* Description:	The implementation of sync connect with tcp for server
**********************************************************************************/

#include "server.h"

size_t read_complete(char *buf, const error_code & err, size_t bytes)
{
	if (err)
	{
		return 0;
	}
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	return found ? 0 : 1;
}

void handle_connections()
{
	ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),1000));
	char buff[1024];
	for (;;)
	{
		ip::tcp::socket sock(service);
		acceptor.accept(sock);
		std::cout << "client ip address: " << sock.remote_endpoint().address() << std::endl;
		int bytes = read(sock,buffer(buff),boost::bind(read_complete,_1,_2));
		std::string msg(buff, bytes);
		sock.write_some(buffer(msg));
		sock.close();
	}
}

int main(int argc, char **argv)
{
	handle_connections();
}