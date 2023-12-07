#include "Arduino.h"
#include "libreriaotto.h"
#include "Oscillator.h"
#include "HCSR04.h"

// Seteo los pines de cada servo y desactivo el modo de descanso
void Otto::init(int pierna_izq, int pierna_der, int pie_der, int pie_izq, int tr, int ec)
{

  servo_pins[0] = pierna_izq;
  servo_pins[1] = pierna_der;
  servo_pins[2] = pie_der;
  servo_pins[3] = pie_izq;

  sensor.Init(tr, ec);

  attachServos();
  isOttoResting = false;
  home();
}

void Otto::attachServos()
{
  servo[0].attach(servo_pins[0]);
  servo[1].attach(servo_pins[1]);
  servo[2].attach(servo_pins[2]);
  servo[3].attach(servo_pins[3]);
}

void Otto::detachServos()
{
  servo[0].detach();
  servo[1].detach();
  servo[2].detach();
  servo[3].detach();
}

// Funcion que mueve los cuatro servos a la posicion indicada
// en la cantidad de tiempo indicada por el parametro time
void Otto::moveServos(int time, int servo_target[])
{

  attachServos();
  // Si la flag de descanso esta en true la cambio a false
  if (getRestState() == true)
  {
    setRestState(false);
  }
  // Calculo el tiempo en el que finalizar los movimientos
  final_time = millis() + time;
  // Si el tiempo pasado como parametro es mayor a 10 ms
  if (time > 10)
  {
    for (int i = 0; i < 4; i++)
    {
      // Calculo del valor parcial con el que se incrementa la posicion de los servos
      // para que el movimiento sea mas fluido. El calculo se realiza para incrementar
      //  cada 10 ms
      increment[i] = (servo_target[i] - servo[i].getPosition()) / (time / 10.0);
    }
    // For para incrementar las posiciones de los servos hasta que se cumpla el tiempo
    // en el que finalizar los movimientos
    for (int iteration = 1; millis() < final_time; iteration++)
    {
      partial_time = millis() + 10; // Calculo el tiempo total del incremento si dura 10 ms
      for (int i = 0; i < 4; i++)
      {
        servo[i].setPosition(servo[i].getPosition() + increment[i]); // Incremento las posicion de cada servo
      }
      while (millis() < partial_time)
        ; // Pausa para completar el tiempo calculado
    }
  }
  else
  {
    // Si el tiempo es menor a 10 ms muevo todos los servos a la posicion final sin incrementos parciales
    for (int i = 0; i < 4; i++)
    {
      servo[i].setPosition(servo_target[i]);
    }
    while (millis() < final_time)
      ; // Pausa para completar el tiempo
  }

  // Ajustes finales si el movimiento no se completo en el tiempo indicado
  bool f = true;
  while (f)
  {
    f = false;
    for (int i = 0; i < 4; i++)
    {
      // Reviso si los servos llegaron a la posicion deseada
      if (servo_target[i] != servo[i].getPosition())
      {
        f = true;
        break;
      }
    }
    if (f)
    {
      // Muevo los servos a la posicion indicada
      for (int i = 0; i < 4; i++)
      {
        servo[i].setPosition(servo_target[i]);
      }
      // Espero 10 ms
      partial_time = millis() + 10;
      while (millis() < partial_time)
        ; // pause
    }
  };
}
// Oscilo los servos en la amplitud dada
void Otto::oscillateServos(int A[4], int O[4], int T, double phase_diff[4])
{
  // Seteo los parametros para configurar la funcion sin
  for (int i = 0; i < 4; i++)
  {
    servo[i].SetO(O[i]);
    servo[i].SetA(A[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }
  double ref = millis();
  // Oscilo durante el tiempo indicado por la variable T
  for (double x = ref; x <= T + ref; x = millis())
  {
    for (int i = 0; i < 4; i++)
    {
      servo[i].update();
    }
  }
}
// Funcion que ejectuta la cantidad de ciclos a oscilar
void Otto::execute(int A[4], int O[4], int T, double phase_diff[4], float steps = 1.0)
{

  attachServos();
  if (getRestState() == true)
  {
    setRestState(false);
  }

  int cycles = (int)steps;

  // Ejecuto ciclos completos
  if (cycles >= 1)
    for (int i = 0; i < cycles; i++)
      oscillateServos(A, O, T, phase_diff);

  // Ejecuto el ciclo final
  oscillateServos(A, O, T, phase_diff);
}

// Posicion de descanso del Otto
void Otto::home()
{

  if (isOttoResting == false)
  { // Va a la posicion de descanso si es necesario

    int homes[4] = {90, 90, 90, 90}; // Posiciones de descanso
    moveServos(800, homes);          // Muevo los servos en medio segundo

    delay(1000);
    detachServos();
    isOttoResting = true;
  }
}

//---------------------------------------------------------
//-- walkForward: Caminar hacia delante
//---------------------------------------------------------
void Otto::walkForward()
{
  //-- Piernas y pies estan 90 grados fuera de fase
  //--      -90 : Caminar hacia delante
  //--       90 : Caminar hacia atras
  int T = 1000;  // Periodo por paso
  int steps = 1; // Cantidad de pasos
  int A[4] = {30, 30, 20, 20};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {0, 0, DEG2RAD(FORWARD * -90), DEG2RAD(FORWARD * -90)};

  execute(A, O, T, phase_diff, steps);
}
//---------------------------------------------------------
//-- walkBackward: Caminar hacia delante
//---------------------------------------------------------
void Otto::walkBackward()
{
  //-- Piernas y pies estan 90 grados fuera de fase
  //--      -90 : Caminar hacia delante
  //--       90 : Caminar hacia atras
  int T = 1000;  // Periodo por paso
  int steps = 2; // Cantidad de pasos
  int A[4] = {30, 30, 20, 20};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {0, 0, DEG2RAD(BACKWARD * -90), DEG2RAD(BACKWARD * -90)};

  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- turn right
//---------------------------------------------------------
void Otto::turnRight()
{
  //-- Las amplitudes de las piernas no son iguales
  //-- Las amplitudes de una pierna son mayores que la otra,
  //-- permitiendo que los pasos sean mayores en la pierna con
  //-- mayo amplitud
  int T = 1000;
  int steps = 2;
  int A[4] = {10, 30, 20, 20};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)};

  execute(A, O, T, phase_diff, steps);
}
//---------------------------------------------------------
//-- turn left
//---------------------------------------------------------
void Otto::turnLeft()
{
  //-- Las amplitudes de las piernas no son iguales
  //-- Las amplitudes de una pierna son mayores que la otra,
  //-- permitiendo que los pasos sean mayores en la pierna con
  //-- mayo amplitud
  int T = 1000;
  int steps = 2;
  int A[4] = {30, 10, 20, 20};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)};

  execute(A, O, T, phase_diff, steps);
}
//---------------------------------------------------------
//-- bend
//---------------------------------------------------------
void Otto::bend()
{
  int T = 1400;
  int steps = 1;
  // Parametros para left bend por defecto
  int bend1[4] = {90, 90, 75, 35};
  int bend2[4] = {90, 90, 75, 105};
  int homes[4] = {90, 90, 90, 90};

  // Tiempo de bend para evitar caidas
  int T2 = 800;

  // Bend
  for (int i = 0; i < steps; i++)
  {
    moveServos(T2 / 2, bend1);
    moveServos(T2 / 2, bend2);
    delay(T * 0.8);
    moveServos(500, homes);
  }
}

