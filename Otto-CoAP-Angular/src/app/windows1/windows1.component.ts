import { Component } from '@angular/core';
import { MatMenuModule } from '@angular/material';
import { Router } from '@angular/router';

@Component({
  selector: 'app-windows1',
  templateUrl: './windows1.component.html',
  styleUrls: ['./windows1.component.css'],
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
  async saveValue(valor: number) {
    try {
      this.indice = await valor;
    } catch (error) {
      console.error('Error en la operacion asincronica: ', error);
    }
  }

  constructor(private router: Router) {}

  goToWindows2() {
    this.router.navigate(['/windows2']);
  }
}
