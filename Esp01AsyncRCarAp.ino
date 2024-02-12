#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "iqhtyar";
const char* password = "12345678";

const int motorPin1 = 0;
const int motorPin2 = 3;
const int motorPin3 = 2;
const int motorPin4 = 1;

AsyncWebServer server(80);

// Kode HTML untuk halaman web kendali motor
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Esp RcBotCar</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
    }
    .button {
      display: inline-block;
      margin: 13px;
      padding: 10px 20px;
      font-size: 15px;
      cursor: pointer;
      border-radius: 5px;
      background-color: #3498db;
      color: white;
      text-decoration: none;
    }
    .button:hover {
      background-color: #2980b9;
    }
  </style>
</head>
<body>
  <h1>Remote Control</h1>
  <div>
    <button class="button" onclick="controlMotor('forward')">MAJU</button>
  </div>
  <div>
    <button class="button" onclick="controlMotor('left')">KIRI</button>
    <button class="button" onclick="controlMotor('stop')">STOP</button>
    <button class="button" onclick="controlMotor('right')">KANAN</button>
  </div>
  <div>
    <button class="button" onclick="controlMotor('reverse')">MUNDUR</button>
  </div>
  <script>
    function controlMotor(command) {
      fetch('/motor', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: `command=${command}`
      })
      .then(response => {
        console.log(response);
      })
      .catch(error => {
        console.error('Error:', error);
      });
    }
  </script>
</body>
</html>
)rawliteral";


void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
//  Serial.print("Access Point IP address: ");
//  Serial.println(myIP);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/motor", HTTP_POST, [](AsyncWebServerRequest *request){
    String command = request->arg("command");
    
    if (command.equals("forward")) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
      delay(5);
    } else if (command.equals("reverse")) {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, HIGH);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
      delay(5);
    } else if (command.equals("left")) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, HIGH);
      digitalWrite(motorPin4, LOW);
      delay(5);
    } else if (command.equals("right")) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, HIGH);
      delay(5);
    } else if (command.equals("stop")) {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);
      delay(5);
    }

    request->send(200, "text/plain", "Motor diatur: " + command);
  });

  server.begin();
 // Serial.end();
}

void loop() {
}
