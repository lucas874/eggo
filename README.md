Software Engineering Project for CE students fall 2020, Aarhus Universitet<br>
![alt text](https://www.nicepng.com/png/full/422-4224687_eggo-thick-and-fluffy-waffles.png) <br>
# Eggo Enterprises

How to build:<br>
NB. This guide assumes that the machine on which software is to be built is using a Linux operating system. To build the system the machine will need a C++ compiler, cmake, libtool and git. If you are unsure whether these things are installed, they can be installed using a package manager. If you are using Ubuntu this can be done by issuing the command: sudo apt-get install git build-essential libtool cmake<br> 

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

2. If the installation of ZeroMQ was successful we can proceed to build the email system. Type the following commands:<br>
    git clone https://github.com/lucas874/eggo.git --branch master<br>
    cd eggo<br>
    mkdir build<br>
    cd build<br>
    cmake ..<br>
    make<br> 
    
    
Now that the server is built it can be started with ./server from the build folder. Open a new terminal and cd to eggo. Run the command unzip eggoinbox.zip.<br>

3. Now we can try out the GUI.<br>
    cd eggoinbox<br>
    qmake ..<br>
    make<br>
 
(if it doesn't want to make then spam "qmake .." 3 times consecutively and afterwards write
"make" this usually works)
once the project has been made then write
	./eggoinbox

A login window should pop up in the top left corner of your screen to login then you will 
first have to register a user. Registering a user is fairly simple first you have to click on the 
register button and afterwards then the register window should pop-up do as the placeholdertext 
tells you and click register. When you register and log in do not put a @ and a domain name, just a desired user name. Now you should have a user good to go. you can now write your newly created username and password in the loginwindow and login to your mailbox.
in your inbox you should start with an empty folder to write a mail to anyone then click on "File"
and then click "New" (alternatively then you can also click on the file icon) 
you should now be able to write a mail to someone just do as the placeholdertext suggests. 
You can play around with the different features for fun.
