cmake_minimum_required(VERSION 3.10)
project(SMTP_implementation)

find_package(cppzmq)


add_library(smtplib SHARED
  smtp_receiver.cpp
  smtp_sender.cpp
)


add_executable(client client.cpp)
target_link_libraries(client cppzmq)
target_link_libraries(client smtplib)

add_executable(server server.cpp)
target_link_libraries(server cppzmq)
target_link_libraries(server smtplib)

add_executable(msgqueue msgqueue.cpp)
target_link_libraries(msgqueue cppzmq)
target_link_libraries(msgqueue smtplib)




