#include "smtp_sender.cpp"

int main() {
  SMTP_Sender s;
  s.SendRequest("HELO");

}
