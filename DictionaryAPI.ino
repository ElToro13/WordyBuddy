#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "SSD1306.h"
SSD1306  display(0x3C, 4, 5);
MDNSResponder mdns;
ESP8266WebServer server(80);
int i=0,j=0,w=0;

void setup(void){
 // pinMode(0,INPUT);
  Serial.begin(115200);
  display.init();//Serial connection
 // display.flipScreenVertically();
 
  display.setColor(WHITE);
  display.clear();
  pinMode(2,INPUT);
//  digitalWrite(0,HIGH);
  WiFi.begin("Pompie", "YashSoni13495");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
   // String payload="";
 //StaticJsonBuffer<200> jsonBuffer;
  }
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   Serial.println("WiFi Connected....");
   for (int16_t i=0; i<DISPLAY_HEIGHT; i+=4) {
    display.drawLine(DISPLAY_WIDTH-1, 0, 0, i);
    display.display();
    delay(10);
  }
  for (int16_t i=0; i<DISPLAY_WIDTH; i+=4) {
    display.drawLine(DISPLAY_WIDTH-1, 0, i, DISPLAY_HEIGHT-1);
    display.display();
    delay(10);
  }
  delay(1000);
   display.clear();
   display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(32,0,"Welcome To ");
    display.display();
     display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
     display.drawString(15,25,"WordNik API ");
    display.display();
     String dd="Hits: ";
    String ggg = dd + String((int)w);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(80,45,ggg);
    display.display();
    
    
}
else{
  Serial.println("Wifi Not Connected!");
}
 if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  server.on("/Check", [](){
    server.send(200,"text/plain","OK");
  });
 server.on("/", [](){
 display.clear();
HTTPClient http;    //Declare object of class HTTPClient

   DynamicJsonBuffer jsonBuffer;
   http.begin("http://api.thingspeak.com/channels/283208/fields/1.json?api_key=AMW91NSXQBNFKAEU&results=1");
 //  http.begin("http://api.railwayapi.com/arrivals/station/BRC/hours/2/apikey/aa6bms28/");
   http.addHeader("Content-Type", "application/json");  //Specify content-type header

   int httpCode = http.GET(); //Send the request
   String payload = http.getString();   
   Serial.println(httpCode);   //Print HTTP return code 200 correct data 401 invalid data  404 Error
   Serial.println(payload);    //Print request response payload
   JsonObject& root = jsonBuffer.parseObject(payload);
   
  if(!root.success()){
    Serial.println("Nope");
  }
  
  else{
   
   JsonArray& trains = root["feeds"];
    JsonObject& train = trains[0];
    String pos = train["field1"];    
    Serial.println(pos);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    Serial.println(pos);
  
 // http.end();
    String Count= "> ";
    String FIN=Count+pos;
    display.drawString(0,0,FIN);
    display.display();
    String one = "http://api.wordnik.com:80/v4/word.json/";
    String two="/definitions?limit=200&includeRelated=true&sourceDictionaries=wiktionary&useCanonical=true&includeTags=false&api_key=a2a73e7b926c924fad7001ca3111acd55af2ffabf50eb4ae5";
    String deff=one+pos+two;
    http.begin(deff);
    int httpCode2 = http.GET(); //Send the request
    String payload2 = http.getString();
    Serial.println(httpCode2);   //Print HTTP return code 200 correct data 401 invalid data  404 Error
    Serial.println(payload2); 
    JsonArray& root2 = jsonBuffer.parseArray(payload2);
    
    if(!root.success()){
    Serial.println("Nope");
  }
  else{
    JsonObject& train2 = root2[0];
    String pos2 = train2["text"];
    String PPP = train2["partOfSpeech"];
     display.setTextAlignment(TEXT_ALIGN_LEFT);
     display.setFont(ArialMT_Plain_16);
      String uuu1="/* ";
    String uuu2=" */";
    String uuu3=uuu1+PPP+uuu2;
     display.drawString(0,15,uuu3);
      display.display(); 
      delay(1500);
      display.clear();
       display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_10);
       display.drawString(0,15,pos2);
       display.display();
       delay(1000);
       display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
      for(i=0;i>=-400;i--){
      
       display.drawString(i,15,pos2);
       display.display();
       delay(10);
       display.clear();
       
      }
  /*  char diss[400];
    pos2.toCharArray(diss,400);
    Serial.println(pos2.length());
    for(int y=0;y<=127;y++){
      display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    if(i>127){
      display.drawString(j,26,String((char)diss[y]));
      display.display();
      j=j+6;
    }
   else{
       display.drawString(i,15,String((char)diss[y]));
       display.display();
    }
   i=i+5;*/
    
    Serial.print("i is: ");
    Serial.println(i);
    Serial.println(pos2);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(32,0,"Welcome To ");
    display.display();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
     display.drawString(15,25,"WordNik API ");
    display.display();
    w++;
    String dd="Hits: ";
    String ggg = dd + String((int)w);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(80,45,ggg);
    display.display();
    }
}
http.end();
        server.send(200,"text/plain","OK"); 
  });
  server.begin();
}
void loop(){
  server.handleClient();
  if(digitalRead(2)==LOW){
     display.clear();
 HTTPClient http;
   DynamicJsonBuffer jsonBuffer;
   
   http.begin("http://api.wordnik.com:80/v4/words.json/randomWords?hasDictionaryDef=true&minCorpusCount=0&maxCorpusCount=-1&minDictionaryCount=1&maxDictionaryCount=-1&minLength=5&maxLength=-1&limit=10&api_key=a2a73e7b926c924fad7001ca3111acd55af2ffabf50eb4ae5");
   http.addHeader("Content-Type", "application/json");  //Specify content-type header

   int httpCode = http.GET(); //Send the request
   String payload = http.getString();   
  // char payload2[200] = http.getString();//Get the response payload
  //String payload =  "{\"card\":\"123456789012\"}";
   
   Serial.println(httpCode);   //Print HTTP return code 200 correct data 401 invalid data  404 Error
   Serial.println(payload);    //Print request response payload
   JsonArray& root = jsonBuffer.parseArray(payload);
   
  if(!root.success()){
    Serial.println("Nope");
  }
  else{
   // Serial.println("It worked!");
  
    JsonObject& train = root[0];
    String pos = train["word"];
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    Serial.println(pos);
  
 // http.end();
    String Count= "> ";
    String FIN=Count+pos;
    display.drawString(0,0,FIN);
    display.display();
    String one = "http://api.wordnik.com:80/v4/word.json/";
    String two="/definitions?limit=200&includeRelated=true&sourceDictionaries=wiktionary&useCanonical=true&includeTags=false&api_key=a2a73e7b926c924fad7001ca3111acd55af2ffabf50eb4ae5";
    String deff=one+pos+two;
    http.begin(deff);
    int httpCode2 = http.GET(); //Send the request
    String payload2 = http.getString();
    Serial.println(httpCode2);   //Print HTTP return code 200 correct data 401 invalid data  404 Error
    Serial.println(payload2); 
    JsonArray& root2 = jsonBuffer.parseArray(payload2);
    
    if(!root.success()){
    Serial.println("Nope");
  }
  else{
    JsonObject& train2 = root2[0];
    String pos2 = train2["text"];
    String PPP = train2["partOfSpeech"];
     display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    String uuu1="/* ";
    String uuu2=" */";
    String uuu3=uuu1+PPP+uuu2;
     display.drawString(0,15,uuu3);
      display.display(); 
      delay(1500);
      display.clear();
   
       display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_10);
       display.drawString(0,15,pos2);
       display.display();
       delay(1000);
      display.setFont(ArialMT_Plain_10);
        for(i=0;i>=-400;i--){
      
       display.drawString(i,15,pos2);
       display.display();
       delay(10);
       display.clear();
       
      }
      
   // delay(1000);
    Serial.print("i is: ");
    Serial.println(i);
    Serial.println(pos2);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(32,0,"Welcome To ");
    display.display();
     display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
     display.drawString(15,25,"WordNik API ");
    display.display();
    w++;
    String dd="Hits: ";
    String ggg = dd + String((int)w);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(80,45,ggg);
    display.display();
    }
}
http.end();
  }
 }


