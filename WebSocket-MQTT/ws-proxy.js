const mqtt = require('mqtt');
const WebSocket = require('websocket').server;
const http = require('http');

const mqttBroker = mqtt.connect('mqtt://192.168.0.139:1883'); // Reemplazar con la IP del broker

const server = http.createServer((request, response) => {
});

const wsServer = new WebSocket({
  httpServer: server,
});

wsServer.on('request', (request) => {
  const connection = request.accept(null, request.origin);
  console.log("server ON");

  connection.on('message', (message) => {
    if (message.type === 'utf8') {
      const data = JSON.parse(message.utf8Data);

      if (data.topic && data.message) {
        console.log("Se publicará en el tópico:", data.topic + " con el mensaje:", data.message);
        mqttBroker.publish(data.topic, data.message);
      }
    }
  });
});

server.listen(3000, () => {
  console.log('Servidor WebSocket corriendo en el puerto 3000');
  // Otros códigos o lógica adicional que puedas tener después de que el servidor comience a escuchar.
});

