#include "lcd.hpp"

void main_screen(uint16_t glycemie, uint16_t* plot, uint8_t size_of_plot, uint8_t batery){
    //main screen
    // @param {string} plot vector avec 24 valeur pour le plot
    // @todo conversao da glicemia para os pixels
  //Tft.drawRectangle(0, 280, SCREENY_MAX, 40, GRAY1);

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
    Tft.drawLine(20, 260, SCREENX_MAX, 260, GRAY1);
    Tft.drawLine(20, 230, SCREENX_MAX, 230, GREEN);
    Tft.drawLine(20, 200, SCREENX_MAX, 200, GRAY1);
    Tft.drawLine(20, 170, SCREENX_MAX, 170, GRAY1);
    Tft.drawLine(20, 140, SCREENX_MAX, 140, GRAY1);
    Tft.drawLine(20, 110, SCREENX_MAX, 110, GRAY1);
    Tft.drawLine(20, 180, SCREENX_MAX, 180, GREEN);


    uint16_t points[size_of_plot];

    if( *(plot+size_of_plot-1) == NULL ) return;
    if( *(plot+size_of_plot-1) < 50 )
    {
      points[size_of_plot-1] = 260;
      Tft.fillCircle(PLOT_Px_DATA+PLOT_DIST*(size_of_plot-1), points[size_of_plot-1], PLOT_POINT_T, YELLOW);
    }
    else if ( *(plot+size_of_plot-1) > 200 )
    {
      points[size_of_plot-1] = 110;
      Tft.fillCircle(PLOT_Px_DATA+PLOT_DIST*(size_of_plot-1), points[size_of_plot-1], PLOT_POINT_T, YELLOW);
    }
    else
    {
      points[size_of_plot-1] = 310 - *(plot+size_of_plot-1);
      Tft.fillCircle(PLOT_Px_DATA+PLOT_DIST*(size_of_plot-1), points[size_of_plot-1], PLOT_POINT_T, RED);
    }
    
    for(int i = 0; i < size_of_plot-1; i++)
    {
      if( *(plot+size_of_plot-i-2) == NULL ) return;
      if( *(plot+size_of_plot-i-2) < 50 )
      {
        points[size_of_plot-2-i] = 260;
        Tft.fillCircle(PLOT_Px_DATA+PLOT_DIST*(size_of_plot-i-2), points[size_of_plot-2-i], PLOT_POINT_T, YELLOW);
      }
      else if (*(plot+size_of_plot-i-2) > 200)
      {
        points[size_of_plot-2-i] = 110;
        Tft.fillCircle(PLOT_Px_DATA+PLOT_DIST*(size_of_plot-i-2), points[size_of_plot-2-i], PLOT_POINT_T, YELLOW);
      }
      else
      {
        points[size_of_plot-2-i] = 310 - *(plot+size_of_plot-2-i);
        Tft.fillCircle(PLOT_Px_DATA+PLOT_DIST*(size_of_plot-i-2), points[size_of_plot-2-i], PLOT_POINT_T, RED);
      }
      
      

      Tft.drawLine(PLOT_DIST*(size_of_plot-i-1) +PLOT_Px_DATA, points[size_of_plot-1-i], PLOT_DIST*(size_of_plot-i-2)+PLOT_Px_DATA, points[size_of_plot-2-i], RED);
      //Tft.fillCircle(PLOT_DIST*(size_of_plot-i-2)+PLOT_Px_DATA, 310 - *(plot+size_of_plot-2-i), PLOT_POINT_T, RED);
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

void draw_mode(uint8_t mode)
{
  if(mode == EXEC)
  {
     Tft.fillRectangle(10, 290, 25, 15, GREEN);
  }
  else
  {
     Tft.fillRectangle(10, 290, 25, 15, RED);
  }
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

void erase_graphic(void)
{
    Tft.fillRectangle(20,110,SCREENX_MAX, 150, BLACK);
}



























/*

  if (*(plot+i) > 200)
      {
        Serial.println(points[i-1]);
        Tft.drawLine(PLOT_DIST*(i-1)+PLOT_Px_DATA, points[i-1], PLOT_DIST*(i)+PLOT_Px_DATA, 110 , RED);
        Tft.fillCircle(PLOT_DIST*(i-1)+PLOT_Px_DATA, 110, PLOT_POINT_T, YELLOW);
        points[i] = 110;
      } 
      if (*(plot+i) < 50)
      {
        Tft.drawLine(PLOT_DIST*(i-1)+PLOT_Px_DATA, points[i-1], PLOT_DIST*(i)+PLOT_Px_DATA, 260 , RED);
        Tft.fillCircle(PLOT_DIST*(i-1)+PLOT_Px_DATA, 260, PLOT_POINT_T, YELLOW);
        points[i] = 260;
      } 




*/
    