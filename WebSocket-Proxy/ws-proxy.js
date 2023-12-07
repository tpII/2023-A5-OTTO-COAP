const mqtt = require('mqtt');
const WebSocket = require('websocket').server;
const http = require('http');
const coap = require('node-coap-client').CoapClient;

const mqttBroker = mqtt.connect('mqtt://163.10.140.180:1883'); // Reemplazar con la IP del broker

const server = http.createServer((request, response) => {
});

const wsServer = new WebSocket({
  httpServer: server,
});
var isCoap= true;

wsServer.on('request', (request) => {
  const connection = request.accept(null, request.origin);
  console.log("server ON");

  connection.on('message', (message) => {
    if (message.type === 'utf8') {
      const data = JSON.parse(message.utf8Data);
      if (data.topic && data.message) {
        if(data.topic==="protocolo"){ // verifico el protocolo activo
          if(data.message==="CoAP"){
          console.log("coap activado");
            isCoap=true;
          }
          else{ isCoap=false; 
            console.log("MQTT activado");}
          }
        else{ //es para el topico movimientos
          if(isCoap){
          console.log(isCoap)
            sendCoAPMessage(data.topic, data.message);}
          else{
            console.log("Se publicará en el tópico:", data.topic + " con el mensaje:", data.message);
            mqttBroker.publish(data.topic, data.message);
        }
        }
      }
      }
    });
  });

  async function sendCoAPMessage(topic, message) {
    try {
      const host= '163.10.140.180';
      const port= 5683;
      const url = `coap://${host}:${port}/movimientos`//${topic}`;
      console.log(`${url}`+" servicio de conexion al coap con la info : ",Buffer.from(message));
      const response = await coap.request(url,'post',Buffer.from(message)); //Se envia al server
      console.log('Respuesta de CoAP:', response.code);
      console.log("Se envió el movimiento al server CoAP: ", message);
    } catch (error) {
      console.error('Error al enviar el mensaje CoAP:', error);
    }
  }
//Servidor escuchando en puerto
server.listen(3000, () => {
  console.log('Servidor WebSocket corriendo en el puerto 3000');
  
});

