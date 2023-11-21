import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { ipname } from 'src/ENV';

@Injectable({
  providedIn: 'root',
})
export class CoapService {
  private coapServerUrl = `http://${ipname}:5683/message`; //Colocar la dirección ip de la máquina
  //dependiendo de la red a la que se conecte

  constructor(private http: HttpClient) {}
  private elapsedTime: number = -1;
  //informa al node sobre el cambio de protocolo
  sendCoapProtocol() {
    this.http.post(`${this.coapServerUrl}`, true);
    console.log(
      'Realiza el aviso de cambio de PROTOCOLO de forma correcta en COAP'
    );
  }
  //metodo que calcula el tiempo de respuesta con el node
  sendCoapRequestAndCalculateTime(message: string) {
    const startTime = Date.now(); // Registra el tiempo de inicio
    this.http.post(`${this.coapServerUrl}`, message).subscribe((response) => {
      // Procesa la respuesta del servidor CoAP
      if (response && response === 'OK') {
        const endTime = Date.now(); // Registra el tiempo de finalización
        this.elapsedTime = endTime - startTime; // Calcula el tiempo transcurrido
      }
    });
    console.log('Envia el movimiento en COAP de forma correcta');
  }
  //devolucion metrica obtenida
  getMetricacoap() {
    return this.elapsedTime;
  }
}
