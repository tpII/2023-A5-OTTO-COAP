#include<Arduino.h>
#include<Thing.CoAP.h>

// Declaración de prototipos de funciones
void callback(char *topic, byte *payload, unsigned int length);// Es la funcion para procesar las publicaciones de MQTT
void observer(Thing::CoAP::Response response);// Es la funcion para pocesar las respuesta de las observaciones de CoAP

