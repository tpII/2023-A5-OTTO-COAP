#include "Oscillator.h"
#include <Arduino.h>

// Prueba si hay que actualizar.
bool Oscillator::shouldUpdate()
{
  
  // Tiempo interno del microcontrolador desde el comienzo de su funcionamiento
  long now = millis();
 

  if(now - lastUpdate > UPDATE_PERIOD) {
    lastUpdate = now;   

    return true;
  }
  
  return false;
}

// Agregar servo
// Input: pin es el pin de arduino donde conecta
void Oscillator::attach(int pin) {
    servo.attach(pin,500,2400);
    // Calculo el numero de muestras que van a ser tomadas para el periodo
    // de un ciclo, y cada cuanto se actualiza
    float numSamples = (float)period / UPDATE_PERIOD;
    // Calculo cuantos radianes se mueve el servo en cada actualizacion
    phaseInc = 2*M_PI / numSamples;
    period=2000; 
    amplitude=45; 
    phase=0;
    currPhase=0;
    currPhase = 0;

}

// Quito el servo
void Oscillator::detach()
{
   //-- If the oscillator is attached, detach it.
  if(servo.attached())
        servo.detach();

}

// Seteo el periodo del oscilador en ms
void Oscillator::SetT(unsigned int T)
{
    //Asigno el periodo
    period=T;
    
    //Calculo nuevamente las variables
    float numSamples = (float)period / UPDATE_PERIOD;
    phaseInc = 2*M_PI / numSamples;
};

// Muevo el servo a la posicion indicada
void Oscillator::setPosition(int a) {

    // Seteo la posicion
    pos=a;
    servo.write(pos);

};


// Actualizar la pocision del servo
void Oscillator::update() {

    // Devuelve si es necesario actualizar
    if(!shouldUpdate())
        return;

    // Si el oscilador no esta parado, calculo la proxima posicion
        // Calculo que calcula la posicion en funcion del seno, que oscila entre
        // 0 y 1 dependiendo de la fase y los incrementos
    float pos = round(amplitude * sin(currPhase + phase) + offset);

        servo.write(pos + 90);
    // Incremento la fase
    currPhase = currPhase + phaseInc;

}

