#include <WiFi.h> //Include WiFi library to establish secure Wifi connection for your ESP32/ESP8266

const char* ssid = "________"; // Enter your network name
const char* pass = "________"; // Enter your network password

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid,pass); // Establish WiFi connection through ssid and password
  while(WiFi.status()!= WL_CONNECTED) // The condition checks whether the WiFi connection(status) is successful or not.'WL_CONNECTED' implies successful connection
  {
    Serial.print(".");
     delay(500);
  }
  Serial.println("The connection has been established successfully");
  Serial.print("The IP address of your connection is ");
  Serial.println(WiFi.localIP()); // "WiFi.localIP()" returns the IP address of the connected local network

}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED)
  {
     Serial.println("You can ping me"); // To check the connection status, open the command prompt and enter "ping <above returned IP address>" 
     delay(500);
  }
  else
  {
    Serial.println("Connection is lost");
    delay(500);
  }
}
