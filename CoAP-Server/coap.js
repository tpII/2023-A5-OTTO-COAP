var coap = require('coap');
var servidor = coap.createServer();
const dataStore = {};
var portNumber=5683;
var ip = '192.168.0.245'

function GuardadoEnMemoria (data){
    dataStore['movimiento'] = data;
    console.log('Datos almacenados en memoria:', data);
}



//Manejador para peticion de GET de los clientes del servidor CoAP
servidor.on('request',(req,res)=> {
    if(req.method == 'GET' && req.url=='/movimiento'){
        console.log('Se recibio una peticion de GET');
        //Envia el movimiento solicitado
        res.end(dataStore['movimiento']);
    } else {
        // Manejar otros tipos de solicitudes o recursos
        res.code = '4.04'; // Código de respuesta para "Not Found"
        res.end();
    }
});

servidor.on('request', (req,res)=>{
    if(req.method == 'POST' && req.url == '/movimiento'){
        consoler.log('Se recibio una peticion de POST');
        // Obtener y procesar los datos del cuerpo de la solicitud
        let requestData = '';
        req.on('data', (chunk) => {
        requestData += chunk;
    });
    req.on('end', ()=>{
        console.log('Datos recibidos:', requestData);
        //Alamcenamos el movimiento recibido desde el interface web
        GuardadoEnMemoria(requestData);
        // Responder con un mensaje de éxito
        res.code = '2.01'; // Código de respuesta para "Created"
        res.end('Datos recibidos con éxito');
    });
    } else {
        // Manejar otros tipos de solicitudes o recursos
        res.code = '4.04'; // Código de respuesta para "Not Found"
        res.end();
    }
});

servidor.listen(portNumber,ip, () =>{
    const address = servidor._sock.address();
    console.log(`Servidor CoAP iniciado en ${address.address}:${address.port}`);
});
