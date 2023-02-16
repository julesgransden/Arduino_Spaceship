#include <Arduino.h> //import all necessary libraries
#include <U8x8lib.h> 
#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

float bulletVPosition= 0;//initialize variable to keep track of bullet vertical position 
float bulletHPosition= 0;//initialize variable to keep track of bullet horizontal position 
float Xval[5];//X value array for Median filter of N=5
float Yval[5];//Y value array for Median filter of N=5

//Let us declare the space ship, bullet and clear tiles
uint8_t planeRight[16] = {0,128,128,128,128,192,240,252,128,128,128,192,224,0,0,0};
uint8_t planeLeft[16] = {1,3,3,3,3,7,31,127,3,3,3,7,15,1,0,0};
uint8_t bullet[16] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
uint8_t Clear[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  

void setup(void) {
  Serial.begin(2000000);//set baud rate for serial data
  while (!Serial)
  {
  };
  //initializwe accelerometer values
  LIS.begin(WIRE, LIS3DHTR_ADDRESS_UPDATED); //IIC init
  delay(100);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  //this instructions allows me to set the OLED screen in high resolution mode. 
  LIS.setHighSolution(true);
  u8x8.begin(); // initialize the screen
//enable flip mode of screen
  u8x8.setFlipMode(1);
}

void loop(void) { //Now we enter continuous loop
  
  int b = digitalRead(4);//read digital output from PushButton(D4)
  float x = ((((LIS.getAccelerationX()+1.01)*2.00)/2.04)-1.00)*-10;//Get x acceleration after correction 
  float y = ((((LIS.getAccelerationY()+1.03)*2.00)/2.05)-1.00)*-10;//Get Y acceleration after correction 
//Note: We multiply by -10 to get a tangiable value and also to allow the spaceship to move in the direction in which we tilt the sensor

  //Now, lets apply the moving avergae to the values of the accelerometer to have a smoother game
    for(int i=4; i>0; i--){ //create and array of the past 5 values 
    Xval[i]=Xval[i-1];
    Yval[i]=Yval[i-1];
  }
  Xval[0] = x;//every itteration of the loop all the values are shifted to the right and the the new X/Y is placed at position 0
  Yval[0] = y;
  int Xsum=0;//initialize the X avg variable that will hoild the sum of X array
  int Ysum=0;//initialize the Y avg variable that will hoild the sum of Y array

//Get avg of the array to get the final value of acceleration that will be passed to the oled screen
 for(int i=0; i<5; i++){
  Xsum = Xsum + Xval[i];
  Ysum = Ysum + Yval[i];
  }
 x = Xsum/5;//final value of X/Y ready to be passed to OLED screen
 y = Ysum/5;

// draw the spaceship at its position depending on the values of the x-y accelerations
  u8x8.drawTile(9+y,3+x, 2, planeRight);
  u8x8.drawTile(9+y,4+x, 2, planeLeft);
//The following sequence describes the bullet on the screen
  if(b==1){ //condition will be true if button is pressed
  u8x8.drawTile(bulletVPosition, bulletHPosition, 2, Clear); //clear any bullet previously initialized
  bulletVPosition=9+y;//set Vertical and Horizontal position of bullet to front of the ship
  bulletHPosition=4+x;
  }
//to allow bullet to move forward, we will clear and reprint the bullet every iteration at a different location
  if(bulletVPosition>0){//if bullet is not at the end of the screen 
    u8x8.drawTile(bulletVPosition, bulletHPosition, 2, Clear); // clear current position of bullet
    u8x8.drawTile(bulletVPosition, bulletHPosition, 1, bullet);//draw bullet 
    bulletVPosition= bulletVPosition-0.1;//move bullet vertical position forward 
  }
  else{//when we get to the end of the screen
    u8x8.drawTile(bulletVPosition, bulletHPosition, 2, Clear);//clear one final time 
  }
  u8x8.drawTile(9+y,3+x,2, Clear); //clear the current position of the spaceship
  u8x8.drawTile(9+y,4+x, 2, Clear);
}

