import { NgModule } from '@angular/core';
import { MqttService } from './mqtt.service';

@NgModule({
  providers: [MqttService],
})
export class MqttModule {}
