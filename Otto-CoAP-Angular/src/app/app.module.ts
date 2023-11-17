import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { MatMenuModule } from '@angular/material/menu';
import { MqttModule } from 'ngx-mqtt';
import { HttpClientModule } from '@angular/common/http';
import { ipname } from 'src/ENV';
@NgModule({
  declarations: [AppComponent],
  imports: [
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule,
    MatMenuModule,
    MqttModule.forRoot({
      hostname: ipname, //aqui se debe colocar la direccion ip en la cual nos encontramos conectado
      port: 1883, // Puerto MQTT predeterminado
      path: '/mqtt',
    }),
    HttpClientModule,
  ],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
