# Imagen base que necesites para ejecutar tu servidor CoAP
FROM python:3.9

# Copiar los archivos necesarios
COPY . /app
WORKDIR /app

# Instalar dependencias
RUN pip install -r requirements.txt

# Comando para iniciar el servidor CoAP
CMD [ "python", "coap_server.py" ]
