#include <dht11.h>

#include <Servo.h>

char str = 0;

Servo servoDoor;
Servo servoWindow;
dht11 DHT11;

int light1 = 2;
int servoPin1 =11;
int light2 = 4;
int servoPin2 = 5;
int fan1 = 6;
int fan2 = 7;
int flamedigital = 8;
int pirPin = 9;
int lockedLed = 10;
int humTempPin = 3;
int light3 = 12;
int buzzer = 13;
int valFlame; // define numeric variable val
int valMotion;

void setup ()
{
  Serial.begin(9600);
  servoDoor.attach(servoPin1);
  servoWindow.attach(servoPin2);


  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  pinMode(lockedLed, OUTPUT);
  pinMode(fan1, OUTPUT);
  digitalWrite(fan1,LOW);
  pinMode(fan2, OUTPUT);

  pinMode (flamedigital, INPUT) ;// input interface defines the flame sensor
  pinMode(pirPin, INPUT);
  pinMode (buzzer, OUTPUT); //output interface defines the buzzer
}

void loop ()
{

  //control
  if (Serial.available())
  {

    str = Serial.read();
    Serial.println(str);
    //main door
    switch (str)
    {
      //main door
      case 'o': {
          servoDoor.write(0);
          delay(100);
          str = 0;
          break;
        }
      case'O': {
          
          servoDoor.write(90);
          digitalWrite(light3, HIGH);
          digitalWrite(lockedLed, LOW);
          delay(100);
          str = 0;
          break;
        }
      case 'L': {
          digitalWrite(light1, HIGH);
          str = 0;
          break;
        }
      case 'l': {
          digitalWrite(light1, LOW);
          str = 0;
          break;
        }
      case 'I': {
          digitalWrite(light2, HIGH);
          str = 0;
          break;
        }
      case 'i': {
          digitalWrite(light2, LOW);
          str = 0;
          break;
        }
      case 'F': {
          analogWrite(fan1, 150);
          str = 0;
          break;
        }
      case 'f': {
          analogWrite(fan1, LOW);
          str = 0;
          break;
        }
      case 'G': {
          digitalWrite(fan2, HIGH);
          str = 0;
          break;
        }
      case 'g': {
          digitalWrite(fan2, LOW);
          str = 0;
          break;
        }
      case'h': {
          DHT11.read(humTempPin);
          str = 0;
          Serial.println(DHT11.humidity + '%');
          break;
        }
      case'T': {
          DHT11.read(humTempPin);
          str = 0;
          Serial.println(DHT11.temperature + 'C');
          break;
        }
      case'k': {
          lock();
          str = 0;
          break;
        }
    }
  }



  //flame sensor

  valFlame = digitalRead (flamedigital) ;// read flamedigital value and assigne it to val variable
  if (valFlame == LOW) // When the flame sensor detects a signal relay is on and buzzer sound (void alarm)
  {

    alarm();

    if (lockedLed == LOW)
    {
      if (servoDoor.read() < 80)
      {
        servoDoor.write(90);
      }
      if (servoWindow.read() < 80)
      {
        servoWindow.write(90);
      }

      delay(1000);
    }


  }
  else
  {
    digitalWrite(buzzer, LOW);
  }

  //intruder sensor


  if (valMotion == HIGH && digitalRead(lockedLed) == HIGH)
  {

    alarm();
  }


  //humidity & temprature sensor

  /*DHT11.read(humTempPin);
  Serial.println(DHT11.temperature + 'C');
  if (DHT11.temperature > 28)
  {
    digitalWrite(fan1, HIGH);
  }
  if (DHT11.humidity > 60)
  {
    digitalWrite(fan2, HIGH);
  }
  else
  {
    digitalWrite(fan2, LOW);
  }
  Serial.println(DHT11.humidity + '%');*/
  delay(100);
}

void alarm()  {
  tone(buzzer, 400, 500); //the buzzer emit sound at 400 MHz for 500 millis
  delay(10); //wait 500 millis
  tone(buzzer, 650, 500); //the buzzer emit sound at 650 MHz for 500 millis
  delay(10); //wait 500 millis
}
void lock()
{
  digitalWrite(light1, LOW);
  digitalWrite(light2, LOW);
  digitalWrite(light3, LOW);
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
  digitalWrite(lockedLed, HIGH);
  if (servoDoor.read() > 2)
  {
    servoDoor.write(0);
  }
  if (servoWindow.read() > 2)
  {
    servoWindow.write(0);
  }
}
