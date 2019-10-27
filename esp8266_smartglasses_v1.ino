// https://electronics.stackexchange.com/questions/273768/flip-the-image-of-oled-128x64-0-96-inch-display

#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// WiFi settings
const char* ssid = "__SG";
const char* password = "Haslo43ASu";

// Time to deep sleep (seconds):
const int sleepTimeS = 5;

// Host
const char* url = "http://192.168.43.1:8080";
String serverResponse="";

// OLED Display
int currentLine = 0;
void oledPrintln(String text) {
  if (currentLine >= 4) { display.clearDisplay(); currentLine = 0; }
  display.setCursor(40, currentLine * 8);
  display.print(text);
  display.display();
  currentLine++;
}


// Setup
void setup() {
  // --- Serial Init ---
  Serial.begin(115200);
  Serial.println("[I] ESP Init...");

  // --- Display Init ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("[E] Dispaly init failed!"));
    for(;;);
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("[I] Conecting ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("[I] Connected to WiFi!");

  Serial.print("[I] IP: ");
  Serial.println(WiFi.localIP());

  // Display Settings
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.display();

  // Connect
  Serial.print("[I] Connecting to ");
  Serial.println(url);
  
  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();
  String response = http.getString();

  Serial.print("[I] [HttpClient] HttpCode: ");
  Serial.println(httpCode);
  Serial.println("[I] [HttpClient] HttpResponse: ");
  Serial.println(response);

  http.end();

  // Oled Print
  oledPrintln(response);
  
  // Sleep
  Serial.print("[W] ESP8266 -> sleep mode");
  ESP.deepSleep(sleepTimeS * 1000000);

}

void loop() {

    
}
