#include <ESP8266WiFi.h>
 
/* define port */
WiFiClient client;
WiFiServer server(80);

 //String agAdi = "Preo Drones-Anka-e12a04"; 
/* WIFI settings */
const char* ssid = "Wi-Fi Name";
const char* password = "**********";
 
/* data received from application */
String data ="";
 
/* define L298N or L293D motor control pins */
int leftMotorForward = 2; /* GPIO2(D4) -> IN3 */
int rightMotorForward = 15; /* GPIO15(D8) -> IN1 */
int leftMotorBackward = 0; /* GPIO0(D3) -> IN4 */
int rightMotorBackward = 13; /* GPIO13(D7) -> IN2 */
 
void setup() {
Serial.begin(115200);
delay(10);
 
/* initialize motor control pins as output */
pinMode(leftMotorForward, OUTPUT);
pinMode(rightMotorForward, OUTPUT);
pinMode(leftMotorBackward, OUTPUT);
pinMode(rightMotorBackward, OUTPUT);
 
/* Connect to WiFi network */
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
/* start server communication */
server.begin();
Serial.println("Server started");
 
/* print the IP address */
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
 
}
 
void loop() {
 
/* If the server available, run the "checkClient" function */
client = server.available();
if (!client) return;
data = checkClient ();
 
/************************ Run function according to incoming data from application *************************/
 
/* If the incoming data is "forward", run the "MotorForward" function */
if (data == "forward") MotorForward();
/* If the incoming data is "backward", run the "MotorBackward" function */
else if (data == "stop") MotorStop();
 
/* Web Browser */
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("");
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<head>");
client.println("<title> MOTOR CONTROL </title>");
client.println("</head>");
client.println("<body>");
client.println("<center>");
client.println("<h2> WiFi CONTROLLED TANK via ESP8266 </h2>");
client.println("<br><br>");
client.println("<hr>");
 
/* Buttons */
client.println("<br><br>");
client.println("<a href=\"/forward\"\"><button>FORWARD </button></a>");
client.println("<br><br>");
client.println("<a href=\"/stop\"\"><button>STOP </button></a><br />");
 
client.println("<br><br>");
client.println("<hr>");
client.println("<br><br>");
client.println("<pre><h3>Motor Control By Nodemcu For Detected Wild Plants</br> WiFi controlled Tank via Web Browser (WiFibot/Web/WiFi/IoT/ESP8266)</br> NodeMCU ESP8266 Project</br></h3></pre>");
client.println("</center>");
client.println("</body>");
client.println("</html>");
}
 
 
/********************************************* FORWARD *****************************************************/
void MotorForward(void)
{
  Serial.println("İleri");
digitalWrite(leftMotorForward,HIGH);
digitalWrite(leftMotorBackward,LOW);
}
 

/********************************************* STOP *****************************************************/
void MotorStop(void)
{
digitalWrite(leftMotorForward,LOW);
digitalWrite(leftMotorBackward,LOW);
digitalWrite(rightMotorForward,LOW);
digitalWrite(rightMotorBackward,LOW);
}
 
/********************************** RECEIVE DATA FROM the WEB ******************************************/
String checkClient (void)
{
while(!client.available()) delay(1);
String request = client.readStringUntil('\r');
request.remove(0, 5);
request.remove(request.length()-9,9);
return request;
}
