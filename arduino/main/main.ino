// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <iostream>
#include <sstream>

/*Arrows*/
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

#define FORWARD 1
#define BACKWARD -1

/*Right Motor*/
#define RIGHT_MOTOR_FORWARD  26  //pin1
#define RIGHT_MOTOR_BACKWARD 27 //pin2
#define RIGHT_MOTOR_EN 14

/*Left Motor*/
#define LETF_MOTOR_FORWARD 1 //pin3
#define LEFT_MOTOR_BACKWARD 3  //pin4
#define LEFT_MOTOR_EN 2

/*Servos*/
#define BOTTOM_SERVO_PIN 18
#define TOP_SERVO_PIN 19

//#define LIGHT_PIN 4


Servo bottomServo;
Servo topServo;

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int PWMSpeedChannel = 2;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"HTMLHOMEPAGE(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
    <style>
      body {
        user-select: none;
        background-color: #fcfcfc;
        font-family: sans-serif;
      }

      p {
        margin: 0 0 8px;
      }

      img {
        display: block;
        margin: 0 auto 32px;
        height: auto;
        width: 450px;
      }

      .container {
        width: 100%;
        max-width: 800px;
        margin: 0 auto;
      }

      .controls {
        display: flex;
        align-items: center;
        justify-content: space-between;
      }

      button {
        background: transparent;
        border: none;
      }

      .sliders {
        width: 50%;
      }

      .slide-container {
        width: 100%;
      }

      .slider {
        width: 100%;
        height: 15px;
        border-radius: 5px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }

      .slider:hover {
        opacity: 1;
      }

      .cross-control {
        margin-left: 55px;
      }

      .cross-center{
        background-color: #333333;
        width: 55px;
        height: 55px;
        position: relative;
      }

      .cross-circle{
        background-color: #292929;
        width: 45px;
        height: 45px;
        position: absolute;
        border-radius: 100%;
        margin-top: 5px;
        margin-left: 5px;
      }

      .cross-top{
        background-color: #333333;
        width: 55px;
        height: 55px;
        position: absolute;
        border-radius: 15%;
        margin-top: -40px;
      }

      .cross-bottom{
        background-color: #333333;
        width: 55px;
        height: 55px;
        position: absolute;
        border-radius: 15%;
        margin-top: 40px;
      }

      .cross-left{
        background-color: #333333;
        width: 55px;
        height: 55px;
        position: absolute;
        border-radius: 15%;
        margin-left: -40px;
      }

      .cross-right{
        background-color: #333333;
        width: 55px;
        height: 55px;
        position: absolute;
        border-radius: 15%;
        margin-left: 40px;
      }
    </style>
  </head>
  <body>
    <div class ="container">
      <img src="http://192.168.4.1:81/stream"></td>
      <div class="controls">
        <div class="cross-control">
          <div class="cross-center">
            <button onclick='sendData("MoveCar","1")' class="cross-top"></button>
            <button onclick='sendData("MoveCar","2")' class="cross-bottom"></button>
            <button onclick='sendData("MoveCar","3")' class="cross-left"></button>
            <button onclick='sendData("MoveCar","4")' class="cross-right"></button>
            <button class="cross-circle"></button>
          </div>
        </div>
        <div class="sliders">
          <p>Velocidade</p>
          <div class="slide-container">
              <input type="range" min="0" max="255" value="150" class="slider" id="Speed" oninput='sendData("Speed",value)'>
          </div>
    
          <p>Servo Inferior</p>
          <div class="slide-container">
            <input type="range" min="0" max="180" value="90" class="slider" id="Bottom" oninput='sendData("Bottom",value)'>
          </div>
    
          <p>Servo Superior</p>
          <div class="slide-container">
            <input type="range" min="0" max="180" value="90" class="slider" id="Top" oninput='sendData("Top",value)'>
          </div>
        </div>
      </div>
    </div>
    <script>
      var websocket;

	    window.addEventListener('load', function() {
        websocket = new WebSocket(`ws://${window.location.hostname}/ws`);

        websocket.onopen = function(event) {
          console.log('Connection established');
        }

        websocket.onclose = function(event) {
          console.log('Connection died');
        }

        websocket.onerror = function(error) {
          console.log('error');
        };

        websocket.onmessage = function(event) {};
      });

      function sendData(key, value) {
        var data = key + "," + value;
        websocket.send(data);
        console.log(data);
      }
	  </script>
  </body>    
