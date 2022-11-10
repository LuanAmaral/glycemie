#include <Arduino.h>
#include "main.hpp"
#include "lcd.hpp"
#include "Arduino_FreeRTOS.h"
#include "TFTv2.h"
#include "queue.h"


QueueHandle_t glycemie_queue;
uint16_t glycemie_data[GLY_DATA_SIZE] 
          = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
uint16_t glycemie_hours[GLY_HOURS_SIZE]; 
uint16_t glycemie_hours_index = 0;

uint8_t flags;
uint8_t alert;
uint16_t glycemie;
uint16_t sucre_meal;
uint16_t insuline;

void regulation(void* arg);
void screen(void* arg);
void recieve_data(void* arg);
void bluetooth_send_data(void* arg);

void send_message();
void convert_data(uint16_t* analog_data, float* glycemie_data);
void organise_plot_vector(uint16_t data_gly, uint16_t* glycemie_vector, uint8_t glycemie_vector_size );


void setup() {  
  // put your setup code here, to run once:
  //TFT_BL_ON;                                         //turn on the background light
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(S0, INPUT);
  pinMode(S1, INPUT);
  pinMode(E0, OUTPUT);
  pinMode(A_BOARD,INPUT);

  Tft.TFTinit(); 
  glycemie_queue = xQueueCreate(5, sizeof(uint16_t));
  //xTaskCreate(regulation, "regulation", 1024, NULL, 5, NULL);
  xTaskCreate(screen, "screen", 1024, NULL, 2, NULL);
  xTaskCreate(recieve_data, "recieve_data", 1024, NULL, 5, NULL);
  xTaskCreate(bluetooth_send_data, "bluetooth_send_data", 1024, NULL, 6, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void regulation(void* arg)
{
  //setup
  TickType_t ticks_delay = 16/portTICK_PERIOD_MS;
  int Yi = 0;
  int Ym1 = 111;
  int Ym2 = 111;
  float U = 0;
  float Um1 = 0;
  float Um2 = 0;

  while (1)
  {
    if(flags)
    {
      Serial.println("Entree dans if");
      Yi = glycemie - 111;
      U = (A*Yi + B*Ym1 - C*Ym2 + D*Um1 + E*Um2);
      if (U<0)
      {
        U = 0;
      }
      Ym2 = Ym1;
      Ym1 = Yi;
      Um2 = Um1;
      Um1 = U;
    
      Serial.println(Yi);
    
      //int test = analogRead(E0);
      Serial.println((U + Ub));
    
      analogWrite(E0, (U + Ub)*850);
      Serial.println((U + Ub)*850);
    }

    insuline = U;
    vTaskDelay(ticks_delay);
  } 
}

void screen(void* arg)
{
  TickType_t ticks_delay = 2000/portTICK_PERIOD_MS;
  uint16_t valeur[23] = {80, 73, 75, 90, 65, 70, 80, 85, 90, 100, 110, 105, 109, 110, 115, 120, 130, 150, 140, 135, 120, 110, 120};
  while (1)
  {
    main_screen(glycemie, glycemie_data, GLY_DATA_SIZE, 80, alert);  
    draw_mode(flags);

    vTaskDelay(ticks_delay);
    erase_glycemie();
    erase_graphic();
  }
}

void recieve_data(void* arg)
{
  int Yi = 0;
  int Ym1 = 111;
  int Ym2 = 111;
  float U = 0;
  float Um1 = 0;
  float Um2 = 0;
  float gly_data;

  int Mi = 0;




  uint16_t data;
  TickType_t ticks_delay = 16/portTICK_PERIOD_MS;
  while (1)
  {
      data = analogRead(A_BOARD);
      if(data < TOL_ANALOG)
      { 
        flags = EXEC;
        data = analogRead(S0);
        gly_data = data * GLY_CONV;
        data = analogRead(S1);
        if (data < 60)
        {
          data=0;
        }
        sucre_meal = data*GLY_CONV;

        if(gly_data < GLY_NO_SENSOR) alert = ALERT_NO_SENSOR;
        else if(gly_data > GLY_MAX_LIM) alert = ALERT_HIPER;
        else if(gly_data < GLY_MIN_LIM) alert = ALERT_HIPO;
        else alert = ALERT_NO;

        Serial.println(alert);

        Yi = gly_data - 111;
        U = (A*Yi + B*Ym1 - C*Ym2 + D*Um1 + E*Um2) + K*sucre_meal;
        if (U<0 || alert == ALERT_NO_SENSOR)
        {
          U = 0;
        }
        Ym2 = Ym1;
        Ym1 = Yi;
        Um2 = Um1;
        Um1 = U;
      
        analogWrite(E0, (U + Ub)*850);

        glycemie = (uint16_t)gly_data;
        if(alert == ALERT_NO_SENSOR) glycemie = 0;
        organise_plot_vector(glycemie, glycemie_data, GLY_DATA_SIZE);
        insuline = (U + Ub);
        
      }
      else
      {
        flags = NO_EXEC;
      }
      // converte to glycemie
      vTaskDelay(ticks_delay);
  }
}

void bluetooth_send_data(void* arg)
{
  TickType_t tick_delay = 16/portTICK_PERIOD_MS;

  while (1)
  {
    Serial1.print(DEVICE_ID);
    Serial1.print(",");
    Serial1.print(glycemie);
    Serial1.print(",");
    Serial1.print(insuline);
    Serial1.print(",");
    Serial1.print(sucre_meal);
    Serial1.print("\n");
    Serial1.flush();
    vTaskDelay(tick_delay);
  }
}


//===============================================================================================================
//functions

void convert_data(uint16_t* analog_data, float* glycemie_data)
{
   *(glycemie_data) = *(analog_data) * GLY_CONV ;                
}

void organise_plot_vector(uint16_t data_gly, uint16_t* glycemie_vector, uint8_t glycemie_vector_size )
{

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

}

