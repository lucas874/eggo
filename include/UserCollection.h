#pragma once
#include <user.h>
#include <unordered_map>
#include <fstream>
#include <zmq.hpp>

class UserCollection {
public:
  UserCollection(){};
  void addUser(User*);
  void removeUser(User*);
  int addUser(std::string, std::string);
  int removeUser(std::string, std::string);


  User* LookUp(std::string);
  void addMailToInbox(User*, PieceOfMail*);

  void listen();
  void reply();
  
private:
  std::unordered_map<std::string, User*> umap;
  zmq::context_t context;
  zmq::socket_t socket;
};
 
