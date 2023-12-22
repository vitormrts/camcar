#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <iostream>
#include <sstream>

// Define constants
#define BOTTOM_SERVO_PIN 18
#define TOP_SERVO_PIN 19
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define STOP 0
#define RIGHT_MOTOR 0
#define LEFT_MOTOR 1
#define FORWARD 1
#define BACKWARD -1

// CAM (access point) credentials
const char* ssid = "LilyGo-CAM-C8:2B";
const char* password = "";

Servo bottomServo;
Servo topServo;

struct MOTOR_PINS
{
  int pinEn;  
  int pinIN1;
  int pinIN2;    
};

std::vector<MOTOR_PINS> motorPins = 
{
  {14, 26, 27}, // Left motor (EnA, IN1, IN2)
  {15, 22, 23},  // Right motor (EnB, IN3, IN4)
};

// PWM for velocity motor control
const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int PWMSpeedChannel = 2;

// Web server and web socket setup
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// HTML Page
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
            <button onmousedown='sendData("MoveCar","1")' onmouseup='sendData("MoveCar","0")' class="cross-top"></button>
            <button onmousedown='sendData("MoveCar","2")' onmouseup='sendData("MoveCar","0")' class="cross-bottom"></button>
            <button onmousedown='sendData("MoveCar","3")' onmouseup='sendData("MoveCar","0")' class="cross-left"></button>
            <button onmousedown='sendData("MoveCar","4")' onmouseup='sendData("MoveCar","0")' class="cross-right"></button>
            <button onclick='sendData("MoveCar","0")' class="cross-circle"></button>
          </div>
        </div>
        <div class="sliders">
          <p>Servo Superior</p>
          <div class="slide-container">
            <input type="range" min="0" max="180" value="90" class="slider" id="Top" oninput='sendData("Top",value)'>
          </div>
    
          <p>Servo Inferior</p>
          <div class="slide-container">
            <input type="range" min="0" max="180" value="90" class="slider" id="Bottom" oninput='sendData("Bottom",value)'>
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

void rotateMotor(int motorNumber, int motorDirection)
{
  if (motorDirection == FORWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);    
  }
  else if (motorDirection == BACKWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);     
  }
  else
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);       
  }
}

void moveCar(int inputValue)
{
  ledcWrite(PWMSpeedChannel, 255);
  switch(inputValue)
  {
    case UP:
      rotateMotor(RIGHT_MOTOR, FORWARD);
      rotateMotor(LEFT_MOTOR, FORWARD);                  
      break;
  
    case DOWN:
      rotateMotor(RIGHT_MOTOR, BACKWARD);
      rotateMotor(LEFT_MOTOR, BACKWARD);  
      break;
  
    case LEFT:
      rotateMotor(RIGHT_MOTOR, BACKWARD);
      rotateMotor(LEFT_MOTOR, FORWARD);  
      break;
  
    case RIGHT:
      rotateMotor(RIGHT_MOTOR, FORWARD);
      rotateMotor(LEFT_MOTOR, BACKWARD); 
      break;
 
    case STOP:
      rotateMotor(RIGHT_MOTOR, STOP);
      rotateMotor(LEFT_MOTOR, STOP);
      break;
  
    default:
      rotateMotor(RIGHT_MOTOR, STOP);
      rotateMotor(LEFT_MOTOR, STOP);    
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
  Serial.println("Setup pins...");

  // Setup servo's pins
  bottomServo.attach(BOTTOM_SERVO_PIN);
  topServo.attach(TOP_SERVO_PIN);

  ledcSetup(PWMSpeedChannel, PWMFreq, PWMResolution);
      
  for (int i = 0; i < motorPins.size(); i++)
  {
    pinMode(motorPins[i].pinEn, OUTPUT);
    Serial.printf("Setting pin %d as output\n", motorPins[i].pinEn);
    pinMode(motorPins[i].pinIN1, OUTPUT);
    Serial.printf("Setting pin %d as output\n", motorPins[i].pinIN1);
    pinMode(motorPins[i].pinIN2, OUTPUT);
    Serial.printf("Setting pin %d as output\n", motorPins[i].pinIN2);
   
    Serial.printf("Attaching pin %d to channel %d\n", motorPins[i].pinEn, PWMSpeedChannel);
    ledcAttachPin(motorPins[i].pinEn, PWMSpeedChannel);
  }

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

  // Create HTTP GET HTML index page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();
}
