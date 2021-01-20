cmake_minimum_required(VERSION 3.10)
project(SMTP_implementation)
set(CMAKE_BUILD_TYPE Debug)

find_package(cppzmq)
include_directories(include)

add_library(maillib SHARED
  src/SMTPsession.cpp
  src/SMTP_event.cpp 
  src/SMTP_States.cpp
  src/POPstates.cpp
  src/POPsession.cpp
  src/event.cpp
  src/top.cpp
  )
add_library(mailuserlib SHARED
  src/PieceOfMail.cpp  
  src/UserCollection.cpp
  src/user.cpp
  )

add_executable(SMTPclient SMTPclient.cpp)
target_link_libraries(SMTPclient cppzmq)

add_executable(POPclient popclient.cpp)
target_link_libraries(POPclient cppzmq)

add_executable(server main.cpp)
target_link_libraries(server maillib)
target_link_libraries(server cppzmq)
target_link_libraries(server mailuserlib)


