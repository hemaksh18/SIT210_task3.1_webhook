#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHT_PIN 2       
#define DHT_TYPE DHT22

char ssid[] = "POCO PHONE";
char pass[] = "dudeperfect";

unsigned long ID = 2249912;
const char *API_Key = "RJKPE27QCV40MNZM";

DHT dht_sensor(DHT_PIN, DHT_TYPE);
WiFiClient client;

void setup() {
  Serial.begin(9600);


  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(1000);
  }

  Serial.println("Connected ");


  ThingSpeak.begin(client);

  dht_sensor.begin();
}

void loop() {

  float temperature = dht_sensor.readTemperature();
  float humidity = dht_sensor.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read data");
    delay(2000);
    return;
  }


  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int response = ThingSpeak.writeFields(ID, API_Key);
  if (response == 200) {
    Serial.println("update successful");
  } else {
    Serial.print("Error updating channel: ");
    Serial.println(response);
  }

  delay(20000);  
}
