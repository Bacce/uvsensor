#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int highest_uv_index = 0;
float highest_sensorValue = 0;

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;);
  }
}

void loop() { 
  float sensorValue = readSensor();

  float sensorVoltage = sensorValue/1024*5.0;
  int uv_index = sensorVoltage / 0.1;
  if(highest_sensorValue < sensorValue){
    highest_sensorValue = sensorValue;
  }

  if(highest_uv_index < uv_index){
    highest_uv_index = uv_index;
  }
  
  //writeText(uv_index, sensorValue);

  // Lock the highest readed value on screen
  writeText(uv_index, highest_uv_index, sensorValue, highest_sensorValue);
}

float readSensor() {
  int s = 100; // Sampling count
  float sum = 0;
  
  for(int i=0; i<s; i++) {
    float v = analogRead(A0);
    sum = v + sum;
    delay(2);
  }
  float sensor_value_average = sum / s;
  
  return sensor_value_average;
}

int writeText(int num, int num_b, float num2, float num3) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("UV:");
  
  display.setTextSize(3);  
  display.print(num);
  
  display.setTextSize(2);
  display.print(" ^");
  display.print(num_b);

  display.setTextSize(2);
  display.setCursor(0, 28);
  if(num>=0 && num<3){display.print("ALACSONY");}
  if(num>=3 && num<6){display.print("MERSEKELT");}
  if(num>=6 && num<8){display.print("MAGAS");}
  if(num>=10){display.print("MAGAS ++");}

  display.setTextSize(1);
  display.setCursor(0, 47);
  display.print("Spektrum:200-370nm");
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("Szenzor:");
  display.print(num2);
  display.print("^");
  display.print(num3);

  display.display();
  delay(500);
}
