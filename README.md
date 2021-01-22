Software Engineering Project for CE students fall 2020, Aarhus Universitet<br>
![alt text](https://www.nicepng.com/png/full/422-4224687_eggo-thick-and-fluffy-waffles.png) <br>
# Eggo Enterprises

How to build:<br>
NB. This guide assumes that the machine on which system is to be built is using a Linux operating system. Furthermore, git is used. If this is not installed on the machine this can be done using the package manager. For Ubuntu this would be done by issuing following command in a terminal: sudp apt install git<br> 

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
    mkdir build<br>
    cd build<br>
    cmake ..<br>
    make<br> 
    
  



