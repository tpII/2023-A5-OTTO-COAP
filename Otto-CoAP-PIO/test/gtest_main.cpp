#include <gtest/gtest.h>
#include <Arduino.h>
#include <main.h>
#include <PubSubClient.h>
#include <libreriaotto.h>
#include <Thing.CoAP.h>

extern PubSubClient mqttClient;
int intValue;
Otto::function f;

class MockCoAPResponse : public Thing::CoAP::Response
{
};
TEST(setup_Wifi, test_conexion_exitosa)
{
    setup_Wifi();
    ASSERT_EQ(WL_CONNECTED, WiFi.status());
}
TEST(callback_observer_test, Response_de_un_Movimiento)
{
    // Configuración del escenario de prueba
    std::vector<uint8_t> payloadValido = {6};
    MockCoAPResponse respuesta;
    respuesta.SetPayload(payloadValido);

    // Llamar a la función observer con respuesta simulada
    observer(respuesta);

    // Verificamos el valor intValue se el valor de un indice de un movimiento
    // y que f no sea null
    ASSERT_LE(intValue, 23);
    ASSERT_NE(f, nullptr);
}
TEST(callback_observer_test, Response_de_Movimiento_que_no_extiste)
{
    // Configuración del escenario de prueba
    std::vector<uint8_t> payloadNoValido = {45};
    MockCoAPResponse respuesta;
    respuesta.SetPayload(payloadNoValido);

    // Llamar a la función observer con respuesta simulada
    observer(respuesta);

    // Verificamos el comportamiento
    ASSERT_EQ(f, nullptr);
}
TEST(callback_observer_test, ResponsePayloadNoValido)
{
    std::vector<uint8_t> payloadNoValido = {1, 2, 4, 8};
    MockCoAPResponse respuesta;
    respuesta.SetPayload(payloadNoValido);

    observer(respuesta);

    // Verificamos el comportamiento
    ASSERT_EQ(f, nullptr);
}
TEST(callback_observer_test, ResponsePayloadEmpty)
{
    std::vector<uint8_t> payloadVacio;
    MockCoAPResponse respuesta;
    respuesta.SetPayload(payloadVacio);

    observer(respuesta);

    ASSERT_EQ(0, intValue);
    ASSERT_EQ(f, nullptr);
}
TEST(callback_test, mensaje_mqtt_movimiento_valido)
{
    char topic[] = "movimientos";
    byte payload[] = {1, 2};
    unsigned int length = sizeof(payload) / sizeof(payload[0]); // Calcular el tamaño del array
    callback(topic, payload, length);

    ASSERT_LE(intValue, 23);
    ASSERT_NE(f, nullptr);
}
TEST(callback_test, mensaje_mqtt_movimiento_no_valido)
{
    char topic[] = "movimientos";
    byte payload[] = {4, 6};
    unsigned int length = sizeof(payload) / sizeof(payload[0]); // Calcular el tamaño del array
    callback(topic, payload, length);

    ASSERT_EQ(f, nullptr);
}
TEST(callback_test, mensaje_mqtt_payload_no_valido)
{
    char topic[] = "movimientos";
    byte payload[] = {1, 2, 3};
    unsigned int length = sizeof(payload) / sizeof(payload[0]); // Calcular el tamaño del array
    callback(topic, payload, length);

    ASSERT_EQ(f, nullptr);
}

TEST(callback_test, mensaje_mqtt_payload_empty)
{
    char topic[] = "movimientos";
    byte payload[] = {};
    unsigned int length = sizeof(payload) / sizeof(payload[0]); // Calcular el tamaño del array
    callback(topic, payload, length);
    ASSERT_EQ(0, intValue);
    ASSERT_EQ(f, nullptr);
}
TEST(reconnect_test, reconeccion_exitosa)
{
    reconnect();
    ASSERT_EQ(true, mqttClient.connected());
}
void setup()
{
    // Deberías inicializar aquí cualquier configuración adicional que necesites para tus pruebas.

    // Inicializar Google Test
    ::testing::InitGoogleTest();

    // Configurar la salida XML para los resultados de las pruebas
    ::testing::GTEST_FLAG(output) = "xml:test_results/";

    // Ejecutar todas las pruebas
    int result = RUN_ALL_TESTS();

    // Comprobar el resultado de las pruebas
    if (result == 0)
    {
        Serial.println("Todas las pruebas pasaron.");
    }
    else
    {
        Serial.println("Al menos una prueba falló.");
    }

    // Salir del programa después de ejecutar las pruebas
    ESP.deepSleep(0); // Esto hará que el programa se detenga y entre en el modo de bajo consumo.
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    /* Serial.begin(115200);
     ::testing::InitGoogleTest();
      // Run tests
      ::testing::GTEST_FLAG(output) = "xml:test_results/"; // "xml:C:\\Users\\Comerci\\Desktop\\2023-A5-OTTO-COAP\\Otto-CoAP-Angular\\src\\assets\\informe\\test_results.xml"
     if (RUN_ALL_TESTS())
         ;

     // sleep for 1 sec
     delay(1000);*/
}

void loop()
{
}

/*
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);


    return RUN_ALL_TESTS();
}*/