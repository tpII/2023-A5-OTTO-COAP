import { CUSTOM_ELEMENTS_SCHEMA, NgModule } from '@angular/core';
import { MqttModule } from '../helper/mqtt/mqtt.module';

@NgModule({
  declarations: [],
  imports: [MqttModule],
  schemas: [CUSTOM_ELEMENTS_SCHEMA],
})
export class Windows2Module {}
