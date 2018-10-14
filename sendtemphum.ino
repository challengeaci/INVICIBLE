#include<ESP8266WiFi.h>
#include<DHT.h>
#define DHTPIN D7
DHT dht(DHTPIN,DHT22);
String apikey="xxxxxxxx";
const char *ssid="xxxxxx";
const char *pass="xxxxxx";
const char* server="api.thingspeak.com";
WiFiClient client;
void setup() {
  Serial.begin(115200);
  delay(250);;
  dht.begin();
  while(!Serial){}
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED)
  {
  Serial.print(".");
  delay(10);
  }
Serial.println("");
Serial.println("WIFI CONNECTED");
sendtemphum();
delay(500);
ESP.deepSleep(12e6);
}
void loop() {}
void sendtemphum(void){
  float hum=dht.readHumidity();
  float temp=dht.readTemperature();
  
  Serial.println("Temp:");
  Serial.println(temp);
  Serial.println("HUm:");
  Serial.println(hum);
if(isnan (temp)||isnan(hum))
{
  Serial.println("error to read the data");
  return;
}
if(client.connect(server,80))//"184.106.153.149" or api.thingspeak.com
  {
    Serial.println("I'm in client connect");
    String postStr=apikey;
    postStr += "&field5=";
     postStr += String(temp);
     postStr += "&field6=";
     postStr += String(hum);
     postStr += "\r\n\r\n";
     client.print("POST /update HTTP/1.1\n");
     client.print("host:api.thingspeak.com\n");
     client.print("connection close\n");
     client.print("X-THINGSPEAKAPIKEY: " + apikey + "\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-length:");
     client.print(postStr.length());
     client.print("\n\n");
      client.print(postStr);
     Serial.print("temperature:");
     Serial.print(temp);
     Serial.print("in degreescelcius, Humidity");
     Serial.print(hum);
     Serial.println("%.data sent to thingspeak");
    }
  client.stop();
}
