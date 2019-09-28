# TERANiS-PLC
Soft-SPS zum Softwareprodukt TERANiS der AVI-GmbH

Eine Beschreibung zu TERANiS findet man hier: https://www.teranis.de

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
