#include <LiquidCrystal.h>



const int relayPin = 7;	    // use this pin to drive the transistor
bool relayIsOn = false;
float sensorAdjust = 0;

// temp ranges
const int lowTemp = 53;
const int highTemp = 57;

// init LCD library with pins
LiquidCrystal lcd(12,11,5,4,3,2);

void setup()
{
  // use 16x2 LCD
  lcd.begin(16, 2);
  lcd.clear();

  pinMode(relayPin, OUTPUT);  // set pin as an output

  pinMode(13, OUTPUT);
  Serial.begin(9600);
}


void loop()                    
{
  float sensor, voltage, degreesC, degreesF, voltageOnSensor, voltageFactor;

  // get exact voltage on sensor so we can get accurate temp
  voltageOnSensor = 5; //analogRead(5) * 5.0 / 1024.0;
  voltageFactor = voltageOnSensor / 1024.0;

  // calculate temperature
  sensor = analogRead(0);
  voltage = (sensor - sensorAdjust) * voltageFactor;
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;

  if (relayIsOn && degreesF < lowTemp)
  {
    digitalWrite(13, HIGH);
    digitalWrite(relayPin, LOW);  // turn the relay off
    sensorAdjust = 0;
    relayIsOn = false;
  }
  if (!relayIsOn && degreesF > highTemp)
  {
    digitalWrite(13, LOW);
    digitalWrite(relayPin, HIGH);   // turn the relay on

    // read the sensor with the relay on and compare to previous reading to get the adjustment value
    delay(1000);
    sensorAdjust = analogRead(0) - sensor;
    relayIsOn = true;
  }

  Serial.print("v on s: ");
  Serial.print(voltageOnSensor);
  Serial.print("  sensor: ");
  Serial.print(sensor);
  Serial.print("  voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
  Serial.print("  deg F: ");
  Serial.print(degreesF);
  if (relayIsOn)
  {
    Serial.print("  adjusted ");
    Serial.print("  ");
    Serial.print(sensorAdjust);
  }
  Serial.println();

  // display temp
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(degreesF);
  lcd.print("F");

  // display raw sensor
  lcd.setCursor(0, 1);
  lcd.print("Sensor: ");
  lcd.print(sensor);
  
  delay(1000);
} 

