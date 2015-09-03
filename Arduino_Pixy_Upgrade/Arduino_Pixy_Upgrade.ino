
#include <SPI.h>  
#include <Pixy.h>

Pixy pixy;

#define X_CENTER        ((PIXY_MAX_X-PIXY_MIN_X)/2)       
#define Y_CENTER        ((PIXY_MAX_Y-PIXY_MIN_Y)/2)

int getX, Left, Right;
int getY;
int time , error;

class ServoLoop
{
public:
  ServoLoop(int32_t pgain, int32_t dgain);

  void update(int32_t error);
   
  int32_t m_pos;
  int32_t m_prevError;
  int32_t m_pgain;
  int32_t m_dgain;
};


ServoLoop panLoop(300, 500);
ServoLoop tiltLoop(500, 700);

ServoLoop::ServoLoop(int32_t pgain, int32_t dgain)
{
  m_pos = PIXY_RCS_CENTER_POS;
  m_pgain = pgain;
  m_dgain = dgain;
  m_prevError = 0x80000000L;
}

void ServoLoop::update(int32_t error)
{
  long int vel;
  char buf[32];
  if (m_prevError!=0x80000000)
  {	
    vel = (error*m_pgain + (error - m_prevError)*m_dgain)>>10;
    //sprintf(buf, "%ld\n", vel);
    //Serial.print(buf);
    m_pos += vel;
    if (m_pos>PIXY_RCS_MAX_POS) 
      m_pos = PIXY_RCS_MAX_POS; 
    else if (m_pos<PIXY_RCS_MIN_POS) 
      m_pos = PIXY_RCS_MIN_POS;
  }
  m_prevError = error;
}



void setup()
{
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT); //left
  pinMode(12, OUTPUT); //right
  
  Serial.print("Starting...\n");
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pixy.init();

}

void loop()
{ 
  
  uint16_t blocks;
  char buf[32]; 
  int32_t panError, tiltError;
  
  blocks = pixy.getBlocks();
  
  if (blocks)
  {
    
    time = 100;
    
    panError = X_CENTER-pixy.blocks[0].x;
    tiltError = pixy.blocks[0].y-Y_CENTER;
    
    panLoop.update(panError);
    tiltLoop.update(tiltError);
    
    pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
    getX = pixy.blocks[0].x; getY = pixy.blocks[0].y;
          
//    Serial.print("x: "); Serial.print(getX); Serial.print(" ");
//    Serial.print("y: "); Serial.println(getY);
    
    
    error = 150 - getX;
    
    analogWrite(3, 150 - error);
    analogWrite(5, 150 + error);

    if (getX < 145) {
        digitalWrite(10, 0);
        digitalWrite(12, 1);

        digitalWrite(9, 0); // none center
        Serial.println("LEFT");
    } else {
        digitalWrite(10, 1);
        digitalWrite(12, 0);

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
       digitalWrite(12, 0);
       Serial.println("CENTER");
       
      }
      
    }
  
} // End loop

