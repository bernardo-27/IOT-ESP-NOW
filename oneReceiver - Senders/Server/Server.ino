#include <esp_now.h>
#include <WiFi.h>

#define MESSAGE_LENGTH 50  // Maximum message length

// Structure to receive the message
typedef struct struct_message {
    char message[MESSAGE_LENGTH];
} struct_message;

// Create a struct instance
struct_message receivedData;

// Callback function when data is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    Serial.print("Received message: ");
    Serial.println(receivedData.message);
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);  // Set ESP32 to station mode

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the receive callback
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    // Nothing to do here, ESP-NOW runs in the background
}
