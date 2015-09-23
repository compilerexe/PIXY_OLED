#include <SPI.h>  
#include <Pixy.h>

Pixy pixy;

int getX, Left, Right;
int getY;
int time , error;

void setup()
{
  Serial.begin(115200);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT); //left
  pinMode(8, OUTPUT); //right
  
  Serial.print("Starting...\n");
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pixy.init();

}

void loop()
{ 
  
  uint16_t blocks;
  
  blocks = pixy.getBlocks();
  
  if (blocks)
  {
    
    time = 100;
    
    getX = pixy.blocks[0].x; getY = pixy.blocks[0].y;
          
//    Serial.print("x: "); Serial.print(getX); Serial.print(" ");
//    Serial.print("y: "); Serial.println(getY);
    
    error = 150 - getX;
    
    analogWrite(3, 150 - error);
    analogWrite(5, 150 + error);

    if (getX < 145) {
        digitalWrite(10, 0);
        digitalWrite(8, 1);

        digitalWrite(9, 0); // none center
        Serial.println("LEFT");
    } else {
        digitalWrite(10, 1);
        digitalWrite(8, 0);

        digitalWrite(9, 0); // none center
        Serial.println("RIGHT");
    }
    
    delay(25);
    
    }else{
      
      time--;
      
      if(time < 0){
       
       analogWrite(3,0);
       analogWrite(5,0);
       digitalWrite(9, 1);
       digitalWrite(10, 0);
       digitalWrite(8, 0);
       Serial.println("CENTER");
       
      }
      
    }
  
} // End loop

