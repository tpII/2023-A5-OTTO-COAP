# Usa una imagen base de Node.js
FROM node:latest as build-step

RUN mkdir -p /app

WORKDIR /app

COPY package.json /app

RUN npm install

COPY . /app

RUN npm run build

#segunda etapa

FROM nginx:latest

COPY --from=build-step /app/dist/Otto-CoAP-Angular /usr/share/nginx/html
