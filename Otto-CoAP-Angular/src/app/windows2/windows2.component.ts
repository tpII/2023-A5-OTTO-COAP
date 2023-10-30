import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-windows2',
  templateUrl: './windows2.component.html',
  styleUrls: ['./windows2.component.css'],
})
export class Windows2Component implements OnInit {
  constructor(private router: Router) {}

  ngOnInit(): void {}

  goToWindows1() {
    this.router.navigate(['/windows1']);
  }
}
