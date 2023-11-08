const coap = require('coap');

const server = coap.createServer();

server.on('request', (req, res) => {
  // Procesa la solicitud CoAP y envía una respuesta
  res.end('Respuesta CoAP');
});

server.listen(() => {
  console.log('Servidor CoAP escuchando en puerto 5683');
});
