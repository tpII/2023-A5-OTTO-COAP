import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { MqttServiceClient } from '../helper/mqtt/mqtt.service';
import { CoapServiceClient } from '../helper/coap.service';

@Component({
  selector: 'app-windows2',
  templateUrl: './windows2.component.html',
  styleUrls: ['./windows2.component.css'],
  providers: [MqttServiceClient],
})
export class Windows2Component implements OnInit {
  Veccoap: number[] = [];
  Vecmqtt: number[] = [];

  constructor(
    private router: Router,
    private mqttService: MqttServiceClient,
    private coapService: CoapServiceClient
  ) {}

  ngOnInit(): void {}

  cargaVec() {
    this.Veccoap.push(this.coapService.getMetricacoap());
    this.Vecmqtt.push(this.mqttService.getMetricamqtt());
  }

  goToWindows1() {
    this.router.navigate(['/windows1']);
  }
}
