/*
 * This ESP8266 NodeMCU code was developed by newbiely.com
 *
 * This ESP8266 NodeMCU code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/esp8266/esp8266-websocket
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "index.h"

const char* ssid = "iPhone de vultorio";     
const char* password = "0987654321";  

//pin sur l'entrée 1 du pont en H (recule moteur droit)
int IN1 = 13;

//pin sur l'entrée 2 du pont en H (avance moteur droit)
int IN2 = 12;

//pin sur l'entrée 3 du pont en H (avance moteur gauche)
int IN3 = 14;

//pin sur l'entrée 2 du pont en H (recule moteur gauche)
int IN4 = 15;

int sensorDroite = D1; // Broche du premier capteur de ligne situé à droite
int sensorGauche = D2; // Broche du deuxième capteur de ligne situé à gauche

boolean isActive = false;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

    //on initialise les entrée et sortie
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(sensorDroite, INPUT);
  pinMode(sensorGauche, INPUT);

  stop();
  
  // Connect to Wi-Fi
  connectToWiFi();

  // Define web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/start", HTTP_GET, handleStart);
  server.on("/stop", HTTP_GET, handleStop);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
  if(isActive)
  {
      //lecture des valeurs des deux capteurs de lignes
    int sensorValueDroite = digitalRead(sensorDroite);
    int sensorValueGauche = digitalRead(sensorGauche);

        // Reconnect to WiFi if connection is lost
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Connection lost. Reconnecting...");
      stop();
      connectToWiFi();
    }
    
    //en fonction des valeurs on avance ou tourne
    if (sensorValueDroite == HIGH && sensorValueGauche == HIGH) {
      // Avancer
      forward();
    } else if (sensorValueDroite == LOW && sensorValueGauche == HIGH) {
      // Tourner à right
      right();
    } else if (sensorValueDroite == HIGH && sensorValueGauche == LOW) {
      // Tourner à droite
      left();
    } else {
      // si on ne détecte rien on avance comme meme 
      forward();
    }
  }

  else {
      stop();
  }
}

void connectToWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void handleRoot() {
  String ipAddress = WiFi.localIP().toString();
  String html = "<html><head><style>";
  html += "button {";
  html += "  display: block;";
  html += "  margin: 0 auto;";
  html += "  padding: 20px 40px;";
  html += "  font-size: 24px;";
  html += "  cursor: pointer;";
  html += "  transition: background-color 0.3s ease;";
  html += "}";
  html += "</style></head><body>";
  html += "<h1 style='text-align:center;'>ESP8266 Web Server</h1>";
  html += "<p style='text-align:center;'>Adresse IP de l'ESP8266 : " + ipAddress + "</p>";
  html += "<button id='startBtn' onclick='handleStart()' onmousedown='buttonClick(this)'>Start</button>";
  html += "<button id='stopBtn' onclick='handleStop()' onmousedown='buttonClick(this)'>Stop</button>";
  html += "<script>";
  html += "function handleStart() {";
  html += "  var xhttp = new XMLHttpRequest();";
  html += "  xhttp.open('GET', '/start', true);";
  html += "  xhttp.send();";
  html += "}";
  html += "function handleStop() {";
  html += "  var xhttp = new XMLHttpRequest();";
  html += "  xhttp.open('GET', '/stop', true);";
  html += "  xhttp.send();";
  html += "}";
  html += "function buttonClick(btn) {";
  html += "  btn.style.backgroundColor = '#ccc';";
  html += "  setTimeout(function(){ btn.style.backgroundColor = ''; }, 100);";
  html += "}";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleStart() {
  isActive = true;
}

void handleStop() {
  isActive = false;
}

//fonction avance
void forward() {
  Serial.println("le robot avance");
  analogWrite(IN1, 0);
  analogWrite(IN2, 255);
  analogWrite(IN3, 255);
  analogWrite(IN4, 0);
}

//fonction tourne à gauche
void left() {
  Serial.println("le robot tourne à gauche");
    analogWrite(IN1, 0);
    analogWrite(IN2, 255);
    analogWrite(IN3, 0);
    analogWrite(IN4, 200);  
  
}

//fonction tourne à droite
void right() {
  Serial.println("le robot tourne à droite");
    analogWrite(IN1, 200);
    analogWrite(IN2, 0);
    analogWrite(IN3, 255);
    analogWrite(IN4, 0);
}

//fonction tourne à droite
void stop() {
    analogWrite(IN1, 0);
    analogWrite(IN2, 50);
    analogWrite(IN3, 50);
    analogWrite(IN4, 0);
}
