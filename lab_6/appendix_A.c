// P15 - PROGRAM GPIO MEM CONTROL //
// Blink GPIO-17 until a HIGH on GPIO-27. //
// PI 3 - GPIO memory starts at 0x3F200000 //
// PI 4 - GPIO memory starts at 0xFE200000 //
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
  // USE VIRTUAL MEMORY SPACE FOR PI 3 //
  // unsigned int BASE = 0x3F200000;
  unsigned int BASE = 0xFE200000;
  // CREATE GPIO - A 4 BYTE //
  // POINTER. INCREMENTING //
  // GPIO BY 1 INCREASES THE //
  // POINTER ADDRESS BY 4 //
  //  volatile unsigned int *GPIO;
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
      for (int i = 0; i < 3; i++) { // left to right

        on_LED(LED_pins[i], LED_num[i]);

        // sleep(sec_delay);
        interrupt(button);
        off_LED(LED_pins[i], LED_num[i]);

        btn_select(button);
      }
    } else if (button == rtol)

      for (int i = 2; i >= 0; i--) { // right to left

        on_LED(LED_pins[i], LED_num[i]);

        // sleep(sec_delay);
        interrupt(button);
        off_LED(LED_pins[i], LED_num[i]);
        btn_select(button);
      }

  } while (button != quit);
  close(MEM);
}
int btn_select(int last_button) {

  for (int i = 0; i < 3; i++) {
    int MASK = 1;
    MASK = (1 << btn_arr[i]);
    int val = *(GPIO + 13) & MASK;
    if (val != 0) {
      return btn_arr[i];
    }
  }
  return last_button;
}

void on_LED(int pin,
            int position) { // bitshift a 1 bit to the bit position where the
                            // GPIO pin corresponds to the bit number
  int MASK = 1;
  MASK = (1 << pin);

  // SET GPIO HIGH //
  // 0000 0000 0000 0000 0000 0000 0010 0000 //
  *(GPIO + 7) = MASK;
  usleep(1000000);
  // REMOVE HIGH COMMAND //
  // 1111 1111 1111 1111 1111 1111 1101 1111 //
  MASK = 0;
  *(GPIO + 7) = MASK;
}

void off_LED(int pin,
             int position) { // bitshift a 1 bit to the bit position where the
                             // GPIO pin corresponds to the bit number
  int MASK = 1;
  MASK = (1 << pin);

  // SET GPIO low //
  // 0000 0000 0000 0000 0000 0000 0010 0000 //
  *(GPIO + 10) = MASK;
  usleep(100000);
  // REMOVE low COMMAND //
  // 1111 1111 1111 1111 1111 1111 1101 1111 //
  MASK = ~MASK;
  *(GPIO + 10) = *(GPIO + 10) & MASK;
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
  while ((time_func() - delay_start) < 5) {
    if (btn_select(last_state) != last_state) {
      break;
    }
  }
}
