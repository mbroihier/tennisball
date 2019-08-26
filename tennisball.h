/*
 *      tennisball.h - Tennis ball class
 *
 *      Copyright (C) 2019 
 *          Mark Broihier
 *
 */

/* ---------------------------------------------------------------------- */
#include <wiringPi.h>
/* ---------------------------------------------------------------------- */
class tennisball {

  private:

  public:


  tennisball(void) {};

  void nsecDelay(int duration);

  int waitFor(int state, int pin, int duration);

  int waitForPulse(int state, int pin, int duration);
  
  ~tennisball(void){
     pinMode(28, INPUT);
     pinMode(29, INPUT);
     printf("Terminating\n");
   };
    
};

