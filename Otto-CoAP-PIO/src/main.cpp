#include <Arduino.h>
#include "libreriaotto.h"

#define PIERNA_D D4
#define PIE_D D5
#define PIERNA_I D7
#define PIE_I D8
#define TRIGER D1
#define ECHO D2

Otto otto;

void setup() {
    otto.init(PIERNA_I, PIERNA_D, PIE_D, PIE_I,TRIGER, ECHO);
    otto.home();
}

void loop() {
  
  otto.turnRight();
  
}
