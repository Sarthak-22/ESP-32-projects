#include <HTTPClient.h> // This library enables a client(ESP32)to communicate with a web server through HTTP requests(GET,POST).
#include <WiFi.h> // Library used to establish a WiFi connection with ESP32
#include <Arduino_JSON.h> // This Arduino library is used to parse and decode JSON files/objects. 

//Enter your network credentials
const char* ssid = "bhavesh v2";
const char* pass = "9821087287";

String APIkey = "22f1d2b02eb187197f60fe09413d58b7";  // Enter the API key that allows you to access data in JSON format from a web page.
String city = "Mumbai";
String countryCode = "IN";

String serverpath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + APIkey; // The path of web server from where the data would be data extracted. As an example, we use the 'openweathermap.org' as the web server and its free API key.

String jsonbuffer; // Acts as a placeholder for json objects and file.

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected successfully");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Connection failed");
  }
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}

void loop() 
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      jsonbuffer = httpGETRequest(serverpath.c_str());
      Serial.println(jsonbuffer); // Outputs the JSON string that contains all the weather information about the mentioned city.

      JSONVar myObject = JSON.parse(jsonbuffer); // Creates a JSON object which acts as a placeholder for the decoded JSON format file. 'parse()' function decodes the JSON file to a readable format.

      if (JSON.typeof(myObject) == "undefined")
      {
        Serial.println("Parsing input failed"); // This 'if' block checks whether the JSON object is an actual JSON file or not.
        return;
      }

      Serial.print("JSON Object: ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
    }
    else
    {
      Serial.println("Wifi disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* servername)
{
  HTTPClient http; // Creates an HTTP object that acts as a placeholder for all http requests/codes used to communicate with the server.
  http.begin(servername); //Establishes a connection with the web server.
  int httpResponseCode = http.GET();// Every connection has a response code affiliated to it. If the response code is positive then the connetion is successful and vise versa.

  String payload ="{}"; // This string contains all the the weather information in the city mentioned and returns it in JSON format.
  
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end(); // End server connection

  return payload;
}
