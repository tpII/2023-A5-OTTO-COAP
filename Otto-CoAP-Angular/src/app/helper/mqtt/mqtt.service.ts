import { Injectable, OnInit } from '@angular/core';
import { MqttService, IMqttMessage } from 'ngx-mqtt';

@Injectable({
  providedIn: 'root',
})
export class MqttServiceClient {
  private confirmationMessage: string = 'ok';
  private metricStartTime: number = 0;
  private metricTimeElapsed: number = 0;

  constructor(private mqttService: MqttService) {
    this.mqttService.connect();
  }
  OnInit() {
    // Suscripción al tópico "mensaje" para recibir confirmaciones

    this.mqttService
      .observe('movimientos')
      .subscribe((message: IMqttMessage) => {
        const receivedMessage = message.payload.toString();

        if (receivedMessage === this.confirmationMessage) {
          // Se recibió la confirmación "ok"
          const metricEndTime = Date.now();
          this.metricTimeElapsed = metricEndTime - this.metricStartTime;
        }
      });
  }
  // publica en el topico movimiento el valor del indice del movimiento
  public publishToMovimientos(value: number): void {
    this.metricStartTime = Date.now();
    const message = value.toString();
    this.mqttService.publish('movimientos', message, {
      qos: 1,
      retain: true,
    });
    console.log('publica en el topico movimientos de forma correcta');
  }
  //publica en el topico protocolo que se modifico de protocolo
  public publishToProtocolo(): void {
    this.metricStartTime = Date.now();
    const message = 'true';
    this.mqttService.publish('protocolo', message, {
      qos: 1,
      retain: true,
    });
    console.log('publica en el topico PROTOCOLO de forma correcta');
  }
  //se obtiene el valor de la metrica
  getMetricamqtt() {
    return this.metricTimeElapsed;
  }
}

export { MqttService };
