#!/bin/bash

echo -e "This script tests the units of the mail system\n"
echo -e "The server should be running. If it is not running please open a new terminal, cd to build folder and issue ./server\n"
echo -e "We will now initiate some sessions. You can follow along by looking at the messages received by server and client programs in terminal windows.\n"

echo -e "First we add a user with username ExampleUser and password password.\n"

./build/addUser

echo -e "\n"
echo -e "We will now send a piece of mail from the newly registered user to two other users of the system, Bob and Alice.\n"


./build/SMTPclient


echo -e "\n"
echo -e "Now we log in as Bob, another user of the system. Bob should have received a piece of mail 
from ExampleUser. If this piece of mail is displayed at the client side terminal after the command
"RETR 0" is issued mail delivery and retrieval works as intended.\n"

./build/POPclient1

echo -e "\n"
echo -e "Finally we log in as user Alice, read the email from ExampleUser, delete it and ask to view this deleted mail. The server should respond that requested piece of mail no longer exists.\n"

./build/POPclient2


