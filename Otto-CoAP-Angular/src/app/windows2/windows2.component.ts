import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-windows2',
  templateUrl: './windows2.component.html',
  styleUrls: ['./windows2.component.css'],
})
export class Windows2Component implements OnInit {
  datosAlmacenados = '';

  constructor(private router: Router, private http: HttpClient) {}

  ngOnInit() {
    this.obtenerContenidoArchivo();
  }

  obtenerContenidoArchivo() {
    this.http
      .get('assets/informes/tiempos_de_comunicacion.txt', {
        responseType: 'text',
      })
      .subscribe((data) => {
        //this.archivoContenido = data;
        this.datosAlmacenados = data;
      });
  }

  goToWindows1() {
    this.router.navigate(['/windows1']);
  }
}
