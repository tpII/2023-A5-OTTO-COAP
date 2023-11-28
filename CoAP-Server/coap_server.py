import asyncio
import socket
import aiocoap
from aiocoap import *
from aiocoap import resource
import logging

# Configura el nivel de logs
logging.basicConfig(level=logging.DEBUG)

class OttoRobotResource(resource.ObservableResource):
    def __init__(self):
        super().__init__()
        self.mymovimiento = f""
        self.observe_count = 0
    async def render_post(self, request):
        try:
            payload = request.payload.decode('utf-8')
            logging.info("Solicitud POST recibida")
            print(f"Solicitud POST recibida con payload: {payload}")

            # Lógica para interpretar comandos y controlar el NodeMCU
            self.mymovimiento = payload
            self.updated_state()
            return aiocoap.Message(code=aiocoap.CHANGED, payload=str(self.mymovimiento).encode('utf-8'))

        except Exception as e:
            print(f"Error en render_post: {e}")
            return aiocoap.Message(code=aiocoap.CHANGED, payload=b"Error interno")

    async def render_get(self, request):
        try:
            payload = bytes(self.mymovimiento, 'utf-8')
            print(f"Payload:{payload}")
            logging.info("Solicitud GET recibida")
            return aiocoap.Message(payload=payload)
        except Exception as e:
            print(f"Error en render_get():{e}")
            return aiocoap.Message(code=aiocoap.CHANGED, payload=b"Error interno")
    def observer(self, response):
        # ... Otras declaraciones ...
        logging.info("Observación recibida en el recurso 'movimiento'")
        # ... Otras declaraciones ...
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
            
            
async def main():

    # Configurar el recurso CoAP y pasar la instancia de NodeMCU
    root = resource.Site()
    root.add_resource(('.well-known', 'core'), resource.WKCResource(root.get_resources_as_linkheader))
    root.add_resource(('movimientos',), OttoRobotResource())
    # Obtener la dirección IP local
    local_ip = get_local_ip()
    print(f"{local_ip}")
    # Configurar y levantar el servidor
    loop= asyncio.get_event_loop()
    asyncio.ensure_future (Context.create_server_context(root, bind=(local_ip, 5683)))
# Local ip debe replazarse con la ip en la que quieras levantar el server
    print(f"Servidor CoAP iniciado en: http://{local_ip}:5683")
    await asyncio.sleep(3600)

if __name__ == "__main__":
      asyncio.run(main())