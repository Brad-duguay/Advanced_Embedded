/*
File:Lab6_masking.c
Author:Bradley Duguay W0516067
Date:2000/03/01
Description: GPIO functionality using pi registers & bitmasking to control GPIO states
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define quit 17
#define ltor 27
#define rtol 22
#define LED_1 26
#define LED_2 6
#define LED_3 5
#define sec_delay 1 
#define LED_amt 3
#define hi_reg 7
#define lo_reg 10
#define btn_reg 13

long long time_func();
void interrupt(int);
int btn_select(int);
void set_output(int);
void on_LED(int, int);
void off_LED(int, int);
int LED_num[] = {1, 2, 3};
int LED_pins[] = {26, 6, 5};

int btn_arr[] = {quit, rtol, ltor};

volatile unsigned int *GPIO;

main(void) {
  //VIRTUAL MEMORY SPACE FOR PI4
  unsigned int BASE = 0xFE200000;

  int MEM, MASK;
  int button = ltor;
  // TEST FOR ROOT ACCESS //
  if (getuid() != 0) {
    printf("ROOT PRIVILEGES REQUIRED\n");
    return 1;
  }
  // OPEN MEMORY INTERFACE //
  if ((MEM = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
    printf("CANNOT OPEN MEMORY INTERFACE\n");
    return 2;
  }
  // SET POINTER GPIO TO //
  // CONTROL MEMORY //
  // BASE ADDRESS (BASE) //
  GPIO = (unsigned int *)mmap(0, getpagesize(), PROT_READ | PROT_WRITE,
                              MAP_SHARED, MEM, BASE);
  if ((unsigned int)GPIO < 0) {
    printf("MEMORY MAPPING FAILED\n");
    return 3;
  }

  // SET GPIO 17 FOR INPUT //
  // 1111 1111 0001 1111 1111 1111 1111 1111 //
  // 0000 0000 0010 0000 0000 0000 0000 0000 //

  MASK = 0xFF1FFFFF;
  *(GPIO + 1) = *(GPIO + 1) & MASK;
  //  MASK = 0x00200000;
  // *(GPIO + 1) = *(GPIO + 1) | MASK;

  // SET GPIO 27&22 FOR INPUT //
  // 1111 1111 0001 1111 1111 1111 1111 1111 //
  MASK = 0xFF1FFE1F;
  *(GPIO + 2) = *(GPIO + 2) & MASK;

  // SET GPIO 6&5 FOR OUTPUT//
  MASK = 0xFE07FFF;
  *(GPIO + 0) = *(GPIO + 0) & MASK;

  MASK = 0x0048000;
  *(GPIO + 0) = *(GPIO + 0) | MASK;

  // SET GPIO 26 FOR OUTPUT//
  MASK = 0xFFE3FFFF;
  *(GPIO + 2) = *(GPIO + 2) & MASK;

  MASK = 0x00040000;
  *(GPIO + 2) = *(GPIO + 2) | MASK;


  do {
    button = btn_select(button);
    if (button == ltor) {
      for (int i = 0; i < LED_amt; i++) { // left to right

        on_LED(LED_pins[i], LED_num[i]);
        interrupt(button);
        off_LED(LED_pins[i], LED_num[i]);
        btn_select(button);
      }
    } else if (button == rtol)

      for (int i = (LED_amt - 1); i >= 0; i--) { // right to left, led amount must be one less to properly index the array

        on_LED(LED_pins[i], LED_num[i]);
        interrupt(button);
        off_LED(LED_pins[i], LED_num[i]);
        btn_select(button);
      }

  } while (button != quit);
  close(MEM);
}
/*
Desc: handles checking each button pin to see if a button is pressed
param a: the last seen button press
return: returns the value of the GPIO pin corresponding to the button
side-effect: N/A
*/


int btn_select(int last_button) {

  for (int i = 0; i < LED_amt; i++) {
    int MASK = 1;
    MASK = (1 << btn_arr[i]);
    int val = *(GPIO + btn_reg) & MASK;
    if (val != 0) {
      return btn_arr[i];
    }
  }
  return last_button;
}
/*
Desc: handles masking for turning the LEDs on
param a: the pin corresponding to the led to be toggled
side-effect: could be combined with off_LED function if GPIO pointer could be affected from a secondary parameter in main
*/

void on_LED(int pin,
            int position) { // bitshift a 1 bit to the bit position where the
                            // GPIO pin corresponds to the bit number
  int MASK = 1;
  MASK = (1 << pin);
  *(GPIO + hi_reg) = MASK;
  usleep(1000000);
  MASK = 0;
  *(GPIO + hi_reg) = MASK;
}
/*
Desc: handles masking for turning the LEDs off
param a: the pin corresponding to the led to be toggled
side-effect: could be combined with on_LED function if GPIO pointer could be affected from a secondary parameter in main
*/

void off_LED(int pin,
             int position) { // bitshift a 1 bit to the bit position where the
                             // GPIO pin corresponds to the bit number
  int MASK = 1;
  MASK = (1 << pin);
  *(GPIO + lo_reg) = MASK;
  usleep(100000);
  MASK = ~MASK;
  *(GPIO + lo_reg) = *(GPIO + lo_reg) & MASK;
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
Desc:calculates the desired delay between leds & allows for exiting if a button
is pressed mid-cycle Param:last button state seen side-effects: unsure if time
structure function with ints, so used long long to be safe
*/
void interrupt(int last_state) {
  long long delay_start = time_func();
  while ((time_func() - delay_start) < sec_delay) {
    if (btn_select(last_state) != last_state) {
      break;
    }
  }
}
