#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

#define DHTPIN D2
#define DHTTYPE DHT11

#define LED_RED D8
#define LED_YELLOW D7
#define LED_GREEN D6
#define BUZZER_PIN D0

#define WIFI_SSID "son"
#define WIFI_PASSWORD "GratisBanget@"

#define THINGSPEAK_CHANNEL_ID 2479774
#define THINGSPEAK_API_KEY "XA4LTFOGMWM9BGL0"

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200);
  delay(10);

  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();
  float celcius = temperature;
  float fahrenheit = (temperature * 9) / 5 + 32;
  float reamur = temperature * 0.8;
  float kelvin = temperature + 273.15;

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.println(celcius);
  Serial.println(fahrenheit);
  Serial.println(reamur);
  Serial.println(kelvin);

  ThingSpeak.setField(1, celcius);
  ThingSpeak.setField(2, fahrenheit);
  ThingSpeak.setField(3, reamur);
  ThingSpeak.setField(4, kelvin);
  ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);


  if (temperature < 30) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, HIGH);
    noTone(BUZZER_PIN);
  } else if (temperature >= 30 && temperature <= 36) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, LOW);
    noTone(BUZZER_PIN);
  } else {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    tone(BUZZER_PIN, 500);
  }

  delay(5000);
}
