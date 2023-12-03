import { TestBed, ComponentFixture, waitForAsync } from '@angular/core/testing';
import { Windows1Component } from './windows1.component';
import { MatMenuModule } from '@angular/material/menu';
import { By } from '@angular/platform-browser';

describe('Windows1Component', () => {
  let fixture: ComponentFixture<Windows1Component>;
  let windows1Component: Windows1Component;

  beforeEach(() => {
    TestBed.configureTestingModule({
      imports: [MatMenuModule],
    });
    fixture = TestBed.createComponent(Windows1Component);
    windows1Component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should perform action when action button in Windows1Component is clicked', () => {
    spyOn(windows1Component, 'saveValue');

    const actionButton = fixture.nativeElement.querySelector('.btn_up');
    actionButton.click();

    expect(windows1Component.saveValue).toHaveBeenCalledWith(1);
  });

  it('Verifies that the loading of the elements is correct based on the order', async () => {
    const matMenuTrigger = fixture.debugElement.query(
      By.css('.column-button1')
    );
    // Simula hacer clic en el botón del menú
    matMenuTrigger.nativeElement.click();
    fixture.detectChanges();
    await fixture.whenStable(); // Espera a que se resuelvan las tareas asíncronas

    // Carga de movimientos
    // Simula hacer clic en la opción deseada
    const menuItem = windows1Component.options;
    const optionToSelect = menuItem[7];

    fixture.detectChanges();

    // Verifica si coincide el elemento con la posicion indicada
    expect(optionToSelect).toBe('moonwalker');
  });

  it('should toggle variable in Windows1Component when toggle button is clicked', () => {
    const toggleButton = fixture.nativeElement.querySelector(
      '.protocolo-selector button'
    );
    toggleButton.click();

    expect(windows1Component.protocoloSeleccionado).toBe('MQTT');
  });
});
