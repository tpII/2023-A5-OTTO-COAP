const coap = require('coap');
const cors = require('cors');
const express = require('express');

const app = express();
app.use(cors({ origin: 'http://192.168.0.160:4200' })); //colocar direccion ip


const server = coap.createServer();

server.on('request', (req, res) => {
  const method = req.method; // Método de la solicitud (GET, POST, etc.)
  const url = req.url; // URL de la solicitud
  const payload = req.payload.toString(); // Cuerpo de la solicitud
  res.setHeader('Access-Control-Allow-Origin', 'http://192.168.0.160:4200');
  
  console.log(`Recibida solicitud ${method} en ${url} con payload: ${payload}`);

  // Procesa el mensaje según el tipo de solicitud
  if (url === '/message' && method === 'POST') {
    // Verifica que el payload sea válido
    if (payload === 'true' || typeof payload === 'string') {
      sendMessage(payload)// manena la logica del envio del mensaje
        .then((resNodeMCU) => {
          // Envía la respuesta del NodeMCU como respuesta CoAP
          res.setOption('Content-Type', 'text/plain');
          res.end(resNodeMCU);
        })
        .catch((error) => {
          console.error('Error al comunicarse con el NodeMCU:', error);
          // Si hay un error, envía una respuesta de error
          res.statusCode = 500; // Código de estado 500 (Internal Server Error)
          res.end('Error interno al comunicarse con el NodeMCU');
        });

    } else {
      // Si el payload no es válido, envía una respuesta de error
      res.statusCode = 400; // Código de estado 400 (Bad Request)
      res.end('Payload no válido');
    }
  } else {
    // Si la URL o el método no coinciden con lo esperado, envía una respuesta de error
    res.statusCode = 404; // Código de estado 404 (Not Found)
    res.end('Recurso no encontrado');
  }
});
const puerto = 5683;
const direccionIP = '0.0.0.0'; // Escucha en todas las interfaces

//Servidor escuchando en la direccion y puerto indicado
server.listen( puerto, direccionIP, () =>{
  
    console.log(`Servidor CoAP escuchando en coap://${direccionIP}:${puerto}`);}
);


// Función para enviar el mensaje al NodeMCU
function sendMessage(mensaje) {
  const nodeMCUHost = '192.168.0.160'; // Reemplaza con la dirección IP real del NodeMCU
  const nodeMCUPuerto = 5683;

  return new Promise((resolve, reject) => {
    const request = coap.request({
      host: nodeMCUHost,
      port: nodeMCUPuerto,
      method: 'POST',
      pathname: '/message', // Reemplaza con la ruta real en el NodeMCU
    });

    // Maneja la respuesta del NodeMCU
    request.on('response', (respuesta) => {
      let data = '';
      respuesta.on('data', (chunk) => {
        data += chunk;
      });

      respuesta.on('end', () => {
        if (respuesta.code === '2.05') {
          // Si la respuesta del NodeMCU es exitosa (código 2.05), resuelve la promesa con la respuesta
          resolve(data);
        } else {
          // Si la respuesta del NodeMCU no es exitosa, rechaza la promesa con un mensaje de error
          reject(`Respuesta inesperada del NodeMCU: ${respuesta.code}`);
        }
      });
    });

    // Maneja errores en la solicitud CoAP
    request.on('error', (error) => {
      reject(`Error al enviar solicitud CoAP al NodeMCU: ${error.message}`);
    });

    // Envía el mensaje al NodeMCU
    request.write(mensaje);
    request.end();
  });
}