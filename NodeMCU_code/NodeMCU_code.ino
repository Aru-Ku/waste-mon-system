#include<ESP8266WiFi.h> // ESP8266 & NodeMCU library
#include<Servo.h> // Servo Moto SG90 Library
#include <BlynkSimpleEsp8266.h>
#include <Blynk.h>

#define trig_us D1  // Trigger pin for both ultrasonic sensor.
#define echo_us1 D2 // Echo pin for 1st ultrasonic sensor.
#define echo_us2 D3 // Echo pin for 2nd ultrasonic sensor.
#define motor_pin D7 // D7 or GPIO2 pin for Servo-motor control.
#define RD_pin D5 // Raindrop sensor digital pin
#define IR_pin D6 // IR sensor digital pin

Servo s;  // Declaring an object for servo class
int RD_value = 0, IR_value = 0; // Variables for reading raindrop sensor & IR sensor Values
int t1, d1, t2, d2; // Variables for measuring system
int drylevel, wetlevel; // Float Variables for bin levels
float h = 20.0; // Height of Bins
int runner = 0; // Variable for selecting Segregation & Uploading functions
int time1 = 0; //for using as time

char auth[] = "BLYNK-TOKEN";
const char *ssid = "SSID-NAME"; // WiFi Name (SSID)
const char *pswd = "SSID-PASSWORD"; // WiFi Password
const char *host = "WEB-HOST-ADDRESS.000webhostapp.com"; // Webpage (host) to display Bin Levels
const int httpPort = 80; // Host port

WiFiClient client; 

void setup() {
  Serial.begin(115200); // Serial Monitor baud-rate
  pinMode(trig_us, OUTPUT); // making Trigger as output
  pinMode(echo_us1, INPUT); // making Echo 1 as input
  pinMode(echo_us2, INPUT); // making Echo 2 as input
  pinMode(RD_pin, INPUT); // RD sensor pin
  pinMode(IR_pin, INPUT); // IR sensor pin
  s.attach(motor_pin);  // Servo motor
  s.write(90);  // Setting Servo motor to 90 deg initially
  wifiConnecting(); // Function for connecting to WiFi.
  wifiConnected(); // Function to display IP Address after connection.
  Blynk.begin(auth, ssid, pswd);
}

void loop() {
  Blynk.run();
  
  if(WiFi.status() != WL_CONNECTED) { // checking frequntly for WiFi Connection
    Serial.println("WiFi Disconnected");
    Serial.println("Retrying to connect.....");
    if(WiFi.status() == WL_CONNECTED) { // Checking WiFi connection status
      wifiConnected(); // Function to display IP Address after connection.
    } else {
      Serial.println("Unable to connect");
    }
  }
  
  if(runner%5 ==0) { // Waste Detections works for every 5 seconds.
    detectWaste();
  }
  if(runner%60 == 0) { // Measuiring ststem works for every 1 minute.
    measuringSystem();
    blynkApplication();
    uploadingValues();
    runner = 1;
  }
  runner = runner + 1; // incrementing timer.
  delay(1000);
}

void detectWaste() {
  IR_value = digitalRead(IR_pin);
  if(IR_value == 0) {
    Serial.println("No waste");
  }
  else {
    RD_value = digitalRead(RD_pin);
    if(RD_value == 1) {
      Serial.println("Dry Waste detected.");
      detected_dry();
    }
    else {
      Serial.println("Wet Waste detected.");
      detected_wet();
    }
  }
}

void detected_wet() {
  s.write(170);
  delay(300);
  s.write(90);
}

void detected_dry() {
  s.write(15);
  delay(300);
  s.write(90);
}

void measuringSystem() {
  digitalWrite(trig_us, LOW);
  delayMicroseconds(3);
  digitalWrite(trig_us, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_us, LOW);
  t1 = pulseIn(echo_us1, HIGH);
  d1 = t1 * 0.034 / 2;
  delay(500);
  digitalWrite(trig_us, LOW);
  delayMicroseconds(3);
  digitalWrite(trig_us, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_us, LOW);
  t2 = pulseIn(echo_us2, HIGH);
  d2 = t2 * 0.034 / 2;

  drylevel = ((h - d1) / h) * 100;
  wetlevel = ((h - d2) / h) * 100;
}

void uploadingValues() {
  Serial.println("Dry Level = " + String(drylevel) + " Wet Level = " + String(wetlevel));
  Serial.println("Connecting to host to upload values");
  if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
  }
  if (d1 <= h && d2 <= h) {
    String url = "/api/in/insert.php?drylevel=" + String(drylevel) + "&wetlevel=" + String(wetlevel);
    Serial.println("Requesting URL: ");
    Serial.print(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "Connection: close\r\n\r\n");
    delay(500);
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
//    Serial.println();
//    Serial.println("Closing connection");
  }
  
}

void wifiConnecting() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(time1 == 60) { // waits for WiFi for 30 seconds
      Serial.println("Error connecting to Wifi.");
      break;
    }
    time1++;
  }
  if(WiFi.status() == WL_CONNECTED) { // Checking WiFi connection status
    wifiConnected(); 
  }
}

void wifiConnected() {
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void blynkApplication() {
  Blynk.virtualWrite(V1, drylevel);
  Blynk.virtualWrite(V2, wetlevel);
  if(drylevel >= 80 || wetlevel >= 80) {
    if(drylevel >= 80) { Blynk.notify("Dry Bin is about to fill"); }
    if(wetlevel >= 80) { Blynk.notify("Wet Bin is about to fill"); }
  }
  if(drylevel >= 80 && wetlevel >= 80) {
    Blynk.notify("Both bins are about to fill");
  }
}
