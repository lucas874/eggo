cmake_minimum_required(VERSION 3.10)
project(SMTP_implementation)
set(CMAKE_BUILD_TYPE Debug)

find_package(cppzmq)
include_directories(include)

add_library(maillib SHARED
  src/SMTPsession.cpp
  src/SMTPevent.cpp 
  src/SMTPstates.cpp
  src/POPstates.cpp
  src/POPsession.cpp
  src/POPevent.cpp
  src/top.cpp
  )
add_library(mailuserlib SHARED
  src/PieceOfMail.cpp  
  src/UserCollection.cpp
  src/user.cpp
  src/editUserCollection.cpp
  )

add_executable(SMTPclient SMTPclient.cpp)
target_link_libraries(SMTPclient cppzmq)

add_executable(POPclient popclient.cpp)
target_link_libraries(POPclient cppzmq)

add_executable(SMTPclient1 SMTPclient1.cpp)
target_link_libraries(SMTPclient1 cppzmq)

add_executable(POPclient1 popclient1.cpp)
target_link_libraries(POPclient1 cppzmq)

add_executable(addUser addusertest.cpp)
target_link_libraries(addUser cppzmq)

add_executable(server main.cpp)
target_link_libraries(server maillib)
target_link_libraries(server cppzmq)
target_link_libraries(server mailuserlib)


