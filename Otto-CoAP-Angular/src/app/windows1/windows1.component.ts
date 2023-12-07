import { Component, OnInit } from '@angular/core';
import { MatMenuModule } from '@angular/material/menu';
import { Router } from '@angular/router';
import { WebSocketService } from '../helper/webSocket.services';

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
    private WebSocketService: WebSocketService
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
  indice: number = 1;
  protocoloSeleccionado: string = 'CoAP';

  //Permite saber en todo momento cual es el protocolo de mensajeria que se desea utilizar, permitiendo el cambio

  toggleProtocol() {
    if (this.protocoloSeleccionado === 'CoAP') {
      this.protocoloSeleccionado = 'MQTT';
      this.WebSocketService.enviarMensaje('protocolo', 'MQTT');
    } else {
      this.protocoloSeleccionado = 'CoAP';
      this.WebSocketService.enviarMensaje('protocolo', 'CoAP');
    }
  }

  //Redireccion hacia la segunda ventana
  goToWindows2() {
    this.router.navigate(['/windows2'], { state: { contenidoArchivo: '...' } });
  }
  //Permite saber la instruccion elegida por el usuario, para luego enviarla al NodeMCU
  saveValue(valor: number) {
    this.indice = valor;
    this.WebSocketService.enviarMensaje('movimientos', this.indice.toString());
  }
}
