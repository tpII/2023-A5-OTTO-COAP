import { Component, OnInit } from '@angular/core';
import { MatMenuModule } from '@angular/material/menu';
import { Router } from '@angular/router';
import { CoapService } from '../helper/coap.service';
import { MqttServiceClient } from '../helper/mqtt/mqtt.service';

@Component({
  selector: 'app-windows1',
  templateUrl: './windows1.component.html',
  styleUrls: ['./windows1.component.css'],
  standalone: true,
  imports: [MatMenuModule],
  providers: [MqttServiceClient],
})
export class Windows1Component {
  constructor(
    private router: Router,
    private mqttServices: MqttServiceClient,
    private coapService: CoapService
  ) {}
  options: string[] = [
    'home',
    'walkForward',
    'walkBackward',
    'turnRight',
    'turnLeft',
    'bend',
    'shakeLeg',
    'moonwalker',
    'crusaito',
    'flapping',
    'swingFix',
    'tiptoeSwing',
    'updown',
    'happy',
    'sad',
    'victory',
    'sleeping',
    'confused',
    'fart',
    'fail',
    'followMode',
    'retrieve',
    'avoidObstacles',
  ];
  selectedOption: string = '';
  indice: number = -1;
  protocoloSeleccionado: string = 'CoAP';

  OnInit() {
    if (this.protocoloSeleccionado === 'CoAP') {
      this.coapService.sendCoapRequestAndCalculateTime(this.indice.toString());
    } else {
      this.mqttServices.publishToMovimientos(this.indice);
    }
  }
  //Permite saber en todo momento cual es el protocolo de mensajeria que se desea utilizar, permitiendo el cambio

  toggleProtocol() {
    if (this.protocoloSeleccionado === 'CoAP') {
      this.protocoloSeleccionado = 'MQTT';
      this.coapService.sendCoapRequestAndCalculateTime(this.indice.toString());
    } else {
      this.protocoloSeleccionado = 'CoAP';
      this.mqttServices.publishToMovimientos(this.indice);
    }
  }

  //Redireccion hacia la segunda ventana
  goToWindows2() {
    this.router.navigate(['/windows2']);
  }
  //Permite saber la instruccion elegida por el usuario, para luego enviarla al NodeMCU
  saveValue(valor: number) {
    this.indice = valor;
  }
}
