#include "lcd.hpp"

void main_screen(uint16_t glycemie, uint16_t* plot, uint8_t size_of_plot, uint8_t batery){
    //main screen
    // @param {string} plot vector avec 24 valeur pour le plot
    // @todo conversao da glicemia para os pixels
  Tft.drawRectangle(0, 280, SCREENY_MAX, 40, GRAY1);

  draw_glycemie(glycemie);
  draw_graphic(plot, size_of_plot);
  draw_batery(batery);

}

void draw_glycemie(uint16_t glycemie)
{
  Tft.drawNumber(glycemie, 50, 20, 8, WHITE );
  Tft.drawNumber(50 , 0, 260, 1, WHITE);
  Tft.drawNumber(80 , 0, 230, 1, WHITE);
  Tft.drawNumber(110, 0, 200, 1, WHITE);
  Tft.drawNumber(140, 0, 170, 1, WHITE);
  Tft.drawNumber(170, 0, 140, 1, WHITE);
  Tft.drawNumber(200, 0, 110, 1, WHITE);
}

void draw_graphic(uint16_t* plot, uint8_t size_of_plot)
{
    if( *(plot) = NULL ) return;
    Tft.drawCircle(5, 310 - *(plot), 1, RED);
    for(int i = 0; i < size_of_plot; i++)
    {
      
    }

}

void draw_alert(void)
{
    Tft.fillCircle(120, 300, 15, RED);
    Tft.drawString("!", 113, 290, 3, WHITE);
}

void draw_batery(uint8_t batery)
{
  Tft.drawNumber(batery, 195, 290, 2, WHITE );
  Tft.drawString("%", 220, 290, 2, WHITE);
}

void erase_screen(void)
{
    Tft.fillRectangle(0,0,SCREENX_MAX,SCREENY_MAX, BLACK);
}

void erase_alert(void)
{
    Tft.fillRectangle(115,285, 30, 30,  BLACK);
}

void erase_glycemie(void)
{
    Tft.fillRectangle(50,20,240,80, BLACK);
}

void erase_batery(void)
{
    Tft.fillRectangle(195,290,24,14, BLACK);
}