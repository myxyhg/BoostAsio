/*********************************************************************************
* FileName:		sync_client.cpp
* Author:		gehan
* Date:			07/19/2017
* Description:	simple connection to server:
				- logs in just with username (no password)
				- all connections are initiated by the client: client asks, server answers
				- server disconnects any client that hasn't pinged for 5 seconds

				Possible requests:
				- gets a list of all connected clients
				- ping: the server answers either with "ping ok" or "ping client_list_changed"
**********************************************************************************/

#include "client.h"

struct talk_to_srv
{
	talk_to_srv(const std::string & username)
		: sock_(service), started_(true), username_(username) {}

	void connect(ip::tcp::endpoint ep)
	{
		sock_.connect(ep);
	}

	void loop()
	{
		write("login " + username_ + "\n");
		read_answer();
		while (started_)
		{
			write_request();
			read_answer();
			int millis = rand() % 7000;
			std::cout << username_ << " postpone ping "
				<< millis << " ms" << std::endl;
			boost::this_thread::sleep(boost::posix_time::millisec(millis));
		}
	}

	std::string username() const
	{
		return username_;
	}
private:
	void write_request()
	{
		write("ping\n");
	}

	void write(const std::string & msg)
	{
		sock_.write_some(buffer(msg));
	}

	void read_answer()
	{
		already_read_ = 0;
		read(sock_, buffer(buff_), boost::bind(&talk_to_srv::read_complete, this, _1, _2));
		process_msg();
	}

	size_t read_complete(const boost::system::error_code & err, size_t bytes)
	{
		if (err)
		{
			return 0;
		}
		already_read_ = bytes;
		bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
		return found ? 0 : 1;
	}

	void process_msg()
	{
		std::string msg(buff_, already_read_);
		if (0 == msg.find("login ")) 
			on_login();
		else if (0 == msg.find("ping")) 
			on_ping(msg);
		else if (0 == msg.find("clients ")) 
			on_clients(msg);
		else 
			std::cerr << "invalid msg " << msg << std::endl;
	}

	void on_login()
	{
		std::cout << username_ << " logged in" << std::endl;
		do_ask_clients();
	}

	void do_ask_clients()
	{
		write("ask_clients\n");
		read_answer();
	}

	void on_ping(const std::string & msg)
	{
		std::istringstream in(msg);
		std::string answer;
		in >> answer >> answer;
		if ("client_list_changed" == answer)
			do_ask_clients();
	}

	void on_clients(const std::string & msg)
	{
		std::string clients = msg.substr(8);
		std::cout << username_ << ", new client list:" << clients;
	}

private:
	ip::tcp::socket sock_;
	enum { max_msg = 1024 };
	int already_read_;
	char buff_[max_msg];
	bool started_;
	std::string username_;
};

ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 1000);
void run_client(const std::string & client_name)
{
	talk_to_srv client(client_name);
	try
	{
		client.connect(ep);
		client.loop();
	}
	catch (boost::system::system_error & err) 
	{
		std::cout << "client terminated " << client.username()
			<< ": " << err.what() << std::endl;
	}
}

int main(int argc, char* argv[]) 
{
	boost::thread_group threads;
	char* names[] = { "John", "James", "Lucy", "Tracy", "Frank", "Abby", 0 };
	for (char ** name = names; *name; ++name) 
	{
		threads.create_thread(boost::bind(run_client, *name));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}
	threads.join_all();
}
