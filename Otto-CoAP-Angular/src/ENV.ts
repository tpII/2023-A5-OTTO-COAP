import { IMqttServiceOptions } from 'ngx-mqtt';

export const ipname: string = '192.168.0.160';

export const MQTT_SERVICE_OPTIONS: IMqttServiceOptions = {
  hostname: ipname, // Coloca la dirección IP de tu servidor MQTT aquí
  port: 9001, // El puerto por defecto de MQTT es 1883
};
