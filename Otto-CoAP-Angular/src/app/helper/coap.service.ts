import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root',
})
export class CoapServiceClient {
  constructor(private http: HttpClient) {}
  private tiempoTranscurrido: number = 0;
  public sendCoapRequest(
    host: string,
    port: number,
    path: string,
    payload: string
  ): Promise<number> {
    return new Promise<number>((resolve, reject) => {
      const startTime = performance.now(); // Registra el tiempo de inicio

      // Realiza la solicitud HTTP POST con el payload
      this.http.post(`coap://${host}:${port}${path}`, payload).subscribe(
        () => {
          // Analiza la respuesta (en este ejemplo, no se requiere analizar una respuesta específica)
          const endTime = performance.now(); // Registra el tiempo de finalización
          this.tiempoTranscurrido = endTime - startTime; // Calcula la diferencia de tiempo
          resolve(this.tiempoTranscurrido);
        },
        (error) => {
          reject(`Error en la solicitud CoAP: ${error}`);
        }
      );
    });
  }
  getMetricaCoap() {
    return this.tiempoTranscurrido;
  }
}
