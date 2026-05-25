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
La sortida que es pot veura al Monitor Sèrie dependrà de si el procés té èxit o falla. En un funcionament correcte, primer es veurà el missatge <span style="color: blue;">"Iniciando SD..."</span> seguit immediatament de inicializacion exitosa. 
A continuació, s'imprimirà <span style="color: blue;">"archivo.txt"</span>: i, just a sota, començarà a aparèixer tot el contingut de text que hi hagi guardat dins d'aquest fitxer a la targeta SD. Si hi hagués algun problema tècnic, la sortida s'aturaria amb el missatge <span style="color: blue;">"No se pudo inicializar"</span> (si la targeta SD no està ben connectada o formatada) o bé mostraria <span style="color: blue;">"Error al abrir el archivo"</span> (si la targeta funciona, però el fitxer "/archivo.txt" no existeix).
</div>

### **Explicació del seu funcionament**

<div style="text-align: justify">
Aquest programa té com a objectiu principal connectar-se a un mòdul de targeta SD, llegir un fitxer de text específic i mostrar-ne el contingut a través del port sèrie. Abans de començar la lògica, el codi importa les llibreries necessàries (<code>SD.h</code> per a la comunicació maquinari i <code>SD.h</code> per gestionar l'arxiu) i defineix quins pins del microcontrolador s'utilitzaran per establir la comunicació SPI (MOSI, MISO, SCK i CS). Com que utilitza el constructor <code>SPIClass(FSPI)</code>, és molt probable que aquest codi estigui pensat per a una placa de la família ESP32.

La segona fase té lloc al principi de la funció <code>setup()</code>. Primer s'inicia el port sèrie a una velocitat de 115200 bauds perquè puguem veure els missatges a l'ordinador. Seguidament, s'engega el bus SPI amb els pins personalitzats i s'intenta inicialitzar la targeta SD. Aquest pas funciona com un filtre de seguretat: si la targeta no respon (<code>!SD.begin(...)</code>), el programa imprimeix un error i utilitza la comanda <code>retur</code> per abandonar el procés immediatament, evitant així que el codi intenti llegir un fitxer d'una memòria que no està disponible.

Si la targeta s'ha muntat correctament, entrem en la fase final de lectura. El codi crida la funció <code>SD.open("/archivo.txt")</code> per buscar el fitxer. Si l'arxiu s'obre amb èxit, s'inicia un bucle <code>while</code> que va llegint el document caràcter per caràcter (<code>myFile.read()</code>) i l'envia directament a la pantalla del teu ordinador (<code>Serial.write()</code>) fins que s'arriba al final del text. Un cop acabat, és crucial que es tanqui el fitxer amb <code>myFile.close()</code> per alliberar memòria. Com que tota aquesta seqüència s'ha programat dins del <code>setup()</code>, s'executarà una única vegada quan s'engegui la placa, i per això la funció <code>loop()</code> es manté buida.
</div>

<div style="page-break-after: always;"></div>

### *Exercici 2 - Lectura de etiqueta RFID*

### **Descripció de la sortida del port serie**

> ```cpp
> #include <Arduino.h>
> #include <SPI.h>
> #include <MFRC522.h>
> 
> #define SS_PIN 10
> #define RST_PIN 9
> 
> #define SCK_PIN 12
> #define MISO_PIN 13
> #define MOSI_PIN 11
> 
> MFRC522 mfrc522(SS_PIN, RST_PIN);
> 
> void setup() {
>   Serial.begin(115200);
> 
>   SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
> 
>   mfrc522.PCD_Init();
> 
>   Serial.println("Acerca una tarjeta...");
> }
> 
> void loop() {
>   if (!mfrc522.PICC_IsNewCardPresent()) return;
>   if (!mfrc522.PICC_ReadCardSerial()) return;
> 
>   Serial.print("UID:");
>   for (byte i = 0; i < mfrc522.uid.size; i++) {
>     Serial.print(" ");
>     Serial.print(mfrc522.uid.uidByte[i], HEX);
>   }
>   Serial.println();
> 
>   mfrc522.PICC_HaltA();
> }
> ```

### **Descripció de la sortida del port serie**

<div style="text-align: justify">
Al Monitor Sèrie, tan bon punt s'iniciï la placa, veuràs un únic missatge inicial: <span style="color: blue;">"Acerca una tarjeta....</span> A partir d'aquí, la pantalla es quedarà en espera de forma silenciosa. Cada vegada que passis una targeta o clauer RFID per sobre del lector, apareixerà el text UID: seguit d'una seqüència de nombres i lletres separats per espais (per exemple, UID: 4A B2 E1 8C). Aquest codi és l'identificador únic de la targeta mostrat en format hexadecimal. Si retires la targeta i la tornes a passar (o en passes una de nova), s'imprimirà una nova línia amb el seu corresponent UID.
</div>

### **Explicació del seu funcionament**

<div style="text-align: justify">
Aquest programa té com a finalitat connectar-se a un mòdul lector de radiofreqüència (específicament el model MFRC522) per detectar targetes intel·ligents o clauers NFC i llegir-ne el seu identificador únic (UID). Per aconseguir-ho, primer s'importen les llibreries essencials: <code>SPI.h</code> per a la comunicació per maquinari i <code>MFRC522.h</code> per controlar directament el sensor. Seguidament, es defineixen els pins que formaran el bus SPI (SCK, MISO, MOSI i SS) juntament amb un pin de reinici (RST) que necessita aquest mòdul. Amb tots aquests pins establerts, es crea l'objecte <code>mfrc522</code> que farà de "pont" entre el nostre codi i el maquinari físic.

Dins de la funció <code>setup()</code>, es duen a terme els preparatius que s'executen un sol cop en engegar-se. S'inicia la comunicació sèrie a 115200 bauds perquè el microcontrolador pugui parlar amb l'ordinador. Tot seguit, s'activa el bus SPI obligant-lo a utilitzar els pins personalitzats que s'han declarat a dalt, i s'engega el mòdul lector mitjançant la instrucció <code>mfrc522.PCD_Init()</code>. Abans de passar a la fase de lectura contínua, s'imprimeix un missatge amistós per avisar l'usuari que el sistema està actiu i a l'espera.

A diferència del codi anterior, aquí la funció <code>loop()</code> treballa de valent contínuament. Primer hi trobem dues línies de control vitals: <code>PICC_IsNewCardPresent()</code> comprova si hi ha una targeta a l'abast del sensor, i <code>PICC_ReadCardSerial()</code> intenta llegir-la. Si no hi ha cap targeta o no es pot llegir, la instrucció <code>return</code> fa que s'ignori la resta del codi i el cicle torni a començar de zero. Si la lectura té èxit, s'inicia un bucle <code>for</code> que va agafant l'identificador byte per byte i l'imprimeix per pantalla en format hexadecimal (HEX). Finalment, el codi executa <code>mfrc522.PICC_HaltA()</code>; aquesta comanda és crucial perquè "adorm" la targeta temporalment, evitant que el programa estigui llegint i saturant la pantalla amb el mateix UID milers de vegades per segon mentre la mantinguis a prop del lector.
</div>

<div style="page-break-after: always;"></div>

## Practica 6: Pujada de nota

### **Codi**

#### **Parte 1.- Realizar utilizando el sd y el lector rfid escribiendo en un fichero.log la hora y codigo de cada lectura ( describir como se resuelve el hardware para utilizar un spi para dos perifericos).**

> ```cpp
> #include <SPI.h>
> #include <SD.h>
> #include <MFRC522.h>
> #include <WiFi.h>
> #include <time.h>
> 
> #define PIN_MOSI 11
> #define PIN_MISO 13
> #define PIN_SCK  12
> 
> #define CS_SD    10
> #define CS_RFID  9
> #define RST_RFID 8
> 
> MFRC522 rfid(CS_RFID, RST_RFID);
> 
> void setup() {
>   Serial.begin(115200);
> 
>   SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);
> 
>   pinMode(CS_SD, OUTPUT);
>   pinMode(CS_RFID, OUTPUT);
> 
>   digitalWrite(CS_SD, HIGH);
>   digitalWrite(CS_RFID, HIGH);
> 
>   // --- SD ---
>   digitalWrite(CS_SD, LOW);
>   if (!SD.begin(CS_SD)) {
>     Serial.println("Error SD");
>     return;
>   }
>   digitalWrite(CS_SD, HIGH);
> 
>   // --- RFID ---
>   digitalWrite(CS_RFID, LOW);
>   rfid.PCD_Init();
>   digitalWrite(CS_RFID, HIGH);
> 
>   // --- Hora (NTP) ---
>   configTime(3600, 0, "pool.ntp.org");
> }
> 
> void loop() {
>   digitalWrite(CS_RFID, LOW);
> 
>   if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
>     digitalWrite(CS_RFID, HIGH);
>     return;
>   }
> 
>   String uid = "";
>   for (byte i = 0; i < rfid.uid.size; i++) {
>     uid += String(rfid.uid.uidByte[i], HEX);
>   }
> 
>   digitalWrite(CS_RFID, HIGH);
> 
>   // Obtener hora
>   struct tm timeinfo;
>   getLocalTime(&timeinfo);
> 
>   char timeStr[30];
>   strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
> 
>   // Guardar en SD
>   digitalWrite(CS_SD, LOW);
> 
>   File file = SD.open("/log.txt", FILE_APPEND);
>   if (file) {
>     file.print(timeStr);
>     file.print(" - UID: ");
>     file.println(uid);
>     file.close();
>     Serial.println("Guardado");
>   } else {
>     Serial.println("Error archivo");
>   }
> 
>   digitalWrite(CS_SD, HIGH);
> 
>   delay(1000);
> }
> ```

### **Explicació del codi**

<div style="text-align: justify">
Aquest programa evoluciona els codis anteriors integrant un lector RFID, un mòdul de targeta SD i sincronització horària d'Internet (NTP) mitjançant les capacitats Wi-Fi d'una placa com l'ESP32. La gran diferència i complexitat d'aquest disseny és que el lector RFID i la targeta SD comparteixen els mateixos pins del bus SPI (MOSI 11, MISO 13 i SCK 12). Per evitar que els dos dispositius parlin a la vegada i es col·lapsin, el codi utilitza una tècnica de gestió de maquinari manual controlant els pins de selecció de xip (<code>CS_SD</code> i <code>CS_RFID</code>), activant un dispositiu posant el seu pin en <code>LOW</code> i apagant-lo posant-lo en HIGH.

Dins del <code>setup()</code>, es configuren aquests dos pins de control com a sortides i es posen immediatament en HIGH per "enviar a callar" ambdós mòduls abans de començar. A continuació, el programa fa una inicialització seqüencial: primer activa exclusivament la SD (<code>CS_SD</code> en <code>LOW</code>), comprova si funciona i la torna a desactivar. Després fa exactament el mateix amb el lector RFID mitjançant <code>rfid.PCD_Init()</code>. Per acabar els preparatius, la funció <code>configTime()</code> configura el rellotge intern de la placa demanant l'hora real a un servidor d'Internet (<code>pool.ntp.org</code>), assumint que la placa ja s'ha connectat prèviament a una xarxa Wi-Fi.

El bloc <code>loop()</code> és un cicle continu que es divideix en dues fases gestionades pels pins de selecció. A la primera fase, el microcontrolador activa el lector de targetes (<code>CS_SD</code> en <code>LOW</code>) i es queda escoltant si hi ha cap targeta a prop. Si no hi ha cap lectura nova, es desconnecta el xip RFID i el codi torna a començar de zero gràcies a la instrucció <code>return</code>. Però, si es detecta una targeta amb èxit, el programa extreu el seu identificador únic (UID), el converteix en una cadena de text en format hexadecimal (<code>String</code>) i immediatament desactiva el lector RFID per alliberar la línia de comunicació.

A la segona fase del <code>loop()</code>, un cop obtingut l'UID, el programa utilitza les estructures de cronometratge internes de C++ (<code>tm</code> i <code>strftime</code>) per capturar l'hora exacta de la placa i donar-li un format llegible de data i hora (Any-Mes-Dia Hora:Minuts:Segons). Tot seguit, s'apaga el RFID, s'activa la targeta SD (<code>CS_SD</code> en <code>LOW</code>) i s'obre un fitxer anomenat <code>/log.txt</code> en mode d'escriptura addicional (<code>FILE_APPEND</code>). Això permet escriure una nova línia amb el registre de l'hora i l'UID detectat sense esborrar el que ja hi havia guardat, creant així un historial complet d'accessos abans de tancar el fitxer i esperar un segon (<code>delay(1000)</code>) per a la següent lectura
</div>

#### **Parte 2.- Generar una pagina web donde se pueda ver la lectura del lector rfid**

![DIAGRAMA DE TIEMPO PUJADA DE NOTA](./Imatge.png)

### **Descripció de la sortida del port serie**

<div style="text-align: justify">
La sortida del Monitor Sèrie en aquest programa és molt més minimalista i està pensada exclusivament com un sistema de confirmació en segon pla. Durant l'arrencada, si la targeta SD no està a punt, veuràs el missatge <span style="color: blue;">"Error SD"</span> i el programa s'aturarà. Si tot s'inicia correctament, la pantalla es quedarà completament en blanc. No serà fins que apropis una targeta RFID al lector que començaran a passar coses: si el sistema llegeix la targeta i aconsegueix escriure correctament les dades a la memòria, es mostrarà la paraula <span style="color: blue;">"Guardado"</span>. En cas que la targeta SD falli en aquell moment precís, veuràs el missatge <span style="color: blue;">"Error archivo"<span>.
</div>
