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

const char *ssid = "Fibertel WiFi595 2.4GHz"; // Parametros del AP
const char *password = "0141161689";          //
const char *mqtt_server = "192.168.0.16";     // Parametros del broker MQTT
const uint16_t mqtt_server_port = 1883;       //
const char *mqttUser = "Otto";                //
const char *mqttPassword = "DefaultOtto";     //
const char *mqttTopicIn = "movimientos";      //
const char *mqttTopicOut = "otto-out";        //

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

WiFiUDP udp;

Coap coap(udp);

bool mqttValid = false;

// Funcion de seteado de wifi
// se realiza una sola vez
void setup_Wifi()
{

  Serial.print("Conectare a la red ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Conectando ...");
  }
  Serial.println("Conectado a WiFi");
}

// Funcion reconnect llamada
// para reconectar con el broker
void reconnect()
{
  while (!mqttClient.connected())
  {
    Serial.println("Intentando reconectar al servidor MQTT...");
    String mqttClientId = "A";
    // Intenta conectar al broker MQTT
    if (mqttClient.connect(mqttClientId.c_str(), mqttUser, mqttPassword))
    {
      Serial.println("Conexión exitosa al servidor MQTT");

      // Una vez conectado, puedes suscribirte a los tópicos necesarios
      mqttClient.subscribe(mqttTopicIn);
    }
    else
    {
      Serial.print("Fallo en la conexión, estado=");
      Serial.print(mqttClient.state());
      Serial.println(" Intentando de nuevo en 5 segundos...");

      // Espera antes de intentar la reconexión
      delay(5000);
    }
  }
}

// Funcion callback invocada cada vez
// que se recive un mensaje mqtt del broker
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived on topic: '");
  Serial.print(topic);
  Serial.print("' with payload: ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (length <= 2)
  {                         // Transformo el mensaje a int
    payload[length] = '\0'; // Make payload a string by NULL terminating it.
    intValue = atoi((char *)payload);
    Serial.print(intValue);
  }
  // Obtengo la funcion del movimiento llamada por el mensaje
  f = otto.Otto::doActionsArray[intValue];
  // Invoco el movimiento
  (otto.*f)();
  mqttClient.publish("mensaje", "Recibido");
}

// Funcio que maneja los response devido los request generados del cliente
void callback_reponse(CoapPacket &packet, IPAddress ip, int port)
{
  Serial.println("[Coap respuesta conseguida: ]");
  char dato[packet.payloadlen + 1];
  memcpy(dato, packet.payload, packet.payloadlen);
  dato[packet.payloadlen] = NULL;
  if (packet.payloadlen <= 2)
  {
    intValue = atoi((char *)dato); // Convierte el dato recivido en un int
    // Obtengo la funcion del movimiento recivida desde por el movimiento
    f = otto.Otto::doActionsArray[intValue];
    // Invoco funcion de movimiento
    (otto.*f)();
    Serial.println(dato);
  }
}

void setup()
{

  Serial.begin(9600);

  otto.init(PIERNA_I, PIERNA_D, PIE_D, PIE_I, TRIGER, ECHO);
  otto.home();

  delay(500);
  setup_Wifi();

  coap.response(callback_reponse);
  // Se realiza la configuracion del cliente mqtt para permitir la comunicacion con el broker
  mqttClient.setServer(mqtt_server, mqtt_server_port);
  mqttClient.setCallback(callback);

  coap.start();
}

void loop()
{
  coap.get(IPAddress(104, 196, 15, 150), 5683, "movimiento");
  coap.loop();
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop();
  // Si se selecciono un movimiento que utiliza el ultrasonido
  //  se queda repitiendo ese movimiento haste que se seleccione otro
  if (intValue >= 20)
  {
    (otto.*f)();
  }
}
