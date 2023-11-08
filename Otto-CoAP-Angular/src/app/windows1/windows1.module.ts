import { CUSTOM_ELEMENTS_SCHEMA, NgModule } from '@angular/core';
import { MatMenuModule } from '@angular/material/menu';
import { MqttModule } from '../helper/mqtt/mqtt.module';

@NgModule({
  declarations: [],
  imports: [MatMenuModule, MqttModule],
  schemas: [CUSTOM_ELEMENTS_SCHEMA],
})
export class Windows1Module {}
