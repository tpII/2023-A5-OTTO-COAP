import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-windows2',
  templateUrl: './windows2.component.html',
  styleUrls: ['./windows2.component.css'],
})
export class Windows2Component {
  private Veccoap: number[] = [];
  private Vecmqtt: number[] = [];

  constructor(private router: Router) {}

  //se encarga de actualizar los valores de los arreglos para luego visualizarlos en la pantalla

  goToWindows1() {
    this.router.navigate(['/windows1']);
  }
}
