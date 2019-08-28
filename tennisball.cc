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
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include <sys/ioctl.h>

#include "tennisball.h"
/* ---------------------------------------------------------------------- */


static const char USAGE_STR[] = "\n"
  "Usage: %s \n";

int done = 0;

void terminate(int level) {
  extern int done;
  done = 1;
}

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
  if (duration > 1000000000) {
    printf("Illegal duration: %d\n", duration);
    return;
  }
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
  if (duration > 1000000000) {
    printf("Illegal duration: %d\n", duration);
    return 0;
  }
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
  if (duration > 1000000000) {
    printf("Illegal duration: %d\n", duration);
    return 0;
  }
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
    //printf("Timeout/Error -- no pulse \n");
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
      //printf("Timeout/Error -- pulse too long\n");
      deltaTime = 0;
    }
  }
  return deltaTime;
};

int main(int argc, char *argv[]) {

  extern int done;
  tennisball tennisballInstance;

  int accumulator = 0;
  int samples = 0;
  int average = 0;
  
  int deltaTime = 0;

  sem_t releaseCPU;
  struct timespec ts;
  
  if (argc != 1) {
    fprintf(stderr, USAGE_STR, argv[0]);
    return -2;
  }

  sem_init(&releaseCPU, 0, 0);
  clock_gettime(CLOCK_REALTIME, &ts);
  printf("Setting up PI\n");
  wiringPiSetup();
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(1, INPUT);
  digitalWrite(0, 1); // set input to low
  signal(SIGINT, &terminate);
  tennisballInstance.nsecDelay(50000000);
  printf("Initialization done.\n");
  while (!done) {
    digitalWrite(29, 0); // begin pulse, should be high at input
    tennisballInstance.nsecDelay(5000);
    digitalWrite(29, 1); // end of pulse
    tennisballInstance.nsecDelay(725000);
    deltaTime = tennisballInstance.waitForPulse(1, 1, 25000000);
    //printf("Delta time: %10d\n", deltaTime);
    if (average == 0) {
      digitalWrite(28, 0);
      if (deltaTime) {
        accumulator += deltaTime;
        samples++;
	if (abs(accumulator - samples*deltaTime) > 100000*samples) {
	  printf("Resetting at count: %d, sample: %d, accumulator: %d\n", samples, accumulator, deltaTime);
	  samples = 0;
	  accumulator = 0;
	}
        if (samples >= 100) {
	  average = accumulator / samples;
	  if (accumulator & 0xe0000000) { // overflow no "target"
	    printf("accumulator overflow\n");
	    average = 0;
	    accumulator = 0;
	    samples = 0;
	  } else {
	    average += 5000; // bias by a bit (5 microseconds)
	  }
	  printf("Average: %d\n", average);
        }
      }
    } else {
      if (deltaTime) {
        if (average < deltaTime) {
	  digitalWrite(28, 0); // can be closer
        } else {
	  digitalWrite(28, 1); // close enough
        }
      }
    }
    //tennisballInstance.nsecDelay(200000); // delay 200 u seconds
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec += 250000000;
    if (ts.tv_nsec >= 1000000000) {
      ts.tv_sec++;
      ts.tv_nsec -= 1000000000;
    }
    if (sem_timedwait(&releaseCPU, &ts) < 0) {
      if (errno != ETIMEDOUT) perror("sem_timedwait");
    } else {
      printf("Should never get here\n");
    }
  }

  return 0;

}

/* ---------------------------------------------------------------------- */