//---------------------------------------------------------
//-- shakeLeg
//---------------------------------------------------------
void Otto::shakeLeg()
{

  int numberLegMoves = 2; // Cantidad de shakes
  int T = 2000;
  int steps = 1;
  // Parametros para shake pierna derecha
  int shake_leg1[4] = {90, 90, 63, 35};
  int shake_leg2[4] = {90, 90, 63, 120};
  int shake_leg3[4] = {90, 90, 63, 60};
  int homes[4] = {90, 90, 90, 90};

  // Tiempo para el movimiento de inclinarse, para evitar caidas
  int T2 = 1000;
  // Tiempo de shake
  T = T - T2;
  T = max(T, 200 * numberLegMoves);

  for (int j = 0; j < steps; j++)
  {
    // Inclinarse
    moveServos(T2 / 2, shake_leg1);
    moveServos(T2 / 2, shake_leg2);

    // Shake
    for (int i = 0; i < numberLegMoves; i++)
    {
      moveServos(T / (2 * numberLegMoves), shake_leg3);
      moveServos(T / (2 * numberLegMoves), shake_leg2);
    }
    moveServos(500, homes); // Return to home position
  }

  delay(T);
}

//---------------------------------------------------------
//--  up & down
//---------------------------------------------------------
void Otto::updown()
{
  int h = 25;
  int T = 1000;
  int steps = 1;
  //-- Los dos pies estan desfasados 180 grados, para que hagan el movimiento contrario

  int A[4] = {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(90)};

  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- swingFix: Igual que el swing, pero con los parametros fijos
//---------------------------------------------------------
void Otto::swingFix()
{
  //-- Los dos pies tienen la misma amplitud y su offset es la mitad de la amplitud
  int T = 1000;
  int steps = 1;
  int h = 30;
  int A[4] = {0, 0, h, h};
  int O[4] = {0, 0, h / 2, -h / 2};
  double phase_diff[4] = {0, 0, DEG2RAD(0), DEG2RAD(0)};

  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- swing: balancearse de lado a lado
//--  Parametros:
//--    steps:  numero de pasos
//--    T : periodo
//--    h: altura
//---------------------------------------------------------
void Otto::swing(int steps, int T, int h)
{
  //-- Los dos pies tienen la misma amplitud y su offset es la mitad de la amplitud
  int A[4] = {0, 0, h, h};
  int O[4] = {0, 0, h / 2, -h / 2};
  double phase_diff[4] = {0, 0, DEG2RAD(0), DEG2RAD(0)};

  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- tiptoeSwing
//---------------------------------------------------------
void Otto::tiptoeSwing()
{
  int h = 25;
  int T = 900;
  int steps = 1;
  // Las dos piernas tienen la misma amplitud, y estan desfasadas 180 grados para que hagan el movimiento contrario.
  int A[4] = {h, h, 0, 0};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), 0, 0};

  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- moonwalker
//---------------------------------------------------------
void Otto::moonwalker()
{
  //-- Es una onda moviendose de un servo a otro.
  //-- La diferencia de fase entre los osciladores es de 60 grados y las amplitudes iguales
  int T = 900;
  int steps = 1;
  int h = 20; // Altura
  int A[4] = {0, 0, h, h};
  int O[4] = {0, 0, h / 2 + 2, -h / 2 - 2};
  int phi = -LEFT * 90;
  double phase_diff[4] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * LEFT + phi)};

  execute(A, O, T, phase_diff, steps);
}

