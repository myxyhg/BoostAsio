/*********************************************************************************
* FileName:		client.h
* Author:		gehan
* Date:			07/14/2017
* Description:	The general header file for client side
**********************************************************************************/

#pragma once

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using boost::system::error_code;

io_service service;