# trimotor

TO DO LIST per completare il codice base
[x] eseguire homing correttamente
[ ] calcolare le misure MAX con funzione dedicata
[ ] aggiunta del pin di ENABLE nel codice
[ ] bool se presente o meno DMX
[ ] canale di consenso movimento (in modalità safe run ignora il canale di setup e comandi in serial monitor, modalità programmer ascolta anche il canale di setup)
[ ] canale di setup (attivo solo in modalità programmatore o standby)
[ ] lettura e scrittura eeprom
[ ] setup variabili tramite monitor seriale
[ ] funzione di intervento di emergenza (fungo)
[ ] sgancio motori remoto

features del canale di setup:
[ ] system reset (homing motori e ritorno in run)
[ ] enble RDM
[ ] disable RDM
[ ] offset cuscinetto ON
[ ] offset cuscinetto OFF
[ ] disabilitazione motori (ne consegue una richiesta di system reset)
[ ] timeout DMX ON
[ ] timeout DMX OFF

LOGICA DEL SISTEMA:
1> power on
2> il sistema carica i dati dalla eeprom
3> standby , attendo il DMX (lampeggio LED 1)
4> sistema in standby , attendo il system reset (lampeggio LED 2);
5> system reset = en driver -> homing
6> standby , ready (lampeggio LED 3 presenza DMX)
7> attendo canale del consenso
8> run mode -> motori = valori DMX
9> motori freezati su EN se consenso spento.
10> attendo comando di disabilitazione motori
