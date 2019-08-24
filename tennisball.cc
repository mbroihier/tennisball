/*
 *      tennisball.cc -- tennisball main class/program implementation
 *
 *      Copyright (C) 2019 
 *          Mark Broihier
 *
 */

/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include <sys/ioctl.h>

#include "tennisball.h"
/* ---------------------------------------------------------------------- */


static const char USAGE_STR[] = "\n"
  "Usage: %s \n";



/* ---------------------------------------------------------------------- */
/*
 *      nsecDelay.cc -- delay for a duration of "duration" nano seconds
 *
 *      Copyright (C) 2019 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */
void tennisball::nsecDelay(int duration) {
  struct timespec lastTime;
  struct timespec startTime;
  bool done = false;
  int deltaTime;
  clock_gettime(CLOCK_MONOTONIC, &startTime);
  while (!done) {
    clock_gettime(CLOCK_MONOTONIC, &lastTime);
    deltaTime = lastTime.tv_nsec - startTime.tv_nsec;
    if (deltaTime < 0) {
      deltaTime += 1000000000;
    }
    if (deltaTime > duration) {
      done = true;
    }
  }
};
/* ---------------------------------------------------------------------- */
/*
 *      waitFor.cc -- wait for 0 or 1 on pin "pin" for maximum of "duration"
 *                    nano seconds
 *
 *      Copyright (C) 2019 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */
int tennisball::waitFor(int state, int pin, int duration) {
  struct timespec lastTime;
  struct timespec startTime;
  int deltaTime = duration + 1;
  clock_gettime(CLOCK_MONOTONIC, &startTime);
  while (digitalRead(pin) != state) {
    clock_gettime(CLOCK_MONOTONIC, &lastTime);
    deltaTime = lastTime.tv_nsec - startTime.tv_nsec;
    if (deltaTime < 0) {
      deltaTime += 1000000000;
    }
    if (deltaTime > duration) {
      break;
    }
  }
  if (deltaTime > duration) { // timeout or error
    printf("Timeout/Error \n");
    deltaTime = 0;
  } else {
    deltaTime = lastTime.tv_nsec - startTime.tv_nsec;
    if (deltaTime < 0) {
      deltaTime += 1000000000;
    }
  }
  return deltaTime;
};
/* ---------------------------------------------------------------------- */
/*
 *      waitForPulse.cc -- wait for a pulse on pin "pin" for maximum of 
 *                         "duration" of nano seconds
 *
 *      Copyright (C) 2019 
 *          Mark Broihier
 *
 */
/* ---------------------------------------------------------------------- */
int tennisball::waitForPulse(int state, int pin, int duration) {
  struct timespec lastTime;
  struct timespec startTime;
  int deltaTime = duration + 1;
  clock_gettime(CLOCK_MONOTONIC, &startTime);
  while (digitalRead(pin) != state) {
    clock_gettime(CLOCK_MONOTONIC, &lastTime);
    deltaTime = lastTime.tv_nsec - startTime.tv_nsec;
    if (deltaTime < 0) {
      deltaTime += 1000000000;
    }
    if (deltaTime > duration) {
      break;
    }
  }
  if (deltaTime > duration) { // timeout or error
    printf("Timeout/Error -- no pulse \n");
    deltaTime = 0;
  } else {
    startTime.tv_nsec = lastTime.tv_nsec;
    while (digitalRead(pin) == state) {
      clock_gettime(CLOCK_MONOTONIC, &lastTime);
      deltaTime = lastTime.tv_nsec - startTime.tv_nsec;
      if (deltaTime < 0) {
        deltaTime += 1000000000;
      }
      if (deltaTime > duration) {
        break;
      }
    }
    if (deltaTime > duration) {
      printf("Timeout/Error -- pulse too long\n");
      deltaTime = 0;
    }
  }
  return deltaTime;
};

int main(int argc, char *argv[]) {

  tennisball tennisballInstance;

  int deltaTime = 0;
  
  if (argc != 1) {
    fprintf(stderr, USAGE_STR, argv[0]);
    return -2;
  }

  printf("Setting up PI\n");
  wiringPiSetup();
  printf("Setting up pin 1\n", deltaTime);
  pinMode(0, OUTPUT);
  pinMode(1, INPUT);
  digitalWrite(0, 1); // set input to low
  tennisballInstance.nsecDelay(50000000);
  printf("Initialization done.\n");
  while (true) {
    digitalWrite(0, 0); // begin pulse, should be high at input
    tennisballInstance.nsecDelay(5000);
    digitalWrite(0, 1); // end of pulse
    tennisballInstance.nsecDelay(725000);
    deltaTime = tennisballInstance.waitForPulse(1, 1, 25000000);
    printf("Delta time: %10d\n", deltaTime);
    sleep(1);
  }

  return 0;

}

/* ---------------------------------------------------------------------- */
