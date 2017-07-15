/*********************************************************************************
* FileName:		udp_sync_echo_server.cpp
* Author:		gehan
* Date:			07/15/2017
* Description:	The implementation of sync connect with udp for server
**********************************************************************************/

#include "server.h"

void handle_connections()
{
	char buff[1024];
	ip::udp::socket sock(service, ip::udp::endpoint(ip::udp::v4(), 1000));
	for (;;)
	{
		ip::udp::endpoint sender_ep;
		int bytes = sock.receive_from(buffer(buff), sender_ep);
		std::string msg(buff, bytes);
		sock.send_to(buffer(msg),sender_ep);
	}
}

int main(int argc, char **argv)
{
	handle_connections();
}