#include <Arduino.h>
#include "main.hpp"
#include "lcd.hpp"
#include "Arduino_FreeRTOS.h"
#include "TFTv2.h"
#include "queue.h"
#include "SoftwareSerial.h"

int8_t flags;
QueueHandle_t glycemie_queue;
uint16_t glycemie_data[GLY_DATA_SIZE] 
          = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
uint16_t glycemie_hours[GLY_HOURS_SIZE]; 
uint16_t glycemie_hours_index = 0;

uint16_t glycemie;
uint16_t sucre_meal;

void regulation(void* arg);
void screen(void* arg);
void recieve_data(void* arg);

void send_message();
void convert_data(uint16_t* analog_data, uint16_t* glycemie_data);
void organise_plot_vector(uint16_t data_gly, uint16_t* glycemie_vector, uint8_t glycemie_vector_size );


void setup() {  
  // put your setup code here, to run once:
  //TFT_BL_ON;                                         //turn on the background light
  Serial.begin(9600);
  Serial.println("START____________________");
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
  TickType_t ticks_delay = 16/portTICK_PERIOD_MS;

  while (1)
  {
    
    vTaskDelay(ticks_delay);
  } 
}


void screen(void* arg)
{
  TickType_t ticks_delay = 2000/portTICK_PERIOD_MS;
  uint16_t valeur[23] = {80, 800, 705, 90, 65, 35, 400, 35, 100, 130, 80, 145, 178, 100, 70, 110, 130, 150, 120, 500, 500, 120, 120};
  while (1)
  {
    main_screen(glycemie, glycemie_data, GLY_DATA_SIZE, 80);  
    //draw_alert();
    draw_mode(flags);
    vTaskDelay(ticks_delay);
    erase_glycemie();
    erase_graphic();
  }
}

void recieve_data(void* arg)
{
  pinMode(A, INPUT);
  uint16_t data;
  TickType_t ticks_delay = 16/portTICK_PERIOD_MS;
  while (1)
  {
      data = analogRead(A);
      if(data < 1023 - TOL_ANALOG)
      { 
        flags = EXEC;
        data = analogRead(S0);
        convert_data(&data, &glycemie);
        Serial.println(data);
        organise_plot_vector(glycemie, glycemie_data, GLY_DATA_SIZE);
        data = analogRead(S1);
        convert_data(&data, &sucre_meal);
      }
      else
      {
        flags = NO_EXEC;
      }
      // converte to glycemie
      vTaskDelay(ticks_delay);
  }
}

//===============================================================================================================
//functions

void convert_data(uint16_t* analog_data, uint16_t* glycemie_data)
{
   *(glycemie_data) = *(analog_data) * GLY_CONV ;                
}

void organise_plot_vector(uint16_t data_gly, uint16_t* glycemie_vector, uint8_t glycemie_vector_size )
{
  //Serial.println(glycemie_hours_index);

  if( glycemie_hours_index < GLY_HOURS_SIZE )
  {
    glycemie_hours[glycemie_hours_index] = data_gly;
    glycemie_hours_index++;
    return;
  }
  glycemie_hours_index = 0;
  float glycemie_for_plot = 0;

  for (uint8_t i = 0; i < GLY_HOURS_SIZE; i++)
  {
    glycemie_for_plot += glycemie_hours[i];
  }
  glycemie_for_plot = glycemie_for_plot/GLY_HOURS_SIZE;
  
  for(int i=1; i<glycemie_vector_size; i++)
  {
    glycemie_vector[i-1] = glycemie_vector[i];
  }
  glycemie_vector[glycemie_vector_size-1] = (uint16_t)glycemie_for_plot;
  //Serial.println(glycemie_vector[glycemie_vector_size-1]);
}

