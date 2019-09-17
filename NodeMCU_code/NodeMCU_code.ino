#include <ESP8266WiFi.h>
#include <Servo.h>

#define trig_us D1   // Trigger pin for both ultrasonic sensor.
#define echo_us1 D2  // Echo pin for 1st ultrasonic sensor.
#define echo_us2 D3  // Echo pin for 2nd ultrasonic sensor.
#define motor_pin D4 // D4 or GPIO2 pin for Servo-motor control.
#define RD_pin D5    // Raindrop sensor digital pin
#define IR_pin D6    // IR sensor digital pin

Servo s; // servo class object
int RD_value = 0, IR_value = 0;
int t1, d1, t2, d2;
float drylevel, wetlevel;
float h = 20.0;
int runner = 0;

const char *ssid = "ak";
const char *pswd = "ak123456789";
const char *host = "iotgarbagemonitoringsystem.000webhostapp.com";
const int httpPort = 80;

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  pinMode(trig_us, OUTPUT);
  pinMode(echo_us1, INPUT);
  pinMode(echo_us2, INPUT);
  pinMode(RD_pin, INPUT); // RD sensor pin
  pinMode(IR_pin, INPUT); // IR sensor pin
  s.attach(motor_pin);    // Servo motor
  s.write(90);            // Setting Servo motor to 90 deg initially

  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.print(WiFi.gatewayIP());
  Serial.println();
}

void loop()
{
  if (runner % 5 == 0)
  {
    detectWaste();
  }
  if (runner % 60 == 0)
  {
    measuringSystem();
    uploadingValues();
    runner = 1;
  }
  runner = runner + 1;
  delay(1000);
}

void detectWaste()
{
  IR_value = digitalRead(IR_pin);
  if (IR_value == 0)
  {
    Serial.println("No waste");
  }
  else
  {
    RD_value = digitalRead(RD_pin);
    if (RD_value == 1)
    {
      Serial.println("Dry Waste detected.");
      detected_dry();
    }
    else
    {
      Serial.println("Wet Waste detected.");
      detected_wet();
    }
  }
}

void detected_wet()
{
  s.write(170);
  delay(300);
  s.write(90);
}

void detected_dry()
{
  s.write(15);
  delay(300);
  s.write(90);
}

void measuringSystem()
{
  digitalWrite(trig_us, LOW);
  delayMicroseconds(3);
  digitalWrite(trig_us, HIGH);
  delayMicroseconds(3);
  digitalWrite(trig_us, LOW);
  t1 = pulseIn(echo_us1, HIGH);
  d1 = t1 * 0.034 / 2;
  delay(500);
  digitalWrite(trig_us, LOW);
  delayMicroseconds(3);
  digitalWrite(trig_us, HIGH);
  delayMicroseconds(3);
  digitalWrite(trig_us, LOW);
  t2 = pulseIn(echo_us2, HIGH);
  d2 = t2 * 0.034 / 2;

  drylevel = (h - d1) / h;
  wetlevel = (h - d2) / h;
}

void uploadingValues()
{
  Serial.println("Connecting to host");

  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }

  if (d1 <= h && d2 <= h)
  {
    String url = "/api/in/insert.php?drylevel=" + String(drylevel * 100) + "&wetlevel=" + String(wetlevel * 100);
    Serial.println("Requesting URL: ");
    Serial.print(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(500);
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
    Serial.println("Closing connection");
  }
}
