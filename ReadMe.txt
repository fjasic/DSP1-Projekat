"NoiseGenerator" i "NoiseRemoval" - šablon za izvršavanje projekta na razvojnoj ploči
"NoiseGenerator Sim" i "NoiseRemoval Sim" - šablon za izvršavanje projekta na simulatoru. 
"streams" - teste audio datoteke (govorni signal)

- Sa simulatorom koristiti .pcm datoteke kao ulaz i izlaz.
- Za otvaranje .pcm datoteke koristiti alat Audacity, opcija File->Import->Raw Data..., prilikom otvaranja odabrati frekvenciju odabiranja 8000, tip podataka kojim su predstavljeni odbirci Signed 16-bit i broj kanala 2). 
- Za pravljenje nove .pcm datoteke koristite Audacity, datoteku sačuvajte sa File->Export, odabirte tip datoteke Other uncompressed files i pritisnite Options. Odaberite format RAW(header-less), i encoding Signed 16-bit PCM

--Sistem koji je potrebno realizovati sastoji se iz 2 dela: bloka za dodavanje šuma u signal i bloka za
uklanjanje šuma iz govornog zvučnog signala. Šum koji se dodaje u govorni signal jeste tzv. periodični
šum. Periodični šum predstavlja jednu ili više sinusoidalnih komponenti koje se javljaju na određenim
frekvencijama. Ovakav tip šuma najčešće javlja se u sistemima za obradu signala kao posledica spoljnog
uticaja na komponente sistema. 

--Jedan primer pojave ovakvog šuma u sistemima za obradu zvuka jeste
pojava niskofrekventne komponente, frekvencije naizmenične struje (50Hz) kod pojačivačkih uređaja.
Ovakav tip šuma najčešće se uklanja koristeći uskopojasni filtar nepropusnik opsega (eng. notch).

--Sistem je potrebno realizovati koristeći dve TMS320C5535 
eZdsp razvojne ploče. Izvor zvuka može biti PC računar ili mikrofonski uređaj. Signal s(t) koji je emitovan
od strane izvora zvuka, predstavlja korisni signal u našem sistemu. Prva ploča, eZdsp1, povezana je sa
izvorom zvuka koristeći dvokanalni audio in ulaz. Na eZdsp1 se izvršava blok za dodavanje šuma u signal.
Izlaz iz sistema jeste signal u koji je dodat aditivni sinusoidalni šum s(t)+n(t), i on je prosleđen na audio
out izlaz na razvojnoj ploči. Druga razvojna ploča, eZdsp2 korsiti se za izvršavanje bloka za uklanjanje
šuma. Ulaz audio in na eZdsp2 povezan je sa izlazom audio out na eZdsp1. Dakle, ulaz u sistem je signal
sa prisutnim šumom. Izlaz iz sistema jeste filtriran korisni signal s(t), i on je emitovan kroz audio out izlaz
na razvojnoj ploči.