cmake_minimum_required(VERSION 3.10)
project(SMTP_implementation)

find_package(cppzmq)

add_library(smtplib SHARED
  SMTPConnection.cpp
  EventData.cpp
  SMTPState.cpp
)

add_executable(test test.cpp)
target_link_libraries(test cppzmq)
target_link_libraries(test smtplib)

