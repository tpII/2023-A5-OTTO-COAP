import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { MqttServiceClient } from '../helper/mqtt/mqtt.service';
import { CoapServiceClient } from '../helper/coap.service';

@Component({
  selector: 'app-windows2',
  templateUrl: './windows2.component.html',
  styleUrls: ['./windows2.component.css'],
})
export class Windows2Component {
  private Veccoap: number[] = [];
  private Vecmqtt: number[] = [];

  constructor(
    private router: Router,
    private mqttService: MqttServiceClient,
    private coapService: CoapServiceClient
  ) {}

  //se encarga de actualizar los valores de los arreglos para luego visualizarlos en la pantalla
  cargaVec() {
    this.Veccoap.push(this.coapService.getMetricacoap());
    this.Vecmqtt.push(this.mqttService.getMetricamqtt());
  }

  goToWindows1() {
    this.router.navigate(['/windows1']);
  }
}
