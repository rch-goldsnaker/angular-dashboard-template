#include <WiFi.h>
#include <HTTPClient.h>
#include <DHTesp.h>

#define pinDht 15
DHTesp dhtSensor;

// Wi-Fi network configuration
const char* ssid = "Wokwi-GUEST";         // Replace with your Wi-Fi network
const char* password = "";                // Replace with your Wi-Fi password

// TagoIO endpoint URL
const char* apiEndpoint = "https://api.tago.io/data";

// Device token (replace this with yours)
const char* deviceToken = "feeb1919-4f5a-4ab3-897b-e291e5b8ec89";

void setup() {
  Serial.begin(115200); // Initialize the serial monitor
  delay(1000);

  dhtSensor.setup(pinDht, DHTesp::DHT22);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connection established.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Initialize the connection to the endpoint
    http.begin(apiEndpoint);

    // Set headers
    http.addHeader("Content-Type", "application/json");
    http.addHeader("device-token", deviceToken);

    // Get sensor data
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    float temp = data.temperature;
    float hum = data.humidity;

    // Check if the values are valid
    if (isnan(temp) || isnan(hum)) {
      Serial.println("Error reading data from DHT22 sensor.");
      delay(5000); // Retry after 5 seconds
      return;
    }

    // Get Wi-Fi data
    String ipAddress = WiFi.localIP().toString();
    int rssi = WiFi.RSSI();
    int snr = rssi + 100; // Approximated SNR (typical noise level: -100 dBm)

    // Generate random battery level (0 to 100%)
    int batteryLevel = random(0, 101);

    // Create JSON dynamically
    String jsonPayload = "[";
    jsonPayload += "{\"variable\": \"temperature\", \"value\": " + String(temp, 1) + ", \"unit\": \"C\"},";
    jsonPayload += "{\"variable\": \"humidity\", \"value\": " + String(hum, 1) + ", \"unit\": \"%\"},";
    jsonPayload += "{\"variable\": \"ip_address\", \"value\": \"" + ipAddress + "\"},";
    jsonPayload += "{\"variable\": \"rssi\", \"value\": " + String(rssi) + ", \"unit\": \"dBm\"},";
    jsonPayload += "{\"variable\": \"snr\", \"value\": " + String(snr) + ", \"unit\": \"dB\"},";
    jsonPayload += "{\"variable\": \"battery_level\", \"value\": " + String(batteryLevel) + ", \"unit\": \"%\"},";
    jsonPayload += "{\"variable\": \"location\", \"value\": \"esp32\", \"location\": {\"lat\": -22.9068, \"lng\": -43.1729}}";
    jsonPayload += "]";

    // Display the generated JSON
    Serial.println("JSON Payload:");
    Serial.println(jsonPayload);

    // Perform the POST request
    int httpResponseCode = http.POST(jsonPayload);

    // Process the server response
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);

      String response = http.getString();
      Serial.println("Server Response:");
      Serial.println(response);
    } else {
      Serial.print("HTTP Request Error: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // End the connection
  } else {
    Serial.println("Wi-Fi not connected.");
  }

  delay(5000); // Wait 5 seconds before making another request
}
