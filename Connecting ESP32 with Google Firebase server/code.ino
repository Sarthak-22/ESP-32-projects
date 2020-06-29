#include <FirebaseESP32.h>    // Connecting IOT devices through Google Firebase cloud storage database server.
#include <WiFi.h>

FirebaseData firebaseData;

const int led1 = 18;
const int led2 = 19;

// Enter your WiFi network credentials
const char* ssid = "__________";
const char* pass = "__________";

// Enter your Firebase database host and secret key.
const char* Firebase_host = "esp32-led-***.firebaseio.com";
const char* Firebase_auth = "4rSAT6IXB875j6Ih3bw***********";

void setup() 
{
  Serial.begin(115200);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("Connected to WiFi successfully");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  
  
  Firebase.begin(Firebase_host,Firebase_auth); // Establishes connection between ESP32 module and Firebase server.

  Firebase.reconnectWiFi(true);

  if (Firebase.setString(firebaseData,"/RED","OFF")) // Returns a boolean value if it successfully sets a string node "RED" with value "OFF"
  {
    Serial.println("Create RED LED node success");
  }
  else 
  {
    Serial.println("Create RED LED node failed");
    Serial.println(firebaseData.errorReason()); // Displays the error reason
  }

  if (Firebase.setString(firebaseData,"/GREEN","OFF"))
  {
    Serial.println("Create GREEN LED node success");
  }
  else 
  {
    Serial.println("Create GREEN LED node failed");
    Serial.println(firebaseData.errorReason());
  }
}

void loop() 
{
  if(Firebase.getString(firebaseData,"/RED")) // Returns a boolean value if it successfully receives a string value from the "RED" string node in the database
  {
    Serial.println("GET RED LED DATA SUCCESS");
   
      String val1 = firebaseData.stringData(); // Gets the value from that particular string node and stores it in string variable "val1"
      //Serial.println(val1);
      if (val1 == "ON")
      {
        Serial.println("Red LED is ON");
        digitalWrite(led1,HIGH);
        delay(200);
        Serial.println("");
      }
      else if (val1 == "OFF")
      {
        Serial.println("Red LED is OFF");
        digitalWrite(led1,LOW);
        delay(200);
        Serial.println("");
      }
      else 
      {
        Serial.println("Error input for RED LED");
        Serial.println(firebaseData.errorReason());
      }
    }

    if(Firebase.getString(firebaseData,"/GREEN"))
   {
     Serial.println("GET GREEN LED DATA SUCCESS");
      String val2 = firebaseData.stringData();
      //Serial.println(val2);
      if (val2 == "ON")
      {
        Serial.println("Green LED is ON");
        digitalWrite(led2,HIGH);
        delay(200);
        Serial.println("");
      }
      else if (val2 == "OFF")
      {
        Serial.println("Green LED is OFF");
        digitalWrite(led2,LOW);
        delay(200);
        Serial.println("");
      }
      else 
      {
        Serial.println("Error input at Green LED");
        Serial.println(firebaseData.errorReason());
      }
    }
  
}
