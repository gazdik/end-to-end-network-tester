# Jednoduchý tester end-to-end parametrov siete #

Vyžaduje knižnice BOOST.

# Popis #

Jednoduchý tester slúžiaci k zisteniu a sledovaniu parametrov end-to-end 
komunikácie. Pozostáva z klientskej časti a serverovej časti.

## Spustenie servera ##

```
./ipkperfserver [-p port]
```

Parametre:

 - `-p --port` UDP port, na ktorom bude server počúvať (implicitne 5656) 

Server sa korektne vypne po obdržaní signálu SIGINT.

## Spustenie klienta ##

```
./ipkperfclient [-p server_port] [-s packet_size] [-r packet_rate] [-t runtime] [-i interval] SERVER
```

Parametre:

 - `-p --port` UDP port servera
 - `-s --size` veľkosť payload paketu, imlicitná hodnota je 
 100 bajtov, minimálna hodnota je 10 bajtov.
 - `-r --rate` rýchlosť odosielania v packetoch za sekundu, implicitná hodnota je 10
 - `-t --timeout` doba behu klienta [s], ak nie je zadaná, potom klient beží nekonečne dlho
 - `-i --interval` označuje dĺžku meraného intervalu
 - `SERVER` meno alebo adresa servera

## Výsledky merania ##

Klient ukladá výsledky do súbora s názvom `ipkperf-SERVERIP-SIZE-RATE`. 
Ak výsledný súbor už existuje, dáta sa k nemu pripoja.

Výsledky sú uložené v csv formáte následovne:

```
Time, IntLen, PcktSent, Pckt Recv, Bytes Send, Bytes Recv, Avg Delay, Jitter, OutOfOrder
```

kde:

 - `Time` označuje začiatok meraného intervalu
 - `IntLen` označuje dĺžku každého meraného intervalu
 - `Pckt Sent` označuje množstvo odoslaných paketov v tomto intervale
 - `Pckt Recv` označuje množstvo prijatých paketov v tomto intervale
 - `Bytes Send` označuje množstvo odoslaných dát v bytoch
 - `Bytes Recv` označuje množstvo prijatých dát v bytoch
 - `Avg Delay` priemerný RTT (Round Trip Time) paketov v príslušnom intervale
 - `Jitter` maximálna hodnota Packet Delay Variation
 - `OutOfOrder` počet paketov v nesprávnom poradí (v %)

# Implementácia #

## Server ##

V slučke odpovedá na pakety prijaté zo strany klienta. Tieto pakety 
nijak nemodifikuje. Činnosť servera končí po obdržaní signálu SIGINT.

## Klient ##

Klient odosiela pakety obsahujúce časové razítka s časom odoslania 
na server, odkiaľ sú v nezmenenej podobe odosielané naspäť. Okrem 
hlavičky s časovým razítkom neobsahuje žiadne užitočné dáta, takže 
zvyšok správy tvoria len náhodné dáta. Po prijatí správy sa zaznamená 
čas jej prijatia s spolu s časom odoslania sa uloží do dátovej 
štruktúry zachovávajúcej poradie vložených prvkov. Po prijatí 
všetkých dát odoslaných v jednom intervale dochádza k ich spracovaniu 
a vyhodnoteniu všetkých parametrov merania. Po skončení spracovania 
a zapísaniu výsledkov do súboru sa celý proces opakuje.

Po vypršaní doby behu programu alebo po obdržaní signálu SIGINT dôjde 
k zastaveniu odosielania dát na server a k dokončeniu spracovania doposiaľ
odoslaných dát v príslušnom intervale. Následne činnosť klienta končí. 

Túto činnosť klient zabezpečuje až pomocou 5 vlákien. Hlavné vlákno 
zabezpečuje riadenie toku programu, vytváranie nových vlákien a 
analýzu paketov. Dve vlákna slúžia k meraniu času. Posledné dve 
realizujú odosielanie a prijímanie paketov. 

## Protokol ##

### Formát ###

```
 +--------+--------+--------+--------+
 |                                   |
 |           Čas odoslania           |
 +--------+--------+--------+--------+
 |                                   | 
 |                Dáta               |
 |
 +---------------- ...

```

Hlavička protokoluje obsahuje len časové razítko s časom odoslania paketu 
zo strany klienta v nanosekundách. Zvyšok tvoria náhodné dáta.

Maximálna veľkosť paketu je 65 507 bytov daná maximálnou veľkosťou 
protokolu IPv4, t.j. 65 535 bytov, kde hlavička IP protokolu zaberá 
20 bytov a hlavička UDP protokolu 8 bytov.

## Testovacia sada ##

Testovacia sada zahŕňa testovanie základnej funkčnosti aplikácie. Po
spustení testovacej sady dôjde k vymazaniu výstupných súborov z predchádzajúcich
meraní kvôli prehladnosti výstupov z jednotlivých testov.

Bližšia činnosť jednodtlivých testov je zrejmá po ich spustení.
