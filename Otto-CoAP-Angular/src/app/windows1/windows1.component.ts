import { Component } from '@angular/core';
import { MatMenuModule } from '@angular/material/menu';
import { Router } from '@angular/router';
import { MqttServiceClient } from '../helper/mqtt.service';
import { CoapServiceClient } from '../helper/coap.service';

@Component({
  selector: 'app-windows1',
  templateUrl: './windows1.component.html',
  styleUrls: ['./windows1.component.css'],
  standalone: true,
  imports: [MatMenuModule],
})
export class Windows1Component {
  constructor(
    private router: Router,
    private mqttService: MqttServiceClient,
    private coapService: CoapServiceClient
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
  private isCoap = true;

  //Permite saber la instruccion elegida por el usuario, para luego enviarla al NodeMCU
  saveValue(valor: number) {
    this.indice = valor;
  }

  //Permite saber en todo momento cual es el protocolo de mensajeria que se desea utilizar, permitiendo el cambio
  protocoloSeleccionado: string = 'CoAP';
  toggleProtocol() {
    if (this.protocoloSeleccionado === 'CoAP') {
      this.protocoloSeleccionado = 'MQTT';
      this.isCoap = false;
    } else {
      this.protocoloSeleccionado = 'CoAP';
      this.isCoap = true;
    }
  }

  comunicacion() {
    if (this.isCoap)
      this.coapService.sendCoapRequest(
        'host',
        5683,
        'ruta',
        this.indice.toString()
      );
    else this.mqttService.publishToMovimientos(this.indice);
  }

  goToWindows2() {
    this.router.navigate(['/windows2']);
  }
}
