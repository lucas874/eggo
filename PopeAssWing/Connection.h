#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include "zhelpers.h"

/*
 * struct with stuff for network communication
 * context and socket
 */
struct Connection {
  void *context;
  void *socket;
};


#endif
