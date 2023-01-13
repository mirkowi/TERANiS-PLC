# TERANiS-PLC
Soft-SPS zum Softwareprodukt TERANiS der AVI-GmbH

Eine Beschreibung zu TERANiS findet man hier: https://www.avi-gmbh.com/produkte/teranis/

Hinweise zur Installation und Konfiguration stehen im Projekt-Wiki.

## Einleitung

TERANiS wird für die technologische Beschreibung von Steuerungsabläufen verwendet. Es existieren Übersetzer für verschiedene Zielsprachen aus der industriellen Steuerungswelt. Es gibt aber auch einen Übersetzer in die Sprache C. Damit kann mit einer Runtime-Umgebung eine Software-Steuerung (Soft-PLC) gebaut werden.
In diesem Projekt wird diese Runtime-Umgebung entwickelt und steht als Open-Source zur Verfügung.

## Anforderungen

### Zielsysteme

Die Runtime soll mit abgestuften Features auf verschiedenen Zielsystemen lauffähig sein.
* Microcontroller mit Entwicklungsumgebung Arduino ab 8 bit z.B. AVR, esp8266 etc.
* Linux ARM und x86, 32 und 64 bit.
* Windows 32 und 64 bit

Als Compiler wird GCC eingesetzt.

### Online-Schnittstelle

Damit Prozessabläufe beobachtet werden können, muss TERANiS auf die Prozesswerte in der Steuerung zugreifen können. Wegen der einfachen Implementierung wird dafür das MODBUS-Protokoll (mit einigen Erweiterungen) verwendet. In leistungsfähigen Umgebungen kann auch OPC-UA zum Einsatz kommen. Über diese Schnittstelle können auch diverse HMI- oder SCADA-Systeme angekoppelt werden.

### Laden von Änderungen ohne Stop der PLC

Häufig ist es nicht möglich einen laufenden Prozess komplett zu stoppen nur um eine Änderung der Software zu laden. Deshalb wird das Laden ohne Unterbrechung der Abarbeitung angestrebt. Diese Funktion ist nur für Systeme mit dynamisch ladbaren Bibliotheken vorgesehen, also Linux/Windows.

## Benutzung

### Build
Um die TERANiS-PLC auszuführen muss man den Quellcode aus diesem Repository auschecken und mittels [cmake](https://cmake.org/) und [make](https://de.wikipedia.org/wiki/Make) übersetzen.
Vorher muss man evtl. noch die [open62541](https://github.com/open62541/open62541)-Bibliothek herunterladen und kompilieren: Siehe dazu `download-open62541.(bat|sh)` und `compile-open62541-(posix.sh|win32.bat)`.

Beim Build muss das Zielsystem über den Parameter `-DENVIRONMENT=....` festgelegt werden.
Folgende Zielsysteme werden unterstützt:

  - **mraa**: I/Q über [Eclipse MRAA](https://github.com/eclipse/mraa). POSIX
  - **revpi**: I/Q über [Revolution PI](https://revolutionpi.de/). POSIX
  - **windows**: kein I/Q. Die Adressen stehen dennoch zur Verfügung. Win32, ws2_32.

### Programmierung

Die Implementierung des tatsächlichen Codes, der in jedem Zyklus ausgeführt wird, findet sich in `teranis_code.h`. Dort kann beliebiger c++-Code verwendet werden.
Die SPS ist permanent in RUN und kann durch killen des Prozesses gestoppt werden.

**Jede Code-Änderung erfordert einen erneuten Build.**

### Adressen und Variablen

Es gibt einige C++-Makros, die hier den Zugriff auf spezielle Adressbereiche der Soft-SPS erlauben:

  | Bereich | Abkürzung | Bedeutung                            |
  | ------- | --------- | ------------------------------------ |
  | Inputs  | I         | Eingangsadressen                     |
  | Outputs | Q         | Ausgangsadresssen          (Quanta)  |
  | Flags   | F         | Memory-Speicheradressen              |
  | Retain  | RF        | persistente Speicheradressen         |
  | System  | SF        | System-Speicheradressen              |

Die Bereiche sind in der `PlcMemory.h` definiert und haben eine Standardgröße von 256 Bytes.
Der Zugriff erfolgt beispielsweise als `I(int,2)` auf das dritte Eingangs-Integer (Adresse=2*sizeof(int)).
Der Zugriff auf einzelne Bits erfolgt mit angehangenem 'X' an dem Bereichsindikator: `QX(2,2)` (Adresse=drittes Bit des dritten Ausgangsbytes). Die Speicherbereiche beginnen immer bei `X(0,0)`.

Folgende Systemvariablen sind definiert:

  | Adresse      | Symbolischer Name | Bedeutung                              |
  | ------------ | ----------------- | -------------------------------------- |
  | S(int64, 10) | S_usTicks         | Mikrosekunden Ticks zum Zyklusbeginn   |
  | S(int32, 18) | S_cycleActMs      | aktuelle Zykluszeit                    |
  | S(int32, 22) | S_minCycleActMs   | minimale Zykluszeit                    |
  | S(int32, 26) | S_maxCycleActMs   | maximale Zykluszeit                    |

### Modbus

Die SPS startet automatisch einen [MODBUS/TCP](https://de.wikipedia.org/wiki/Modbus)-Server auf dem Standard-Port 502/TCP.
Der Port kann über den Kommandozeilenparameter `-mbport 123` geändert werden. 

**Das ist auch der einzig mögliche Kommandozeilenparameter!**

### OPC/UA

Die SPS startet automatisch einen [OPC/UA](https://de.wikipedia.org/wiki/OPC_Unified_Architecture)-Server mit der Standardkonfiguration von open62541.
Der Server kann in der Datei `opcua/opcua_configuration.cpp` über die Funktion `void configureOpcuaServer(OpcuaServer *server)` konfiguriert werden.
Die einzelnen, veröffentlichten Variablen können in `void configureOpcuaVariables(std::set<OpcuaVariable *> &vars)` definiert werden.

**Dort ist standardmäßig `F(uint32,30)` unter dem Namen `DebugPlcCycleCount` eingetragen.**

## Übersetzung und Installation unter WSL2

Um die Software unter WSL2 zu kompilieren, muss CMAKE sowie MRAA installiert werden. 
Außerdem muss open62541 heruntergeladen und übersetzt werden.

Für Ubuntu:
```
sudo add-apt-repository ppa:mraa/mraa
sudo apt-get update
sudo apt-get install -y \
  cmake \
  libmraa2 libmraa-dev libmraa-java python-mraa python3-mraa mraa-tools

./download-open62541.sh
./compile-open62541-posix.sh
```
