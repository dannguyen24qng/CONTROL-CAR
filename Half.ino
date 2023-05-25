#include <BluetoothSerial.h>


const int trigPin = 19 ;
const int echoPin = 18; 

const int enablePinA = 33;       
const int motorPin1  = 25;       
const int motorPin2  = 26;     
const int motorPin3  = 27;       
const int motorPin4  = 14;
const int enablePinB = 12;       

const int turnDelay = 750;
const int stopDelay = 300;
const int minDistance = 15;

int   status1= false ;
int   status2= false ;
int motorSpeed = 180;
long khoangcach1 = 0;
char kytu ;
String chuoi; 


// Khai báo biến PWM
const int freq = 30000;
const int channel = 0;
const int resolution = 8;
// Khai báo biến bluetooth
BluetoothSerial serialBT;



void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  serialBT.begin("Control Car");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  

  pinMode(enablePinA, OUTPUT);
  pinMode(motorPin1,  OUTPUT);
  pinMode(motorPin2,  OUTPUT);
  pinMode(motorPin3,  OUTPUT);
  pinMode(motorPin4,  OUTPUT);
  pinMode(enablePinB, OUTPUT);

  // Khởi tạo PWM để điều khiển tốc độ động cơ
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(enablePinA, channel);
  ledcAttachPin(enablePinB, channel);
  // ttc=0; //trang thai nut nhan chuyen
}

void loop() 
{
  // dokhoangcach();
  voice();
   if (serialBT.available()) 
   {
    kytu=serialBT.read();
    chuoi=chuoi+kytu;
    Serial.println(kytu);
    
    if (kytu == 'A')
    {
      status1 = true;
    }
    else if (kytu == 'T')
    {
      status1 = false;
    }
     if (kytu == 'M')
    {
      status2 = true;
    }
    else if (kytu == 'N')
    {
      status2 = false;
    }
  }
   if (status1) 
   {
     autocar();
   }
     if (status2) 
   {
     manual();
   }
}


void voice()
{
    if(chuoi.indexOf("thẳng")>=0)
      {
        carforward();
        delay(2000);
        carstop();
        chuoi="";
      }
    if(chuoi.indexOf("lùi")>=0)
      {
        carbackward();
        delay(2000);
        carstop();
        Serial.println(chuoi);
        chuoi="";
      }
     if(chuoi.indexOf("trái")>=0)
      {
        carturnleft();
        delay(750);
        carstop();
        Serial.println(chuoi);
        chuoi="";
        
      }
    if(chuoi.indexOf("phải")>=0)
      {
        carturnright();
        delay(750);
        carstop();
        Serial.println(chuoi);
        chuoi="";
       
      }
}
void manual()
{
    Serial.write(kytu);
    switch(kytu)
    {
      case 'U':
        carforward();
        break;
      case 'D': 
        carbackward();
        break;
      case 'L':
        carturnleft();
        break;
      case 'R':
        carturnright();
        break;
      case 'S':
        carstop();
        break;
    }

}

void carforward() 
{
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  ledcWrite(channel, motorSpeed);
}
void carbackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  ledcWrite(channel, motorSpeed);
}
void carturnleft() 
{
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  ledcWrite(channel, motorSpeed);
}
void carturnright() 
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  ledcWrite(channel, motorSpeed);
}
void carstop() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
void autocar() {
  long khoangcach = 0;
  khoangcach = dokhoangcach();

  if (khoangcach > minDistance || khoangcach == 0) {
    khoangcach = dokhoangcach();
    if (khoangcach > minDistance || khoangcach == 0) {
      carforward();
      Serial.println("Di thang");
    }
  }
  else {
    carstop();
    delay(stopDelay);
    khoangcach1 = dokhoangcach();
    if (khoangcach1 < 15 || khoangcach == minDistance )
    {
      carstop();
      carbackward();
      delay(turnDelay);
      carstop();
      delay(stopDelay);
      carturnleft();
      delay(turnDelay);      
      carstop();
      delay(stopDelay);
    }
    else {
      carstop();
      carforward();
      Serial.println("Di thang");
    }
  }
}

long dokhoangcach() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long thoigian = pulseIn(echoPin, HIGH);
  long khoangcach = thoigian / 2 / 29.412; //* 0.034 / 2;  /
  return khoangcach;
}
