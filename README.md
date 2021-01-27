Software Engineering Project for CE students fall 2020, Aarhus Universitet<br>
![alt text](https://www.nicepng.com/png/full/422-4224687_eggo-thick-and-fluffy-waffles.png) <br>
# Eggo Enterprises

How to build:<br>
NB. This guide assumes that the machine on which software is to be built is using a Linux operating system. To build the system the machine will need a C++ compiler, cmake, libtool and git. If you are unsure whether this is installed, you can download these using a package mangager. If you are using Ubuntu this can be done by issuing the command: sudo apt-get install build-essential libtool cmake<br> 

1. The system requires ZeroMQ for communicating over network. To install the ZeroMQ library two things must be downloaded and built. Open a terminal and type these commands:<br>
    git clone https://github.com/zeromq/libzmq.git<br>
    cd libzmq<br>
    ./autogen.sh<br>
    mkdir build<br>
    cd build<br>
    cmake ..<br>
    sudo make -j4 install<br>
    
    cd ~<br>
    
    git clone https://github.com/zeromq/cppzmq.git<br>
    cd cppzmq<br>
    mkdir build<br>
    cd build<br>
    cmake ..<br>
    sudo make -j4 install<br>
    
    cd ~<br>

2. If the installation of ZeroMQ was successfull we can proceed to build the email system. Type the following commands:<br>
    git clone https://github.com/lucas874/eggo.git --branch master<br>
    cd eggo<br>
    mkdir build<br>
    cd build<br>
    cmake ..<br>
    make<br> 
    
  



