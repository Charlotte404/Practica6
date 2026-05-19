#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#define PIN_MOSI 11
#define PIN_MISO 13
#define PIN_SCK  12
#define PIN_CS   10

SPIClass spi = SPIClass(FSPI); // o HSPI segons configuració
File myFile;
void setup()
{
Serial.begin(115200);
spi.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);
Serial.print("Iniciando SD ...");
if (!SD.begin(PIN_CS, spi)) {
Serial.println("No se pudo inicializar");
return;
}
Serial.println("inicializacion exitosa");
myFile = SD.open("/archivo.txt");//abrimos el archivo
if (myFile) {
Serial.println("archivo.txt:");
while (myFile.available()) {
Serial.write(myFile.read());
}
myFile.close(); //cerramos el archivo
} else {
Serial.println("Error al abrir el archivo");
}
}
void loop()
{
}