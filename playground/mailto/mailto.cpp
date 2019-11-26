// Copyright Samolet LLC
// Author: kkozlov
// mailto.cpp

#include <stdlib.h>

#include "windows.h"
#include "shellapi.h"

void	mailto(void) {
  system("START mailto:foo@baz.com");
}

void	mailto2(void) {
  ShellExecute(NULL, "open", "mailto:k.kozlov@baz.com", NULL, NULL, 0);
}

int	main(void) {
  mailto2();
  return (0);
}
