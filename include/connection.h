#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <zmq.hpp>

struct connection {
	zmq::context_t context;
	zmq::socket_t socket;
};

#endif
