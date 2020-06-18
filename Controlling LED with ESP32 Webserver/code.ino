#include <WiFi.h> // Include the WiFi library to make connection with webserver

// Enter your network credentials
const char* ssid = "__________";
const char* pass = "__________";

WiFiServer server(80); // This command instructs the ESP32 to create a server at port 80 which is essentially a webserver
String header; // Variable to store the http requests and outputs the process at the backend of the server.

String output26state = "off"; // Auxiliar variables to control the current output state
String output27state = "off"; //   "         "       "    "     "     "      "      "

const int output26 = 26; // Indicates the position of the outpins 
const int output27 = 27;

unsigned long currentTime = millis(); 
unsigned long previousTime = 0;
const long timeoutTime = 2000; // 2 seconds delay before the process starts to ensure proper WiFi connection

void setup() {
  Serial.begin(115200);
  pinMode(output26,OUTPUT);
  pinMode(output27,OUTPUT);
  digitalWrite(output26,LOW); //Sets the initial output to 0
  digitalWrite(output27,LOW);

  // Setup the WiFi connection
  WiFi.begin(ssid,pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to WiFi successfully");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    server.begin(); // Establish connection with the webserver 
  }
  
  else
  {
    Serial.println("WiFi connection failed");
  }

}

void loop() 
{
  WiFiClient client = server.available(); // Creates an object for client and establishes connection between the client and webserver

  // If a client connects to the webserver then the above object takes a logic True value. 
  if (client)
  {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New client connected");
    String currentLine = ""; // This string holds the incoming data from the client.
    while (client.connected() && (currentTime - previousTime <= timeoutTime)) // loop while the clients connected
    {
      currentTime = millis();
      if (client.available()) // If there's incoming bytes from client then execute this block
      {
        char c = client.read(); // Reads the incoming string from client and stores it in string variable c
        Serial.write(c);// Print the string on Serial monitor
        header += c;
        if (c == '\n') // If the byte is a newline character
        {
          // If the current line is blank as well then we get two newline characters in a row
          // This marks the end of client HTTP request so send a response
          if (currentLine.length() == 0)
          {
            // HTTP header always starts with a response code (e.g HTTP/1.1 200 OK) and a content type which denotes the type of page and closes the connection with a blank line
            client.println("HTTP/1.1 200 OK");
            client.println("Content type: text/html");
            client.println("Connection close");
            client.println("");

            if (header.indexOf("GET /26/on") >=0)
            {
              Serial.println("GPIO pin 26 is on");
              output26state = "on";
              digitalWrite(output26,HIGH);
            }
            else if (header.indexOf("GET /26/off") >=0)
            {
              Serial.println("GPIO pin 26 is off");
              output26state = "off";
              digitalWrite(output26,LOW);
            }
            else if (header.indexOf("GET /27/on") >=0)
            {
              Serial.println("GPIO pin 27 is on");
              output27state = "on";
              digitalWrite(output27,HIGH);
            }
            else if (header.indexOf("GET /27/off") >=0)
            {
              Serial.println("GPIO pin 27 is on");
              output27state = "off";
              digitalWrite(output27,LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26state + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26state=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + output27state + "</p>");
            // If the output27State is off, it displays the ON button       
            if (output27state=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            client.println(""); // HTTP request ends with another blank line
            // Break out of the loop
            break; 
          }
          
          else // If we get a newline character, clear the currentLine variable
          {
            currentLine = "";
          }   
        }
        else if (c != '\r') // If c is anything but carriage return character.
        {
          currentLine += c ;// Add the character to the varible
        }
      }     
    }
     // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println(""); 
  }
}
