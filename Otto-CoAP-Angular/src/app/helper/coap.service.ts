import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root',
})
export class CoapServiceClient {
  private coapServerUrl = 'http://localhost:5683';

  constructor(private http: HttpClient) {}
  private elapsedTime: number = -1;
  //informa al node sobre el cambio de protocolo
  sendCoapProtocol() {
    this.http.post(`${this.coapServerUrl}`, true);
  }
  //metodo que calcula el tiempo de respuesta con el node
  sendCoapRequestAndCalculateTime(message: string) {
    const startTime = Date.now(); // Registra el tiempo de inicio
    this.http
      .post(`${this.coapServerUrl}`, message)
      .subscribe((response: any) => {
        // Procesa la respuesta del servidor CoAP
        if (response && response === 'OK') {
          const endTime = Date.now(); // Registra el tiempo de finalización
          this.elapsedTime = endTime - startTime; // Calcula el tiempo transcurrido
        }
      });
  }
  getMetricacoap() {
    return this.elapsedTime;
  }
}
