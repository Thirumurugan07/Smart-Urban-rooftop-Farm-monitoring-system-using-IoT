#include <DHT.h>
#include <BlynkSimpleEsp8266.h>// #include <WiFiClient.h>
#define BLINK_PRINT Serial
 #define DHTTYPE DHT11   // DHT 11
 #define DHTPIN D6       // DHT 11 data pin
#define LEDPIN 2
#define LDR_PIN D2
#define BLYNK_TEMPLATE_ID "BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_ID"

char auth[] = BLYNK_AUTH_TOKEN;
const char *ssid = "YOUR_WIFI_SSID";
const char *pass = "YOUR_WIFI_PASSWORD";
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer; 

float moisture_level,h,t;
int moisture_percentage=0;
const int relay = D1;

void sendSensor()
{
  moisture_level =  (analogRead(A0) );  //put Sensor insert into soil
   
  moisture_percentage=(map(moisture_level,693,480,0,100));
  if (output > 100) output = 100;
   if (output < 0) output = 2;
   delay(1000);
   Serial.print("moisture percentage: ");
   Serial.println(moisture_percentage);
   Blynk.virtualWrite(V2, moisture_percentage);
    
   h = dht.readHumidity();
   t = dht.readTemperature();
   bool light = digitalRead(LDR_PIN);

  Serial.print("LDR: ");
  Serial.println(light);   

  if (isnan(h) || isnan(t)) {
  Serial.println('F');
  return;
  }
  
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V6, light);
  Blynk.virtualWrite(V4, h);
   Serial.print("HUMIDITY: ");
  Serial.println(h);  

  Serial.print("TEMPERATURE:");
  Serial.println(t); 
  delay(1000);
  if (light == 0) {
     if (t > 30) {
    if(h <40 && output <40 )  {  
       digitalWrite(relay, LOW); 
       Serial.println("Current Flowing"); 
    }       
    
    else {
      digitalWrite(relay, HIGH);
    Serial.println("Current Not Flowing"); 
    } 
     } 
  }
  else{
    digitalWrite(relay, HIGH);
    Serial.println("Current Not Flowing");  
  }  
}

void setup()
{
  pinMode(relay, OUTPUT); 
  pinMode(D7, OUTPUT); 
  pinMode(A0, INPUT);
  pinMode(D6, INPUT);
  pinMode(D2, INPUT);
  Serial.begin(9600);
  Blynk.begin(auth,ssid,pass,"blynk.cloud",80);
  dht.begin();
  timer.setInterval(100L, sendSensor);  

}
   
void loop() {
  Blynk.run();
  timer.run();
}
