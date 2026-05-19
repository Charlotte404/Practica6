#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <time.h>

#define PIN_MOSI 11
#define PIN_MISO 13
#define PIN_SCK  12

#define CS_SD    10
#define CS_RFID  9
#define RST_RFID 8

MFRC522 rfid(CS_RFID, RST_RFID);

void setup() {
  Serial.begin(115200);

  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  pinMode(CS_SD, OUTPUT);
  pinMode(CS_RFID, OUTPUT);

  digitalWrite(CS_SD, HIGH);
  digitalWrite(CS_RFID, HIGH);

  // --- SD ---
  digitalWrite(CS_SD, LOW);
  if (!SD.begin(CS_SD)) {
    Serial.println("Error SD");
    return;
  }
  digitalWrite(CS_SD, HIGH);

  // --- RFID ---
  digitalWrite(CS_RFID, LOW);
  rfid.PCD_Init();
  digitalWrite(CS_RFID, HIGH);

  // --- Hora (NTP) ---
  configTime(3600, 0, "pool.ntp.org");
}

void loop() {
  digitalWrite(CS_RFID, LOW);

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    digitalWrite(CS_RFID, HIGH);
    return;
  }

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  digitalWrite(CS_RFID, HIGH);

  // Obtener hora
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char timeStr[30];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);

  // Guardar en SD
  digitalWrite(CS_SD, LOW);

  File file = SD.open("/log.txt", FILE_APPEND);
  if (file) {
    file.print(timeStr);
    file.print(" - UID: ");
    file.println(uid);
    file.close();
    Serial.println("Guardado");
  } else {
    Serial.println("Error archivo");
  }

  digitalWrite(CS_SD, HIGH);

  delay(1000);
}