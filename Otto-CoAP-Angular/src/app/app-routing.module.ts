import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { Windows1Component } from './windows1/windows1.component';
import { Windows2Component } from './windows2/windows2.component';

const routes: Routes = [
  { path: 'windows1', component: Windows1Component },
  { path: 'windows2', component: Windows2Component },
  { path: '', redirectTo: '/windows1', pathMatch: 'full' },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule],
})
export class AppRoutingModule {}
