#ifndef Otto_h
#define Otto_h

#include <Servo.h>
#include "Oscillator.h"
#include <HCSR04.h>

//-- Constants
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30


class Otto 
{
  public:

    //Inicializacion Otto
    void init(int pierna_izq, int pierna_der, int pie_der, int pie_izq,int tr, int ec);
    void attachServos();
    void detachServos();

    // Funciones de movimientos
    void moveServos(int time, int  servo_target[]);
    void oscillateServos(int A[4], int O[4], int T, double phase_diff[4]);

 
    void home(); //Posicion de descanso
    bool getRestState(){return isOttoResting;};
    void setRestState(bool state){isOttoResting=state;};

    // Todos los movimientos del otto
    void walkForward();
    void walkBackward();
    void turnRight();
    void turnLeft();
    void bend ();
    void shakeLeg ();

    void updown();
    void swing(int steps=1, int T=1000,int h=30);
    void swingFix();
    void tiptoeSwing();

    void moonwalker();
    void crusaito();
    void flapping();

    void happy();
    void sad();
    void victory();
    void angry();
    void sleeping();
    void confused();
    void fart();
    void fail();

    void avoidObstacles();
    void followMode();
    void retrieve();
    
    // Vector de los movimientos
    typedef void (Otto::*function) ();
    function doActionsArray [23]={
      &Otto::home,
      &Otto::walkForward,
      &Otto::walkBackward,  
      &Otto::turnRight,
      &Otto::turnLeft,
      &Otto::bend,
      &Otto::shakeLeg,
      &Otto::moonwalker,
      &Otto::crusaito,
      &Otto::flapping,
      &Otto::swingFix,
      &Otto::tiptoeSwing,
      &Otto::updown,
      &Otto::happy,
      &Otto::sad,
      &Otto::victory,
      &Otto::sleeping,
      &Otto::confused,
      &Otto::fart,
      &Otto::fail,
      &Otto::followMode,
      &Otto::retrieve,
      &Otto::avoidObstacles, 
    };

  private:

    Oscillator servo[4];
    int servo_pins[4];
    UltraSonicDistanceSensor sensor;
    unsigned long final_time;
    unsigned long partial_time;
    float increment[4];

    bool isOttoResting;
    void execute(int A[4], int O[4], int T, double phase_diff[4], float steps);

};

#endif
