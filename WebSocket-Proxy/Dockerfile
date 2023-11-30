# Imagen base que necesites para ejecutar tu servidor WebSocket
FROM node:latest

# Copiar los archivos necesarios
COPY . /app
WORKDIR /app

# Instalar dependencias
RUN npm install

# Comando para iniciar el servidor WebSocket
CMD [ "node", "ws-proxy.js" ]
