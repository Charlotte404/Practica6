# Practica 6

### *Exercici 1 - Lectura/Escritura de memoria SD*

### **Codi**

> ```cpp
> #include <Arduino.h>
> #include <SPI.h>
> #include <SD.h>
> #define PIN_MOSI 11
> #define PIN_MISO 13
> #define PIN_SCK  12
> #define PIN_CS   10
> 
> SPIClass spi = SPIClass(FSPI); // o HSPI segons configuració
> File myFile;
> void setup()
> {
> Serial.begin(115200);
> spi.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);
> Serial.print("Iniciando SD ...");
> if (!SD.begin(PIN_CS, spi)) {
> Serial.println("No se pudo inicializar");
> return;
> }
> Serial.println("inicializacion exitosa");
> myFile = SD.open("/archivo.txt");//abrimos el archivo
> if (myFile) {
> Serial.println("archivo.txt:");
> while (myFile.available()) {
> Serial.write(myFile.read());
> }
> myFile.close(); //cerramos el archivo
> } else {
>   Serial.println("Error al abrir el archivo");
>   }
> }
> void loop()
> {
> }
> ```

### **Descripció de la sortida del port serie**

<div style="text-align: justify">
...
</div>

### **Explicació del seu funcionament**

<div style="text-align: justify">
...
</div>

Salto de pag

<div style="page-break-after: always;"></div>

### *Exercici 2 - Lectura de etiqueta RFID*

### **Descripció de la sortida del port serie**

<div style="text-align: justify">
...
</div>

### **Explicació del seu funcionament**

<div style="text-align: justify">
...
</div>

<div style="page-break-after: always;"></div>

### *Exercici - Pujada de nota*

### **Foto del montatge**

<div style="text-align: justify">
...
</div>

### **Sortidas de depuraciño (print...)**

<div style="text-align: justify">
...
</div>

### **Codi**

<div style="text-align: justify">
...
</div>

### **Explicació del codi**

<div style="text-align: justify">
...
</div>

