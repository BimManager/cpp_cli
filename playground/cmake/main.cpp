// Copyright 2019 Samolet LLC
// Author: kkozlov
// main.cpp

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef USE_MYMATHS
#include "TutorialConfig.h"
#endif

int main(int argc, char **argv) {
  if (argc < 2)
  {
    fprintf(stdout, "%s Version %d.%d\n",
            argv[0], Tutorial_VERSION_MAJOR,
            Tutorial_VERSION_MINOR);
    fprintf(stdout, "usage: %s number\n", argv[0]);
    return (1);
  }
  double inputValue;
  double outputValue;
  inputValue = atof(argv[1]);
#ifdef USE_MYMATHS
  outputValue = mysqrt(inputValue);
#else
  outputValue = sqrt(inputValue);
#endif
  fprintf(stdout, "The square root of %g is %g\n",
          inputValue, outputValue);
  return (0);
}
