import { Injectable } from '@angular/core';
import { MqttService, IMqttMessage } from 'ngx-mqtt';
import { mqttConfig } from './mqtt.config';

@Injectable({
  providedIn: 'root',
})
export class MqttServiceClient {
  private confirmationMessage: string = 'ok';
  private metricStartTime: number = 0;
  private metricTimeElapsed: number = 0;

  constructor(private mqttService: MqttService) {
    this.mqttService.connect(mqttConfig);

    // Suscripción al tópico "mensaje" para recibir confirmaciones
    this.mqttService.observe('mensaje').subscribe((message: IMqttMessage) => {
      const receivedMessage = message.payload.toString();

      if (receivedMessage === this.confirmationMessage) {
        // Se recibió la confirmación "ok"
        const metricEndTime = Date.now();
        this.metricTimeElapsed = metricEndTime - this.metricStartTime;
      }
    });
  }
  //devuelve la metrica obtenida
  getMetricMqtt() {
    return this.metricTimeElapsed;
  }

  // publica en el topico movimiento el valor del indice del movimiento
  public publishToMovimientos(value: number): void {
    this.metricStartTime = Date.now();
    const message = value.toString();
    this.mqttService.unsafePublish('movimientos', message, {
      qos: 1,
      retain: true,
    });
  }
}
