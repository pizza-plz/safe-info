#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid = "ATTqMhTf5S";
const char* password = "password";

#define DHTPIN D1

//sensor type (dht-22)
#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);

//current temp and humidity, updated in loop()
float t = 0.0;
float h = 0.0;

//Create asyncwebserver object on port 80
AsyncWebServer server(801);

//Generally you should use unsigned long for variables that hold time
//The value will become to large for an int to store
unsigned long previousMillis = 0;  //stores last time DHT was updated

//Updates DHT readings every 10 seconds
const long interval = 10000;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <link rel="stylesheet" href="https://gitcdn.xyz/repo/pizza-plz/safe-info/master/style.css">
</head>

<body class="gradient">
  <div class="container">
    <div class+"row">
        <h2 id="greeting">Good Morning, Pizza</h2>
        <div class="cluster">
          <div class="gauge gauge-temperature">
            <progress max="120" value="%TEMPERATURE%" id="prog-temperature"></progress>
            <i class="fas fa-thermometer-half"></i>
            <span id="temperature">%TEMPERATURE%</span>
            <sup class="units">&deg;F</sup>
            <span class="dht-labels">Temperature</span>
          </div>
          <div class="gauge gauge-humidity">
            <progress max="120" value="%HUMIDITY%" id="prog-humidity"></progress>
            <i class="fas fa-tint"></i>
            <span id="humidity">%HUMIDITY%</span>
            <sup class="units">%</sup>
            <span class="dht-labels">Humidity</span>
          </div>
        </div>
    </div>
    <div class="row">
        <div class="weather">
            <div class="weather-item">
                <div class="day" id="weather-day1">Today</div>
                <div class="temp">
                    <div class="lows">
                        <i class="icon fa fa-arrow-down"></i>
                        <span class="temp-value" id="day1-temp-low">999</span>
                        <span class="temp-units">F</span>
                    </div>
                    <div class="highs">
                        <i class="icon fa fa-arrow-up"></i>
                        <span class="temp-value" id="day1-temp-high">999</span>
                        <span class="temp-units">F</span>
                    </div>
                </div>
            </div>
            <div class="weather-item">
                <div class="day" id="weather-day2">Tomorrow</div>
                <div class="temp">
                    <div class="lows">
                        <i class="icon fa fa-arrow-down"></i>
                        <span class="temp-value" id="day2-temp-low">999</span>
                        <span class="temp-units">F</span>
                    </div>
                    <div class="highs">
                        <i class="icon fa fa-arrow-up"></i>
                        <span class="temp-value" id="day2-temp-high">999</span>
                        <span class="temp-units">F</span>
                    </div>
                </div>
            </div>
            <div class="weather-item">
                <div class="day" id="weather-day3">Day After Tomorrow</div>
                <div class="temp">
                    <div class="lows">
                        <i class="icon fa fa-arrow-down"></i>
                        <span class="temp-value" id="day3-temp-low">999</span>
                        <span class="temp-units">F</span>
                    </div>
                    <div class="highs">
                        <i class="icon fa fa-arrow-up"></i>
                        <span class="temp-value" id="day3-temp-high">999</span>
                        <span class="temp-units">F</span>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="car-container">
            <div class="car">
                <img src="car-yellow.png" />
            </div>
            <div class="car">
                <img src="car-green.png" />
            </div>
            <div class="car">
                <img src="car-red.png" />
            </div>
        </div>
    </div>
  </div>
</body>
<script src="script.js"></script>
</html>
)rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var){
//Serial.println(var);
if(var == "TEMPERATURE"){
return String(t);
}
else if(var == "HUMIDITY"){
return String(h);
}
return String();
}
void setup(){
// Serial port for debugging purposes
Serial.begin(115200);
dht.begin();
// Connect to Wi-Fi
WiFi.begin(ssid, password);
Serial.println("Connecting to WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println(".");
}
// Print ESP8266 Local IP Address
Serial.println(WiFi.localIP());
// Route for root / web page
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/html", index_html, processor);
});
server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/plain", String(t).c_str());
});
server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/plain", String(h).c_str());
});
// Start server
server.begin();
}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    //save the last tim eyou updated DHT
    previousMillis = currentMillis;
    // (true) means farenheit
    float newT = dht.readTemperature(true);
    if (isnan(newT)){
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT;
      Serial.println(t);
    }

    //Humidity
    float newH = dht.readHumidity();
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else{
      h = newH;
      Serial.println(h);
    }
   }
 }
