import asyncio
import socket
import aiocoap
from aiocoap import *
from aiocoap import resource
import logging

# Configura el nivel de logs
logging.basicConfig(level=logging.DEBUG)

class OttoRobotResource(resource.Resource):
    def __init__(self):
        super().__init__()
        self.mymovimiento = f""

    async def render_post(self, request):
        try:
            payload = request.payload.decode('utf-8')
            logging.info("Solicitud POST recibida")
            print(f"Solicitud POST recibida con payload: {payload}")

            # Lógica para interpretar comandos y controlar el NodeMCU
            self.mymovimiento = payload

            return aiocoap.Message(payload=b"Recibido")
        except Exception as e:
            print(f"Error en render_post: {e}")
            return aiocoap.Message(code=aiocoap.CHANGED, payload=b"Error interno")

    async def render_get(self, request):
        try:
            payload = bytes(self.mymovimiento, 'utf-8')
            self.mymovimiento=f""
            print(f"Payload:{payload}")
            logging.info("Solicitud GET recibida")
            return aiocoap.Message(payload=payload)
        except Exception as e:
            print(f"Error en render_get():{e}")
            return aiocoap.Message(code=aiocoap.CHANGED, payload=b"Error interno")
   
    
def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # Esto intentará conectar a una dirección de Internet, pero no enviará realmente datos
        s.connect(('10.255.255.255', 1))
        return s.getsockname()[0]
    except Exception:
         return '127.0.0.1'
    finally:
        s.close()
            
            
def main():

    # Configurar el recurso CoAP y pasar la instancia de NodeMCU
    root = resource.Site()
    root.add_resource(('.well-known', 'core'), resource.WKCResource(root.get_resources_as_linkheader))
    root.add_resource(('movimiento',), OttoRobotResource())
    # Obtener la dirección IP local
    local_ip = get_local_ip()
    print(f"{local_ip}")
    # Configurar y levantar el servidor
    asyncio.Task(Context.create_server_context(root, bind=(local_ip, 5683)))
    print(f"Servidor CoAP iniciado en: http://{local_ip}:5683")
    asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
    main()