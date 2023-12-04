#include<Arduino.h>
#include<Thing.CoAP.h>
#include<PubSubClient.h>

// Declaración de prototipos de funciones
void setup_Wifi();//Se encarga de conectar el esp8266 a la red wifi
void reconnect();//Se encarga de reconectar en el cliente mqtt si este se desconecta del servidor
void c_respuesta(char *topic, byte *payload, unsigned int length);// Es la funcion para procesar las publicaciones de MQTT
void c_observer(Thing::CoAP::Response response);// Es la funcion para pocesar las respuesta de las observaciones de CoAP
void sendMessage();// Funcion encargada de enviar una peticion get al servidor CoAP
