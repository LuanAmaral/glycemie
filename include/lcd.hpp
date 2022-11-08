#ifndef ldc_display
#define ldc_display

#include "TFTv2.h"

#define SCREENX_MAX     240 
#define SCREENY_MAX     320 
#define PLOTX_MIN       0
#define PLOTX_MAX       200
#define PLOTY_MIN       100
#define PLOTY_MAX       220
#define PLOT_H          200
#define PLOT_W          210
#define DONNEX_MIN      200
#define DONNEX_MAX      320
#define DONNEY_MIN      0
#define DONNEY_MAX      220
#define DONNEX_POS      260
#define DONNEY_POS      110 
#define DONNE_H         120
#define DONNE_W         220
#define N_PLOT          24
#define GLY_MIN         50
#define GLY_STEP        30
#define GLY_STEP_PX     40

void main_screen(uint16_t glycemie, uint16_t* plot, uint8_t size_of_plot, uint8_t batery);
void draw_glycemie(uint16_t draw_glycemie);
void draw_graphic(uint16_t* plot, uint8_t size_of_plot);
void draw_batery(uint8_t batery);
void draw_alert(void);


void erase_screen(void);
void erase_glycemie(void);
void erase_graphic(void);
void erase_batery(void);
void erase_alert(void);



#endif