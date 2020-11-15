#include "smtp_sender.h"

int SMTP_Sender::SendRequest(char* buffer){
  
  /*int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        s_send (requester, "HELO");
        char *string = s_recv (requester);
        printf ("Received reply %d [%s]\n", request_nbr, string);
        free (string);
	}*/
    s_send(requester, buffer);
    char *string = s_recv(requester);
    std::cout << string;
    free (string);
    

   return 0;
}
  
