import { IMqttServiceOptions } from 'ngx-mqtt';

export const ipname: string = '192.168.0.16';

export const MQTT_SERVICE_OPTIONS: IMqttServiceOptions = {
  hostname: ipname, // Coloca la dirección IP del servidor MQTT
  port: 1883, // El puerto por defecto de MQTT es 1883
};
