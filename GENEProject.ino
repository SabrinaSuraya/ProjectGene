
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "cA_CeMZv2H6Tkmex-NxTcCZBFtYxNT6C";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Sasu";
char pass[] = "roxasxiii";

#define DHTPIN 7          // What digital pin we're connected to
#define SoilPin A0
#define WaterPump 6

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  
}

void SoilMoistureDetector()
{
  float moisture_percentage;

  moisture_percentage = analogRead(SoilPin);

  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Serial.println("%");
  Blynk.virtualWrite(V7, moisture_percentage);
  delay(1000);

  if(moisture_percentage>= 500)
  {
    Blynk.notify(" Hey, I thirsty right now! Need water please!");
    digitalWrite(WaterPump, HIGH);
    delay(1000);
  }
  else
  {
    Blynk.notify(" Gulp Gulp Already Full!");
    digitalWrite(WaterPump, LOW);
    delay(1000);
  }
}
void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(SoilPin, INPUT);
  pinMode(WaterPump, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  SoilMoistureDetector();
}

void loop()
{
  Blynk.run();
  timer.run();
}