//----------------------------------------------------------
//-- crusaito
//-----------------------------------------------------------
void Otto::crusaito()
{
  int h = 20; // Altura
  int T = 900;
  int steps = 1;
  int A[4] = {25, 25, h, h};
  int O[4] = {0, 0, h / 2 + 4, -h / 2 - 4};
  double phase_diff[4] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * FORWARD)};

  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- flapping
//---------------------------------------------------------
void Otto::flapping()
{
  int h = 25; // Altura
  int T = 1000;
  int steps = 1;
  int A[4] = {12, 12, h, h};
  int O[4] = {0, 0, h - 10, -h + 10};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * FORWARD), DEG2RAD(90 * FORWARD)};

  execute(A, O, T, phase_diff, steps);
}

void Otto::happy()
{

  swing(1, 800, 20);
}

void Otto::sad()
{
  int mSad[4] = {110, 70, 20, 160};
  moveServos(700, mSad);
}

void Otto::victory()
{
  int vic[4] = {90, 90, 150, 30};
  int vic2[4] = {90, 90, 130, 90};
  int vic3[4] = {90, 90, 90, 50};
  int vic4[4] = {90, 90, 130, 50};
  int homes[4] = {90, 90, 90, 90};
  moveServos(1000, vic);
  moveServos(600, homes);
  moveServos(300, vic2);
  moveServos(300, vic3);
  moveServos(300, vic4);
  moveServos(600, homes);
}

void Otto::angry()
{
  int angry1[4] = {110, 110, 90, 90};
  int angry2[4] = {70, 70, 90, 90};
  int angry3[4] = {90, 90, 70, 110};
  moveServos(1000, angry1);
  moveServos(1000, angry2);
  moveServos(2000, angry3);
}

void Otto::sleeping()
{
  int sleep[4] = {100, 80, 60, 120};
  moveServos(2000, sleep);
}

void Otto::confused()
{
  int conf[4] = {110, 70, 90, 90};
  moveServos(300, conf);
  delay(500);
}
void Otto::fart()
{
  int fart1[4] = {90, 90, 145, 122};
  int fart2[4] = {90, 90, 80, 110};
  int fart3[4] = {90, 90, 145, 80};
  int homes[4] = {90, 90, 90, 90};
  moveServos(500, fart1);
  delay(500);
  moveServos(500, fart2);
  delay(500);
  moveServos(500, fart3);
  delay(1000);
  moveServos(500, homes);
}
void Otto::fail()
{
  int fail1[4] = {90, 90, 70, 35};
  int fail2[4] = {90, 90, 55, 35};
  int fail3[4] = {90, 90, 42, 35};
  int fail4[4] = {90, 90, 34, 35};
  moveServos(300, fail1);
  moveServos(300, fail2);
  moveServos(300, fail3);
  moveServos(300, fail4);
}
//-- avoidObstacles
//-- Otto camina si no detecta nada a una distancia definida por la variable distance
//-- Si detecta un objeto da dos pasos hacia atras y gira a la izquierda
void Otto::avoidObstacles()
{
  int distance = 10; // Distancia de objeto
  if (sensor.measureDistanceCm() <= distance)
  {

    confused();
    delay(600);
    walkBackward(); // BACKWARD x2
    walkBackward();
    turnLeft(); // LEFT x2
    turnLeft();
  }
  walkForward(); // FORWARD x1
}
//-- followMode
//-- Funcion que sigue a un objeto que se encuentre a un rango de 15-30cm
void Otto::followMode()
{
  int min = 15; // Distancia minima de objeto
  int max = 20; // Distancia maxima del objeto

  if (sensor.measureDistanceCm() < max)
  {
    if (sensor.measureDistanceCm() > min)
    {
      walkForward(); // FORWARD x1
      delay(500);
    }
  }
}

//-- retrieve
//-- Otto camina hacia atras si detecta un objeto a menor distancia
//-- que la variable distancia
void Otto::retrieve()
{
  int distance = 15; // Distancia de objeto
  if (sensor.measureDistanceCm() <= distance)
  {

    walkBackward(); // BACKWARD x1
    delay(500);
  }
}
