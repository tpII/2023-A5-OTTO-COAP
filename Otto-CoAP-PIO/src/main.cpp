#include <Arduino.h>
#include "libreriaotto.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <coap-simple.h>

// Configuracion

#define PIERNA_D D4
#define PIE_D D5
#define PIERNA_I D7
#define PIE_I D8
#define TRIGER D1
#define ECHO D2

Otto otto;
Otto::function f;
int intValue = 0;

const char* ssid = "";             // Parametros del AP
const char* password = "";     // 
const char* mqtt_server = ""; //Parametros del broker MQTT
const uint16_t mqtt_server_port = 1883;    //
const char* mqttUser = "Otto";             //
const char* mqttPassword = "DefaultOtto";  //
const char* mqttTopicIn = "otto";          //
const char* mqttTopicOut = "otto-out";     //

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Coap coap;




bool mqttValid = false;

//Funcion de seteado de wifi
//se realiza una sola vez
void setup_Wifi(){
  
  Serial.print("Conectare a la red ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ...");
  }
  Serial.println("Conectado a WiFi");
}

//Funcion reconnect llamada
//para reconectar con el broker
void reconnect(){
  while (!mqttClient.connected()) {
    Serial.println("Intentando reconectar al servidor MQTT...");
    String mqttClientId ="A"; 
    // Intenta conectar al broker MQTT
    if (mqttClient.connect(mqttClientId.c_str(),mqttUser,mqttPassword)) {
      Serial.println("Conexión exitosa al servidor MQTT");

      // Una vez conectado, puedes suscribirte a los tópicos necesarios
      mqttClient.subscribe(mqttTopicIn);
    } else {
      Serial.print("Fallo en la conexión, estado=");
      Serial.print(mqttClient.state());
      Serial.println(" Intentando de nuevo en 5 segundos...");
      
      // Espera antes de intentar la reconexión
      delay(5000);
    }
  } 
}

//Funcion callback invocada cada vez
//que se recive un mensaje mqtt del broker
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived on topic: '");
  Serial.print(topic);
  Serial.print("' with payload: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if(topic=="movimiento"){
    if (length >= 2) { //Transformo el mensaje a int
      payload[length] = '\0'; // Make payload a string by NULL terminating it.
      intValue = atoi((char *)payload);
      Serial.print(intValue);
    }
    // Obtengo la funcion del movimiento llamada por el mensaje
    f = otto.Otto::doActionsArray [intValue];
    // Invoco el movimiento
    (otto.*f) ();
  }else{
    { 
      payload[length] = '\0'; // Make payload a string by NULL terminating it.
      mqttValid = ((char *)payload =="true");
    }
  }
  mqttClient.publish("mensaje","Recibido");
}

//Funcion que realiza el manejo de mensajes CoAP para el movimiento
void callback_movimiento(CoapPacket &packet, IPAddress ip, int port) {
  char dato[packet.payloadlen + 1];

 if (packet.code == COAP_POST) {
    // Maneja solicitudes POST
    Serial.print("' with payload: ");
    Serial.print((char)packet.payload);

    if (packet.payloadlen>=2){
      memcpy(dato, packet.payload, packet.payloadlen);
      dato[packet.payloadlen] = NULL;
      intValue = atoi((char*)dato);//Convierte el dato recivido en un int
    }
    //Obtengo la funcion del movimiento recivida desde por el movimiento
    f = otto.Otto::doActionsArray[intValue];
    //Invoco funcion de movimiento 
    (otto.*f)(); 
    coap.sendResponse(ip, port, packet.messageid,"Recibido");
  }
}
void callback_protocolo(CoapPacket &packet, IPAddress ip, int port) {
  char dato[packet.payloadlen + 1];

 if (packet.code == COAP_POST) {
    // Maneja solicitud para el cambion de protocolo de comunicacion
    Serial.print("' with payload: ");
    Serial.print((char)packet.payload);
    memcpy(dato, packet.payload, packet.payloadlen);
    dato[packet.payloadlen] = NULL;

      mqttValid = ((char *)dato =="true");
  }
  /*if (mqttValid) {
    coap.sendResponse(ip, port, packet.messageid, "Se a decido por MQTT");
  }*/
  coap.sendResponse(ip, port, packet.messageid,"Recibido");
}


void setup() {

    Serial.begin(9600);

    otto.init(PIERNA_I, PIERNA_D, PIE_D, PIE_I,TRIGER, ECHO);
    otto.home();

    delay(500);
    setup_Wifi();

    //Agrega los punto de acceso para para la cominicacion con CoAP
    coap.server(callback_movimiento,"movimiento");
    coap.server(callback_protocolo,"protocolo");

    //Se realiza la configuracion del cliente mqtt para permitir la comunicacion con el broker
    mqttClient.setServer(mqtt_server,mqtt_server_port);
    mqttClient.setCallback(callback);

    coap.start();

}

void loop() {
  if(mqttValid==false){
    coap.loop();
  }else{ 
    if(!mqttClient.connected()){
      reconnect();
    }
    mqttClient.loop(); 
  }
  //Si se selecciono un movimiento que utiliza el ultrasonido 
  // se queda repitiendo ese movimiento haste que se seleccione otro
  if(intValue >= 20){
    (otto.*f)();
  } 
}
