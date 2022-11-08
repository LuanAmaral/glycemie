#include <Arduino.h>
#include "TFTv2.h"
#include "lcd.hpp"
#include "Arduino_FreeRTOS.h"
#include "queue.h"

#define GLY_DATA_SIZE 23

QueueHandle_t glycemie_queue;
uint16_t glycemie_data[GLY_DATA_SIZE] 
          = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

void regulation(void* arg);
void screen(void* arg);
void recieve_data(void* arg);

void convert_data(uint16_t* analog_data, uint16_t* glycemie_vector, uint8_t glycemie_vector_size);

void setup() {  
  // put your setup code here, to run once:
  //TFT_BL_ON;                                         //turn on the background light

  Tft.TFTinit(); 
  glycemie_queue = xQueueCreate(5, sizeof(uint16_t));
  xTaskCreate(regulation, "regulation", 1024, NULL, 5, NULL);
  xTaskCreate(screen, "screen", 1024, NULL, 2, NULL);
  xTaskCreate(recieve_data, "recieve_data", 1024, NULL, 3, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void regulation(void* arg)
{
  //setup

  while (1)
  {
    vTaskDelay(portMAX_DELAY);
  } 
}


void screen(void* arg)
{
  uint16_t valeur[23] = {500, 600, 700, 400, 500, 600, 700, 400, 500, 600, 700, 400, 500, 600, 700, 400, 500, 600, 700, 400, 500, 600, 700};
  uint16_t glicemie = 111;
  TickType_t ticks_delay = 500/portTICK_PERIOD_MS;
  while (1)
  {
    main_screen(glicemie, glycemie_data, 23, 80);  
    draw_alert();
    vTaskDelay(ticks_delay);
    erase_glycemie();
    erase_batery();
    main_screen(555, valeur, 23, 65); 
    vTaskDelay(ticks_delay);
  }
}

void recieve_data(void* arg)
{
  pinMode(A8, INPUT);
  uint16_t data;
  TickType_t ticks_delay = 500/portTICK_PERIOD_MS;
  while (1)
  {
      data = analogRead(A8);
      // converte to glycemie
      convert_data(&data, glycemie_data, GLY_DATA_SIZE);
      vTaskDelay(ticks_delay);
  }
}

//===============================================================================================================
//functions

void convert_data(uint16_t* analog_data, uint16_t* glycemie_vector, uint8_t glycemie_vector_size)
{

}
