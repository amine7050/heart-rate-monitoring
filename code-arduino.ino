#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#define BLYNK_PRINT Serial
  

#include <BlynkSimpleEsp32.h>
int Led_OnBoard = 34;                  // Initialize the Led_OnBoard 
char auth[] = "fb8O7A4UNNZmNGsx9P10Y7cZdhAJZLB8";//the token in the blynk app for the security
const char* ssid = "wifi name";                  // Your wifi Name       
const char* password = "password";          // Your wifi Password
WidgetLCD lcd(V5);
WidgetLCD lcd2(V3);
const char *host = "Your host"; //Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.

void setup() {
  // put your setup code here, to run once:
  Blynk.begin(auth, ssid, password);
  delay(1000);
  pinMode(Led_OnBoard, OUTPUT);       // Initialize the Led_OnBoard pin as an output
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
lcd.clear();
lcd.print(0, 0, "heart beat");
lcd2.clear();
lcd2.print(0, 0, "votre statut");
}
   Serial.print("Connecting");
   Wait for connection
   while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(Led_OnBoard, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(Led_OnBoard, HIGH);
    delay(250);
   }

  digitalWrite(Led_OnBoard, HIGH);
  //if connection successful show IP address in serial monitor
  

void loop() {
  // put your main code here, to run repeatedly:
  

  HTTPClient http;    //Declare object of class HTTPClient
 
  String LdrValueSend, postData;
  int ldrvalue=(analogRead(33)/28);  //Read Analog value of LDR
  LdrValueSend = String(ldrvalue);   //String to interger conversion
 //Serial.println(ldrvalue);
  Blynk.virtualWrite(V4,ldrvalue );
  lcd.clear();
lcd.print(0, 0, "heart beat");
lcd.print(7, 1, ldrvalue);

if (ldrvalue>100){
  lcd2.clear();
lcd2.print(0, 0, "statut");
lcd2.print(0, 1, "danger");
}
  Blynk.run();
  delay(100);
  //Post Data
  postData = "ldrvalue=" + LdrValueSend;
  
  http.begin("http://your host/name of database/InsertDB.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println("plm Value=" + ldrvalue);
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  Serial.println("plm Value=" + LdrValueSend);
  
  http.end();  //Close connection

  delay(60000);  //Here there is 4 seconds delay plus 1 second delay below, so Post Data at every 5 seconds
  digitalWrite(Led_OnBoard, LOW);
  delay(1000);
  digitalWrite(Led_OnBoard, HIGH);
  delay(1000);
}
