#include <esp_now.h>
#include <WiFi.h>

#define MESSAGE_LENGTH 50  // Maximum message length

// REPLACE WITH RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x7B, 0x29, 0x04};

// Structure to hold the message
typedef struct struct_message {
    char message[MESSAGE_LENGTH];  // Message buffer
} struct_message;

// Create a struct instance
struct_message myData;

// Callback function when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\nSend Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);  // Set ESP32 to station mode

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register send callback
    esp_now_register_send_cb(OnDataSent);

    // Register the receiver as a peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

void loop() {
    Serial.println("Type a message to send:");
    
    while (Serial.available() == 0) {
        // Wait for user input
    }

    String input = Serial.readStringUntil('\n');  // Read user input
    input.trim();  // Remove spaces or newlines

    // Copy message to struct (ensure it fits)
    strncpy(myData.message, input.c_str(), MESSAGE_LENGTH);
    myData.message[MESSAGE_LENGTH - 1] = '\0';  // Ensure null-termination

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
        Serial.println("Message sent successfully!");
    } else {
        Serial.println("Error sending message.");
    }

    delay(1000);  // Small delay before next input
}
