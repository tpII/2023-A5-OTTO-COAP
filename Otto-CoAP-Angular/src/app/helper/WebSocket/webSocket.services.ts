import { Injectable } from '@angular/core';
import { ipname } from 'src/ENV';

@Injectable({
  providedIn: 'root',
})
export class WebSocketService {
  private ws!: WebSocket; // Inicializador de "ws" como tipo WebSocket

  constructor() {
    this.connectWebSocket();
  }

  private connectWebSocket() {
    this.ws = new WebSocket(`ws://${ipname}:3000/`);

    this.ws.onopen = () => {
      console.log('Conexión WebSocket establecida');
      console.log('Dirección del WebSocket:', this.ws.url);
    };
  }

  enviarMensaje(topic: string, message: string) {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      const data = JSON.stringify({ topic, message });
      console.log('Se enviará la siguiente información:', data);
      this.ws.send(data);
    } else {
      console.error(
        'La conexión WebSocket no está disponible o no está abierta.'
      );
    }
  }
}
