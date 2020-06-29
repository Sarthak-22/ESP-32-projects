#include <ESP32_MailClient.h>

SMTPData smtpdata; // Create an object that acts as a placeholder for SMTPdata


// Insert your network credentials
const char* ssid = "__________";
const char* pass = "___________";

// Insert the mail credentials of sender and receiver
#define emailaccount_sender  "<Sender_email@gmail.com>"
#define emailaccount_pass  "<SenderEmailIDPassword>"
#define emailrecipient  "<Destination_email>@gmail.com"

//Insert your email provider smtp settings (Here we have used gmail account)
#define smtpServer "smtp.gmail.com" 
#define smtpServerPort 465

#define emailSubject "ESP32 test"

void sendCallback(SendStatus info); // Callback function to get the eamil sending status

 
void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(200);
   }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to WiFi successfully");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }

  // Set the connection between SMTP server and ESP32
  smtpdata.setLogin(smtpServer, smtpServerPort, emailaccount_sender, emailaccount_pass);
  //smtpdata.setSTARTTLS(true);

  smtpdata.setSender("ESP32",emailaccount_sender); // Sender name and email address

  smtpdata.setPriority("High");

  smtpdata.setSubject(emailSubject); // Set the subject of email

  smtpdata.setMessage("Hey there, this is a test email from esp32",false); // Set the bosy of the email

  smtpdata.addRecipient(emailrecipient); // Add the receiver email address

  smtpdata.setSendCallback(sendCallback);

  if (!MailClient.sendMail(smtpdata))
  {
    Serial.println("Error sending email, " +MailClient.smtpErrorReason()); // Display the error message if the process fails
  }

  smtpdata.empty();
  
}

void loop() {
 }

 void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("The process is successful");
  }
}
