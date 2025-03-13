#include <esp_now.h>
#include <WiFi.h>

// Simplified ESP-NOW code based on RandomNerdTutorials

uint8_t peer1[] = {0xF8, 0xB3, 0xB7, 0x32, 0xCA, 0x24}; // GETMACADD
uint8_t peer2[] = {0xAC, 0x15, 0x18, 0xE9, 0xA6, 0x1C}; // RECEIVER

struct Message {
  char text[32];
};

Message myMessage;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sent Successfully" : "Send Failed");
}

void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
  memcpy(&myMessage, data, sizeof(myMessage));
  Serial.print("Received: ");
  Serial.println(myMessage.text);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peer1, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  memcpy(peerInfo.peer_addr, peer2, 6);
  esp_now_add_peer(&peerInfo);
}

void loop() {
  Serial.println("Type a message: ");
  while (!Serial.available());

  String input = Serial.readStringUntil('\n');
  input.trim();
  strncpy(myMessage.text, input.c_str(), sizeof(myMessage.text));

  esp_now_send(peer1, (uint8_t *)&myMessage, sizeof(myMessage));
  esp_now_send(peer2, (uint8_t *)&myMessage, sizeof(myMessage));

  delay(1000);
}
