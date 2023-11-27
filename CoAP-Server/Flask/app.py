import asyncio
from flask import Flask, jsonify, request, render_template
from aiocoap import *
from flask_cors import CORS
import aiocoap.defaults
import subprocess

# Configurar la política de retransmisión y el tiempo de espera
aiocoap.defaults.transmit_retries = 3
aiocoap.defaults.transmit_wait = 1.0  # Tiempo de espera en segundos

app = Flask(__name__)
CORS(app)

app.config['JSON_AS_ASCII']=False

async def enviar_datos_a_coap(servidor_coap_ip, servidor_coap_port, datos):
    try:
        # Construir la solicitud CoAP
        request = Message(code= POST, uri=f'coap://{servidor_coap_ip}:{servidor_coap_port}/movimiento')
        request.payload = datos.encode('utf-8')

        # Enviar la solicitud y esperar la respuesta
        loop = asyncio.get_event_loop()
        context = await Context.create_client_context(loop=loop)
        response = await context.request(request).response

        # Procesar la respuesta del servidor CoAP si es necesario
        print(f"Respuesta del servidor CoAP: {response.payload}")
        return response.payload.decode('utf-8')
    except Exception as e:
        # Manejar posibles errores
        print(f"Error al enviar datos al servidor CoAP: {str(e)}")
        return None


@app.route('/')
def index():
    """# Dirección IP o nombre de host de tu servidor CoAP
    server_coap_address = '192.168.0.254'  # Reemplaza con la dirección IP de tu servidor CoAP

    # Ejecutar el comando ping para probar conectividad con servidor CoAP
    try:
        app.logger.info('Iniciando ejecucion de ping ...')
        result = result = subprocess.run(['C:\\Windows\\System32\\ping.exe', '-n', '4', server_coap_address], capture_output=True, text=True, check=True)
        app.logger.info('Ping ejecutado con exito.')
        output = result.stdout
        print(output)
    except subprocess.CalledProcessError as e:
        output = f"Error al ejecutar ping: {e}"
        print(output)

    return f'Resultado del ping a {server_coap_address}:\n{output}'"""
    return render_template('index.html')  # Puedes utilizar una plantilla HTML para tu interfaz web

@app.route('/movimiento', methods=['POST'])
def movimiento(): 
    try:
        datos = request.json['datos']
        # Envía los datos al servidor CoAP
        servidor_coap_ip = "163.10.142.82"  # Cambia esto con la IP de tu servidor CoAP
        servidor_coap_port = 5683  # Cambia esto con el puerto de tu servidor CoAP

        respuesta_coap = asyncio.run(enviar_datos_a_coap(servidor_coap_ip, servidor_coap_port, datos))
        # Devuelve una respuesta a la interfaz web
        return jsonify({'mensaje': 'Datos procesados y enviados al servidor CoAP con éxito', 'respuesta_coap': respuesta_coap})
    except Exception as e:
        # Manejar posibles errores
        print(f"Error al procesar los datos en Flask: {str(e)}")
        return jsonify({'mensaje': 'Error al procesar los datos en Flask'}), 500
   
if __name__ == '__main__':
    app.run(debug=True)
    
"""@app.route('/otto', methods=['POST'])
def otto():
    coap_host = '192.168.0.245'  # Cambia a la IP de tu servidor CoAP
    coap_port = 5683

    # Obtiene el comando del cuerpo de la solicitud HTTP
    command = request.get_data().decode('utf-8')

    # Crea y envía la solicitud CoAP
    async def send_coap_request():
        context = await aiocoap.Context.create_client_context()
        peticion = Message(code= POST, payload=command.encode('utf-8'))
        peticion.opt.uri_host = coap_host
        peticion.opt.uri_port = coap_port
        peticion.opt.uri_path = ("otto")

        try:
            response = await context.request(peticion).response
            return response.payload.decode('utf-8')
        except Exception as e:
            return f"Error en la solicitud CoAP: {str(e)}"

    # Espera la respuesta de la solicitud CoAP y la devuelve como JSON
    coap_response = asyncio.run(send_coap_request())
    return jsonify({'coap_response': coap_response})

if __name__ == '__main__':
    app.run(port=5000)"""