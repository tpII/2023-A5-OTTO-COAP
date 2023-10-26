#ifndef Oscillator_h
#define Oscillator_h

#include <Servo.h>
#include <math.h>


//Cantidad de tiempo a esperar entre las actualizaciones de la posicion hacia la posicion final
// En milisegundos
#define UPDATE_PERIOD 30

//-- Macro for converting from degrees to radians
#ifndef DEG2RAD
  #define DEG2RAD(g) ((g)*M_PI)/180
#endif

class Oscillator
{
  public:
    void attach(int pin);// Agrego el servo
    void detach();// Quitar el servo
    
    void SetA(unsigned int a) {amplitude=a;};
    void SetO(int o) {offset=o;};
    void SetPh(float Ph) {phase=Ph;};
    void SetT(unsigned int period);
    void setPosition(int position); 
    void Reset() {currPhase=0;};
    void update();
    int getPosition() { return pos;}

  private:
    // Revisa si hay que actualizar
    bool shouldUpdate();
    
  private:

    Servo servo;        // Objeto Servo
    unsigned int period;    // Tiempo total de una oscilasion en milisegundos
    unsigned int amplitude;  // Amplitud de la oscilasion entre 0 y 90 grados
    float phase;        // Fase inicial en la que inicia el ciclo
    int offset;      // Compensacion desde 0 grados
    float currPhase;    // Angulo actual de la fase
    float phaseInc;     // Valor para incrementar la fase en cada actualizacion
    bool attached=false;// True is attached, false otherwise
    float pos;            //Posicion en la que se encuentra el servo en grados
    long lastUpdate=0; // Ultima vez que se actualizo la posicion del servo


};

#endif
