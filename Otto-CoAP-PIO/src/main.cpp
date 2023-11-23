#include <Arduino.h>
#include "libreriaotto.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include "Thing.CoAP.h"
#include <iostream>

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

const char* ssid = "Fibertel WiFi595 2.4GHz";  // Parametros del AP
const char* password = "0141161689"; // 
const char* mqtt_server = "192.168.0.16"; //Parametros del broker MQTT
const uint16_t mqtt_server_port = 1883;    //
const char* mqttUser = "Otto";             //
const char* mqttPassword = "DefaultOtto";  //
const char* mqttTopicIn = "movimientos";          //
const char* mqttTopicOut = "otto-out";     //


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

WiFiUDP udp;


Thing::CoAP::Client coapClient;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;
bool respuestaRecibida = false;
Thing::CoAP::ObserveToken token;

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
    if (length >= 2) { //Transformo el mensaje a int
      payload[length] = '\0'; // Make payload a string by NULL terminating it.
      intValue = atoi((char *)payload);
      Serial.print(intValue);
    }
    // Obtengo la funcion del movimiento llamada por el mensaje
    f = otto.Otto::doActionsArray [intValue];
    // Invoco el movimiento
    (otto.*f) ();
  mqttClient.publish("mensaje","Recibido");
}
void observer(Thing::CoAP::Response response){
      Serial.println("Respuesta de observacion resivida");
      std::vector<uint8_t> payload = response.GetPayload();
      std::string received(payload.begin(), payload.end());
      Serial.println("Server envio el siguiente mensaje:");
      Serial.println(received.c_str());
      if(received.length()<=2){
        intValue = stoi(received);
      }
      f = otto.Otto::doActionsArray[intValue];
      (otto.*f)();
  }
void sendMessage(){
  //Make a post
  coapClient.Get("movimiento", "", [](Thing::CoAP::Response response){
      std::vector<uint8_t> payload = response.GetPayload();
      std::string received(payload.begin(), payload.end());
      Serial.println("Server envio el siguiente mensaje:");
      Serial.println(received.c_str());
      if(received.length()<=2){
        intValue = stoi(received);
      }
      f = otto.Otto::doActionsArray[intValue];
      (otto.*f)();
  });
}

void setup() {

    Serial.begin(9600);

    otto.init(PIERNA_I, PIERNA_D, PIE_D, PIE_I,TRIGER, ECHO);
    otto.home();

    delay(500);
    setup_Wifi();

    
    //Se realiza la configuracion del cliente mqtt para permitir la comunicacion con el broker
    mqttClient.setServer(mqtt_server,mqtt_server_port);
    mqttClient.setCallback(callback);
    //Configuracion del cliente CoAP y coneccion al servidor 
    coapClient.SetPacketProvider(udpProvider);
    IPAddress ip(192, 168, 0, 245);
    coapClient.Start(ip, 5683);
    Serial.println("Configurando observación del recurso 'movimiento'");
    token = coapClient.Observe("movimiento", observer);
}

void loop() {

  //Se utilizo para probar si cominicaba con el servidor CoAP
  //sendMessage();

  coapClient.Process();
  
  if(!mqttClient.connected()){
      reconnect();
    }
  mqttClient.loop();
  //Si se selecciono un movimiento que utiliza el ultrasonido 
  // se queda repitiendo ese movimiento haste que se seleccione otro
  if(intValue >= 20){
    (otto.*f)();
  }
  delay(5000);
}
