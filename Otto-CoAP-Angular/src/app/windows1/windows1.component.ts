import { Component } from '@angular/core';
import { MatMenuModule } from '@angular/material/menu';
import { Router } from '@angular/router';

@Component({
  selector: 'app-windows1',
  templateUrl: './windows1.component.html',
  styleUrls: ['./windows1.component.css'],
  standalone: true,
  imports: [MatMenuModule],
})
export class Windows1Component {
  options: string[] = [
    'turnRight',
    'turnLeft',
    'bend',
    'shakeLeg',
    'updown',
    'swing',
    'swingFix',
    'tiptoeSwing',
    'moonwalker',
    'crusaito',
    'flapping',
    'happy',
    'sad',
    'victory',
    'angry',
    'sleeping',
    'confused',
    'fart',
    'fail',
    'avoidObstacles',
    'followMode',
    'retrieve',
  ];
  selectedOption: string = '';
  indice: number = 0;

  //Permite saber la instruccion elegida por el usuario, para luego enviarla al Otto
  saveValue(valor: number) {
    this.indice = valor;
  }

  constructor(private router: Router) {}
  protocoloSeleccionado: string = 'CoAP';

  toggleProtocol() {
    if (this.protocoloSeleccionado === 'CoAP')
      this.protocoloSeleccionado = 'MQTT';
    else {
      this.protocoloSeleccionado = 'CoAP';
    }
  }

  goToWindows2() {
    this.router.navigate(['/windows2']);
  }
}