</html>
)HTMLHOMEPAGE";

void moveCar(int inputValue)
{
  Serial.printf("Got value as %d\n", inputValue);  
  switch(inputValue)
  {

    case UP:
      digitalWrite(RIGHT_MOTOR_EN, HIGH);
      digitalWrite(LEFT_MOTOR_EN, HIGH);
      digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
      digitalWrite(LETF_MOTOR_FORWARD, HIGH);
      digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
      Serial.println("Forward");
      break;
  
    case DOWN:
      digitalWrite(RIGHT_MOTOR_EN, HIGH);
      digitalWrite(LEFT_MOTOR_EN, HIGH);
      digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
      digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);  
      digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
      digitalWrite(LETF_MOTOR_FORWARD, LOW);
      Serial.println("Backward");
      break;
  
    case LEFT:
      digitalWrite(RIGHT_MOTOR_EN, HIGH);
      digitalWrite(LEFT_MOTOR_EN, HIGH);
      digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
      digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
      digitalWrite(LETF_MOTOR_FORWARD, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
      Serial.println("Left");  
      break;
  
    case RIGHT:
      digitalWrite(RIGHT_MOTOR_EN, HIGH);
      digitalWrite(LEFT_MOTOR_EN, HIGH);
      digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
      digitalWrite(LETF_MOTOR_FORWARD, HIGH);
      digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
      Serial.println("Right");
      break;
 
    case STOP:
      digitalWrite(RIGHT_MOTOR_EN, LOW);
      digitalWrite(LEFT_MOTOR_EN, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
      digitalWrite(LETF_MOTOR_FORWARD, LOW);
      digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
      Serial.println("Stop");    
      break;
  
    default:
      digitalWrite(RIGHT_MOTOR_EN, LOW);
      digitalWrite(LEFT_MOTOR_EN, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
      digitalWrite(LETF_MOTOR_FORWARD, LOW);
      digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD, LOW);   
      break;
  }
}

void webSocketEventHandler(AsyncWebSocket *server, 
                      AsyncWebSocketClient *client, 
                      AwsEventType type,
                      void *arg, 
                      uint8_t *data, 
                      size_t len) 
{                      
  switch (type) 
  {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());

      moveCar(0);
      bottomServo.write(90);
      topServo.write(90);       
      break;
    case WS_EVT_DATA:
      AwsFrameInfo *info;
      info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
      {
        std::string myData = "";
        myData.assign((char *)data, len);
        std::istringstream ss(myData);
        std::string key, value;
        std::getline(ss, key, ',');
        std::getline(ss, value, ',');
        Serial.printf("Key [%s] Value[%s]\n", key.c_str(), value.c_str()); 
        int valueInt = atoi(value.c_str());     
        if (key == "MoveCar")
        {
          moveCar(valueInt);        
        }
        else if (key == "Speed")
        {
          ledcWrite(PWMSpeedChannel, valueInt);
        }
        else if (key == "Bottom")
        {
          bottomServo.write(valueInt);
        }
        else if (key == "Top")
        {
          topServo.write(valueInt);   
        }             
      }
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    default:
      break;  
  }
}

void setupPins()
{
  Serial.println("Setup pins");
  delay(1000);
  bottomServo.attach(BOTTOM_SERVO_PIN);
  topServo.attach(TOP_SERVO_PIN);

  //Set up PWM
  ledcSetup(PWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(PWMLightChannel, PWMFreq, PWMResolution);


  /*Setup right motor (1)*/
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_EN, OUTPUT);

  /*Setup left motor (2)*/
  pinMode(LETF_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_EN, OUTPUT);

  delay(1000);

  Serial.println("Done pins!");

  moveCar(STOP);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  setupPins();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  ws.onEvent(webSocketEventHandler);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();
}
