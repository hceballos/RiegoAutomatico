#include "Arduino.h"
#include <EMailSender.h>
#include <ESP8266WiFi.h>
const int pin = 13;


#define SECOND 1000L
#define MINUTE (60*SECOND)
#define HOUR (60*MINUTE)


    const char* ssid = "ITA-2";
    const char* password = "01-17-AZ-!";

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

EMailSender emailSend("c0x08266@gmail.com", "angamos2270");

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);
        Serial.println(nSSID);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}


void loop1()
{
  Serial.println("MENSAJE DE PRUEBAAA ");
} 


void setup()
{
    Serial.begin(115200);
    pinMode(pin, OUTPUT);  //definir pin como salida
    int lectura = analogRead(A0);
    //int lecturaPorcentajeCorreo = map(lectura, 1023, 0, 0, 100);
    int lecturaPorcentajeCorreo = map(lectura, 700, 100, 0, 100);
    String myString = String(lecturaPorcentajeCorreo);
    
    connection_state = WiFiConnect(ssid, password);
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    EMailSender::EMailMessage message;
    message.subject = "Albahaca 2: " + myString + "% humedad";
    message.message = "La humedad de la tierra de la Albahaca es : " + myString;

    EMailSender::Response resp = emailSend.send("ceballosaviles@gmail.com", message);

    Serial.println("Sending status: ");
    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
    loop1();
}



void loop() {
  delay(1000);
  int lectura = analogRead(A0);
  Serial.print(lectura);
  
  int lecturaPorcentaje = map(lectura, 700, 100, 0, 100);
  
  if (lecturaPorcentaje <=60){
    Serial.print("La humedad es del: ");
    Serial.print(lecturaPorcentaje);
    Serial.println("%");
    digitalWrite(13, HIGH);
    delay(3000);
    digitalWrite(13, LOW); 
    delay(1000);
    setup();
    delay (1*HOUR) ;
  }
  else{
    digitalWrite(13, LOW);
    Serial.print("La humedad es del: ");
    Serial.print(lecturaPorcentaje);
    Serial.println("%");
  }
}
