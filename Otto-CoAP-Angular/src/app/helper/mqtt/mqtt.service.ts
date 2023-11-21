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
  OnInit() {}
  // publica en el topico movimiento el valor del indice del movimiento
  public publishToMovimientos(value: number): void {
    this.metricStartTime = Date.now();
    const message =
      'Se publica el movimiento con el indice: ' + value.toString();
    this.mqttService.unsafePublish('movimientos', message, {
      qos: 1,
      retain: true,
    });
    console.log('publica en el topico movimientos de forma correcta');
  }
  //se obtiene el valor de la metrica
  getMetricamqtt() {
    return this.metricTimeElapsed;
  }
}

export { MqttService };
