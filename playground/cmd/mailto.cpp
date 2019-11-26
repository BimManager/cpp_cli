// Copyright Samolet LLC
// Author: kkozlov
// mailto.cpp

#include <stdlib.h>

#using <System.dll>
namespace sys = System;
namespace mail = System::Net::Mail;

void	mailto(void) {
  system("start \"\" \"mailto:k.kozlov@samoletgroup.ru");
}

void SendMail(sys::String^ smtp_host) {
  mail::SmtpClient^	client;
  mail::MailAddress^	from;
  mail::MailAddress^	to;
  mail::MailMessage^	msg;
}

int	main(void) {
  mailto();
  return (0);
}
