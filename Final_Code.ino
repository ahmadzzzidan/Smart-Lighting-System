#define BLYNK_TEMPLATE_ID "TMPLvXLStkI_"
#define BLYNK_DEVICE_NAME "Lighting"
#define BLYNK_AUTH_TOKEN "tKFe3ZUn_-x9NiJiJAoCeiJVJaSizSoy"

#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "henrie.xo"; //hotspot name
char pass[] = "7175henriexo"; //hotspot password

#include <SoftwareSerial.h> //Software Serial on Uno
SoftwareSerial EspSerial(3, 2); // RX, TX pin

#define ESP8266_BAUD 9600 //ESP8266 baud rate

ESP8266 wifi(&EspSerial);

const int RELAY_PIN_1 = A1; //Relay Pin
const int RELAY_PIN_2 = 13;
const int analogPin = A0; //Battery Pin
int offset =20;
const int PIN_TO_SENSOR = 12;   // the pin that OUTPUT pin of sensor is connected to
const int led_1 = 7;
const int led_2 = 8;
int pinStateCurrent   = LOW; // current state of pin
int pinStatePrevious  = LOW; // previous state of pin
float analogValue;
float input_voltage;

BLYNK_WRITE(V13)
{
 int pinvalue=param.asInt();
 digitalWrite(13, pinvalue);  
 }
 
void setup() {
  Serial.begin(115200);     // initialize serial
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);

  pinMode(PIN_TO_SENSOR, INPUT); 
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
}

void loop() {
  Blynk.run();
  pinStatePrevious = pinStateCurrent; // store old state
  pinStateCurrent = digitalRead(PIN_TO_SENSOR);   // read new state
  int volt = analogRead(A0); //read the A0 pin value
  double voltage = map(volt,0,1023, 0, 2500) + offset;
  voltage /=100;
  if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    if (voltage > 12.3) {
      digitalWrite(RELAY_PIN_1, HIGH);
      Serial.print("Using Battery");
      digitalWrite(RELAY_PIN_2, HIGH);
      digitalWrite(led_2, HIGH);
      Blynk.logEvent ("motion_alert", "Living room lights are turned on!");
    }
    else {
      digitalWrite(RELAY_PIN_1, LOW);
      Serial.print("Using Power Supply");
      digitalWrite(RELAY_PIN_2, HIGH);
      digitalWrite(led_1, HIGH);
      Blynk.logEvent ("motion_alert", "Living room lights are turned on!");
    }
  }
  else
  if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);    
  }
}

