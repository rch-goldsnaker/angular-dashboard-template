import { Component, ElementRef, inject, QueryList, ViewChildren } from '@angular/core';
import { Breakpoints, BreakpointObserver } from '@angular/cdk/layout';
import { map } from 'rxjs/operators';
import { AsyncPipe } from '@angular/common';
import { MatGridListModule } from '@angular/material/grid-list';
import { MatMenuModule } from '@angular/material/menu';
import { MatIconModule } from '@angular/material/icon';
import { MatButtonModule } from '@angular/material/button';
import { MatCardModule } from '@angular/material/card';

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrl: './dashboard.component.scss',
  standalone: true,
  imports: [
    AsyncPipe,
    MatGridListModule,
    MatMenuModule,
    MatIconModule,
    MatButtonModule,
    MatCardModule
  ]
})
export class DashboardComponent {
  private breakpointObserver = inject(BreakpointObserver);

  @ViewChildren('dashboardIframe') iframes!: QueryList<ElementRef<HTMLIFrameElement>>;

  ngAfterViewInit() {
    setInterval(() => {
      this.iframes.forEach((iframeRef) => {
        const iframe = iframeRef.nativeElement;
        iframe.src += '';
      });
    }, 10000);
  }

  /** Based on the screen size, switch from standard to one column per row */
  cards = this.breakpointObserver.observe(Breakpoints.Handset).pipe(
    map(({ matches }) => {
      if (matches) {
        return [
          { title: 'Temperature', cols: 4, rows: 1 },
          { title: 'Humidity', cols: 4, rows: 1 },
          { title: 'Temperature-Area', cols: 4, rows: 1 },
          { title: 'Humidity-Area', cols: 4, rows: 1 },
          { title: 'Rssi', cols: 4, rows: 1 },
          { title: 'Snr', cols: 4, rows: 1 },
          { title: 'Batery', cols: 4, rows: 1 },
          { title: 'Ip', cols: 4, rows: 1 },
          { title: 'Location', cols: 4, rows: 1 },
        ];
      }

      return [
        { title: 'Temperature', cols: 2, rows: 1 },
        { title: 'Humidity', cols: 2, rows: 1 },
        { title: 'Temperature-Area', cols: 2, rows: 1 },
        { title: 'Humidity-Area', cols: 2, rows: 1 },
        { title: 'Rssi', cols: 1, rows: 1 },
        { title: 'Snr', cols: 1, rows: 1 },
        { title: 'Batery', cols: 1, rows: 1 },
        { title: 'Ip', cols: 1, rows: 1 },
        { title: 'Location', cols: 4, rows: 1 },
      ];
    })
  );
}
