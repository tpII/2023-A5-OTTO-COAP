# Usa una imagen base de Node.js
FROM node:latest

WORKDIR /app
# Copia los archivos necesarios al contenedor
COPY package.json package.json ./
COPY package.json package-lock.json ./
COPY docker-compose.yml docker-compose.yml ./

# Instala las dependencias
RUN npm install 

COPY ws-proxy.js ws-proxy.js ./
# Expone el puerto utilizado por tu servidor Node.js
EXPOSE 3000

# Comando para ejecutar tu aplicación al iniciar el contenedor
CMD ["npm", "start"]
