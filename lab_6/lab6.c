/*
File:Lab6_wiringPi
Author:Bradley Duguay W0516067
Date:March 1st 2026
Description:GPIO control of LED bar using wiringpi library. Sets in/output pins & cycles through the set LED array with
assistance from time.h structures
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wiringPi.h>

#define quit 17
#define ltor 27
#define rtol 22
#define led1 26
#define led2 6
#define led3 5
#define LED_amount 3

int btn_arr[] = {quit, ltor, rtol};
int LED_arr[] = {led1, led2, led3};
int button_func(int);
void cycle(int, int);
void interrupt(int);
long long time_func();

int main(void) {
  wiringPiSetupGpio();//necessary for wiringpi library to work
  int button = ltor;
  int last_state;

  pinMode(quit, INPUT);
  pinMode(rtol, INPUT);
  pinMode(ltor, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  while (button != quit) {
    last_state = button;
    button = button_func(last_state);

    if (button == ltor) {

      for (int i = 0; i < 3; i++) {

        cycle(LED_arr[i], last_state);

        button = button_func(last_state);//controls if lights keep cycling that direction
      }

    }

    else if (button == rtol) {
      for (int i = (LED_amount - 1); i >= 0; i--) { //as the loop goes DOWN the array, must subtract 1 from initial starting value to properly index array

        cycle(LED_arr[i], last_state);

        button = button_func(last_state); // controls if lights keep cycling that direction
      }
    }
  }
  return 0;
}


/*
Desc:Cycles through the LEDs
param a:the LED's position in the bar array
param b: the last known button state
side-effect:N/A
*/
void cycle(int LED_num, int last_state) {
  digitalWrite(LED_num, 1);
  interrupt(last_state);
  digitalWrite(LED_num, 0);
}

/*
Desc:Checks if a button is pressed on its respective GPIO pin based on a set array
param: last known button state
return: return the state of the button that is pressed based off of the array position, will return param if none is found
Side-effect:N/A
*/
int button_func(int last_state) {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(btn_arr[i]) == 1) {
      return btn_arr[i];
    }
  }
  return last_state;
}
/*
Desc:Retrieves the time in seconds elapsed from the linux epoch
side-effects:N/A
*/
long long time_func() {

  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec;
}
/*
Desc:calculates the desired delay between leds & allows for exiting if a button is pressed mid-cycle
Param:last button state seen
side-effects: unsure if time structure function with ints, so used long long to be safe
*/
void interrupt(int last_state) {
  long long delay_start = time_func();
  while ((time_func() - delay_start) < 5) {
    if (button_func(last_state) != last_state) {
      break;
    }
  }
}
