#include <Arduino.h>
#include "libreriaotto.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Thing.CoAP.h"
#include "PubSubClient.h"
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
bool activarO;
unsigned long coapRequestTime = 0;
unsigned long coapResponseTime = 0;

const char *ssid = "AC-Otto";              //"alumnosInfo";          // Parametros del AP
const char *password = "12345678";         //"Informatica2019";  //
const char *mqtt_server = "192.168.0.245"; //"163.10.142.82"; // Parametros del broker MQTT
const uint16_t mqtt_server_port = 1883;    //
const char *mqttUser = "Otto";             //
const char *mqttPassword = "DefaultOtto";  //
const char *mqttTopicIn = "movimientos";   //
const char *mqttTopicOut = "otto-out";     //

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

WiFiUDP udp;

Thing::CoAP::Client coapClient;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;

Thing::CoAP::ObserveToken token;

// Funcion de seteado de wifi
// se realiza una sola vez
void setup_Wifi()
{
  WiFi.mode(WIFI_AP);
  while (!WiFi.softAP(ssid, password))
  {
    Serial.println(".");
    delay(100);
  }
  Serial.print("Iniciado AP ");
  Serial.println(ssid);
  Serial.println(WiFi.softAPIP());
  /*Serial.print("Conectare a la red ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Conectando ...");
  }
  Serial.println("Conectado a WiFi");*/
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
  Serial.println("Message arrived on topic: '");
  Serial.print(topic);
  Serial.print("' with payload: ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  mqttClient.publish(mqttTopicOut, "recibido");
}
void observer(Thing::CoAP::Response response)
{
  // Registra el tiempo de recepción de la respuesta
  coapResponseTime = millis();

  Serial.println("Respuesta de observación recibida");
  std::vector<uint8_t> payload = response.GetPayload();
  std::string received(payload.begin(), payload.end());
  Serial.println("Server envió el siguiente mensaje:");
  Serial.println(received.c_str());
  // Calcula el Round Trip Time (RTT)
  unsigned long rtt = coapResponseTime - coapRequestTime;
  Serial.print("Round Trip Time (RTT): ");
  Serial.println(rtt);

  // Actualiza el tiempo de envío para la próxima solicitud
  coapRequestTime = millis();
}
void setup()
{

  Serial.begin(9600);

  otto.init(PIERNA_I, PIERNA_D, PIE_D, PIE_I, TRIGER, ECHO);
  otto.home();

  delay(500);
  setup_Wifi();

  // Se realiza la configuracion del cliente mqtt para permitir la comunicacion con el broker
  mqttClient.setServer(mqtt_server, mqtt_server_port);
  mqttClient.setCallback(callback);
  // Configuracion del cliente CoAP y coneccion al servidor
  coapClient.SetPacketProvider(udpProvider);
  IPAddress ip(192, 168, 0, 245); // CONFIGURAR IP COAP
  coapClient.Start(ip, 5683);
  // Configurando cliente CoAP para que observe el recurso movimientos
  coapRequestTime = millis();
  token = coapClient.Observe("movimientos", observer);
}

void loop()
{
  if (!mqttClient.connected())
  {
    reconnect();
    coapClient.CancelObserve(token);
    activarO = true;
  }
  if (activarO)
  {
    // Reinicia la observacion del recurso
    token = coapClient.Observe("movimientos", observer);
    activarO = false;
  }
  // Procesa la peticiones Coap
  coapClient.Process();
  // Procesa la peticiones mqtt
  mqttClient.loop();

  //  Si se selecciono un movimiento que utiliza el ultrasonido
  //   se queda repitiendo ese movimiento haste que se seleccione otro
  if (intValue >= 20)
  {
    (otto.*f)();
  }
  delay(5000);
}
