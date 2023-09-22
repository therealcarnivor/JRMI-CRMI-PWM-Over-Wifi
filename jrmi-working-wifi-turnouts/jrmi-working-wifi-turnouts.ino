#include "CMRI.h"
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>

#define TCP_PORT 9007
#define LED_BUILTIN 2
#define numServos 6 //The number of servos connected


const char* ssid = "ssid";
const char* password =  "password";

WiFiServer wifiServer(TCP_PORT);
WiFiClient jmriClient;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //setup the board address 0
CMRI cmri(1, 64, 64, jmriClient); // node number, number of inputs, number of outputs, strean client

int Status[numServos]; //Create a table to hold the status of each turnout, signal, etc.
int Throw[numServos]; //Create a table to hold the throw value for each servo
int Close[numServos]; //Create a table to hold the close value for each servo

void setup() {
  Serial.begin(115200); //Just for debug console feedback, not CMRI connection
  pwm.begin();
  pwm.setPWMFreq(50);  // This is the maximum PWM frequency
  delay(1000);
  
  //SET THE THROW AND CLOSE VALUES FOR EACH SERVO BASED ON THE CALIBRATION PROCESS
  
  //Servo connection 0 - point motor
  Throw[0] = 1015;
  Close[0] = 1800;

  //Servo connection 1 - point motor
  Throw[1] = 1015;
  Close[1] = 1800;

  //Servo connection 2 - point motor
  Throw[2] = 1015;
  Close[2] = 1800;

  //Servo connection 3 - point motor
  Throw[3] = 1015;
  Close[3] = 1800;

  //Servo connection 4 - point motor
  Throw[4] = 1015;
  Close[4] = 1800;

  //Servo connection 5 - point motor
  Throw[5] = 1015;
  Close[5] = 1800;

  WiFi.hostname("CRMI1");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
  delay(1000);

  InitialiseConfig();
}

void loop() {
  
  waitForJMRI();

  cmri.process();

  for (int i = 0; i < numServos; i++) {
      Status[i] = (cmri.get_bit(i));
      if (Status[i] == 1){
          pwm.writeMicroseconds(i, Throw[i]);
      }
      else {
          pwm.writeMicroseconds(i, Close[i]);
      }
  }

}

bool waitForJMRI() {
  bool jmriConnected = jmriClient.connected();
  while (!jmriConnected) {
    jmriClient = wifiServer.available();
    if (jmriClient && jmriClient.connected()) {
      jmriConnected = true;
      Serial.println("JMRI Connected");
    }
  }
  return true;
}

void InitialiseConfig() {
  pinMode(23, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
}

