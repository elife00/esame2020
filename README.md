# Relazione progetto “Modello SIR”
Angeli Ettore, Ferri Elisabetta, Mattei Davide

## INTRODUZIONE

Il progetto consiste nella simulazione della diffusione di un’epidemia all’interno di una popolazione, basata sul Modello SIR.[9]

Il programma parte dalla creazione di una griglia quadrata bidimensionale, all’interno della quale un insieme di celle, che rappresenta la popolazione in esame, può muoversi casualmente. Le celle che formano la griglia possono assumere cinque stati. “Susceptible”,  “Infected” e “Recovered” rappresentano i tre stati previsti dal modello SIR che una persona può assumere (il modello prevede che una persona guarita non possa più infettarsi). Lo stato “Empty” rappresenta le celle vuote, tra le quali la popolazione è libera di muoversi. Infine si è scelto di aggiungere lo stato "Quarantine", che rappresenta la popolazione in quarantena, la quale è infetta ma non può interagire né muoversi all’interno della griglia. La scelta di usufruire di quest’ultimo stato è lasciata all’utente.
Lo sviluppo dell’epidemia è determinato da diversi parametri, tra cui la probabilità di infettarsi di una cella suscettibile quando interagisce con una infetta (“pInf”) o il raggio d’azione (“range”) che definisce la distanza di interazione tra le celle.

La scelta del valore dei diversi parametri è lasciata all’utente; alternativamente verranno generati valori casuali (all’interno di range opportuni).

Si è utilizzata la libreria grafica SFML per la visualizzazione della griglia nel corso dell’epidemia e ROOT per la creazione del grafico, che rappresenta a posteriori l’andamento dei tre parametri del modello (S, I, R).

## DESCRIZIONE DEL PROGRAMMA

I cinque stati sono definiti attraverso una enumeration “State” di tipo char (scelto per ragioni di ottimizzazione), abbreviati poi come variabili globali “constexpr” nelle sigle “E”, “S”,” I”, “R” e “Q”, che verranno utilizzate all’interno del codice [5].

(Col termine “cella viva” si intende una cella che non si trova allo stato Empty.)

Si è scelto poi di creare una struct “Situation” composta di 4 numeri interi: “t” rappresenta il tempo trascorso (ogni iterazione può essere pensata come un giorno), mentre “s”, “i” e “r” sono rispettivamente il numero di celle/persone suscettibili, infette e guarite].

È stato necessario poi definire l’operatore “==“ al fine di eseguire i test sul codice [3].

La griglia bidimensionale è rappresentata dalla classe "Board" che è definita nel “private” da cinque variabili: “n_” definisce la lunghezza dei lati della griglia, che sarà dunque composta di n_ * n_ celle, “density_” rappresenta la densità percentuale della popolazione. Il vettore "board_” rappresenta gli stati delle singole celle che compongono la griglia. Il vettore “stay_” rappresenta il numero di iterazioni (giorni) di malattia di ogni cella. Infine il vettore di “Situation” “evolution_” rappresenta l’evoluzione dell’epidemia, in quanto tiene conto ad ogni iterazione del numero di celle suscettibili, infette e guarite.

Il costruttore necessita di due sole variabili, ossia la lunghezza dei lati della griglia (“n”) e la densità percentuale (“d”). Del vettore "board_", inizializzato a n * n elementi, tutte le  n * n * d celle “vive” vengono inizializzate allo stato “Susceptible” (facendo una forzatura ad intero del numero), le restanti celle vengono settate allo stato "Empty"; infine utilizzando l’algoritmo “shuffle” vengono rimescolate le celle. Viene anche impostato il primo elemento del vettore "situation_" alla situazione iniziale, ossia (0, S, 0, 0) dove S è n * n * d. Dunque tutte le celle vive saranno inizialmente suscettibili.

### Breve descrizione delle funzioni membro.

Premessa: si è scelto di utilizzare l’intero [(y - 1) * n_ + (x - 1)] per indicare la i-esima cella, dati i valori x e y, in modo che le coordinate siano comprese nell’intervallo [(1,1),(n,n)], invece che [(0,0),(n-1,n-1)].

* “get” : riceve in input le coordinate (x,y) e restituisce lo stato della cella in esame. Se le coordinate escono dal range [(1,1),(n,n)], la funzione ritorna Empty, per escludere che le celle esterne siano considerabili potenzialmente infette.

* “set” : riceve in input le coordinate (x,y) e uno stato S e setta ad S la cella in esame.

* “contact”: riceve in input le coordinate (x,y) e un intero r, che rappresenta il raggio d’azione del virus, e restituisce il numero di celle infette all’interno del range, ossia in un quadrato di lato 2r+1.

* “infection” : dà inizio all’epidemia. Riceve in input un numero decimale “ratInf” che rappresenta la percentuale iniziale di infetti sulla popolazione. La funzione dunque setta   n * n * d * ratInf (valore forzato ad intero) celle suscettibili allo stato "infected". Infine attraverso l’algoritmo "shuffle" vengono mischiate le celle.

* “swap” : permette alla cella di coordinate (x,y) di muoversi casualmente all’interno della griglia. Vengono generati due numeri interi casuali nel range [-1,1] “i” e “j” e viene analizzato lo stato della cella nelle nuove coordinate casuali (x+i,y+j). Ad ogni iterazione una cella può dunque muoversi solo nel caso in cui lo stato in esame sia "Empty". La cella rimane invece ferma nel caso in cui la nuova coordinata esca dalla griglia, nel caso in cui in essa sia già presente una cella viva oppure se i due numeri generati sono entrambi 0. Quando una cella si sposta viene eseguito uno scambio anche degli interi del vettore "stay_", che tengono traccia dei giorni di malattia.

* “epidemic”: fa evolvere la griglia in base alle caratteristiche dell’epidemia. Riceve in input quattro variabili: “pInf” rappresenta la probabilità che una cella suscettibile sia infettata quando interagisce con una cella infetta, “range” rappresenta la distanza massima di interazione, “avgTime” è il tempo medio di malattia e infine la variabile booleana “quarantine” determina l’attivazione o meno dell’opzione quarantena.

Attraverso un doppio "ciclo for” vengono analizzate le singole celle della griglia in modo da determinare gli stati che andranno a costituire la nuova griglia “next”, che rappresenta la popolazione alla successiva iterazione. La funzione si basa sulla generazione di numeri casuali attraverso la libreria <random>.
  
Caso 1 – la cella in esame è suscettibile. Vengono contate le celle infette nel raggio d’azione del virus attraverso la funzione “contact”. Questo numero, chiamato “infected”, e “pInf” costituiscono l’input dell’algoritmo “std::binomial_distribution“, che genera un
numero intero casuale nel range [0,infected] secondo una distribuzione binomiale, il quale rappresenta quante tra le interazioni totali hanno avuto successo, ossia hanno portato all’infezione della cella. 
Si agisce poi sulla nuova griglia “next”. Se il numero casuale è maggiore di 0, la cella i-esima viene settata ad "infected", alternativamente viene settata a “Susceptible”, ossia non subisce alcuna modifica.

Caso 2 – la cella in esame è infetta. In questo caso viene prodotta una probabilità casuale attraverso l’algoritmo “std::uniform_real_distribution”.  Attraverso l’input “avgTime” si dispone indirettamente della probabilità media di guarigione ad ogni iterazione, ossia 1/avgTime. Se la probabilità casuale è minore o uguale alla probabilità media di guarigione (funzione costante), il valore viene accettato (“Metodo hit or miss”) e dunque la cella i-esima guarisce e viene settata a "Recovered". In caso contrario non viene modificata.
Se l’opzione “quarantine” è attiva si va ad analizzare i giorni trascorsi di malattia attraverso il vettore "stay_"  (il quale viene incrementato ad ogni iterazione nel caso la cella sia infetta). Se sono trascorsi la metà dei giorni medi di malattia, la cella viene settata allo stato "Quarantine" e non sarà più in grado né di interagire, né di muoversi sino alla completa guarigione.

Caso 3 – la cella in esame è guarita. In questo caso non viene eseguita alcuna modifica.

Caso 4 – la cella in esame è in quarantena. Identico al caso 2, dunque in caso di mancata guarigione la cella rimane in quarantena.

Caso 5 – la cella in esame è vuota.Anche in questo caso non viene eseguita alcuna modifica.

Nei casi 1-2-3-4 vengono anche incrementati gli interi “s”, “i”, “r” che costituiscono la “situation” della griglia che verrà aggiunta al vettore “next.evolution_” (Notare che nel caso 4 viene incrementato “i”, dunque le celle in quarantena contano come infette, inoltre l’intero “t” viene incrementato alla fine della funzione).

E’ importante sottolineare che in questo modo l’ultimo elemento del vettore rappresenta la situazione della griglia all’iterazione precedente. 

A questo punto viene eseguito un secondo ciclo sulla griglia “next” agendo su ogni
cella allo stato "Susceptible", "Infected" o "Recovered attraverso la funzione “swap”. (E’ necessario un secondo ciclo in quanto si deve già disporre della griglia evoluta).

* “avg_time” : calcola la media effettiva dei giorni di malattia dell’epidemia. La media viene fatta utilizzando l’algoritmo “std::accumulate” che agisce sul vettore "stay_". Essenzialmente controlla che l’evoluzione sia in accordo col parametro avgTime.

* “trend” : stampa sul file “trend.txt” il vettore “evolution_”, dunque l’andamento dei tre parametri del modello nel tempo, oltre che la media effettiva dei giorni di malattia.

* “situation”: accede al vettore privato “situation_” e ne restituisce l’ultimo elemento, ossia la situazione all’iterazione precedente. Viene utilizzata per eseguire i test sulla funzione “epidemic”.

* ”end” : scorre le celle della griglia e restituisce il numero di celle infette. Viene utilizzato per terminare l’evoluzione della griglia, ossia quando il valore restituito è 0.

* ”draw” : passa in input la griglia “board_” alla classe “RepresentBoard”, utilizzata per la rappresentazione grafica, attraverso la libreria SFML.

Il programma prevede un'interfaccia utente direttamente sul terminale. L’utente può decidere se inserire manualmente i parametri che caratterizzano l’epidemia, oppure farli generare casualmente. In entrambi i casi questi valori vengono stampati su terminale.
I parametri richiesti sono:
* “density” - la densità della popolazione all’interno della cella, 
* “percInf” - la percentuale di infetti iniziali sulla popolazione,
* “pInf” - la probabilità di infezione per contatto diretto,
* “avgTime” - il tempo medio di malattia,
* “range” - raggio d’azione del virus.

Caso 1 - parametri inseriti in input

Questo caso è gestito dalla funzione “input_parameters”. Vengono chiesti in ordine i valori dei parametri all’utente, i quali vanno a riempire il vettore restituito dalla funzione.

I valori di input vengono inizializzati come stringhe, in modo da gestire il caso in cui l’utente abbia inserito “,” al posto di “.” nei valori percentuali. All’interno di un “ciclo while” si utilizza l’algoritmo “stod” della libreria <string> per trasformare la stringa nel valore di tipo “double”, in seguito si controlla che questi valori rientrino in opportuni range, ossia [0,1] per le prime tre percentuali, [1,40] per il tempo medio di malattia e [1,10] per il raggio d’azione. Questi ultimi due range sono del tutto arbitrari. Si è cercato di scegliere valori in modo da garantire che l’epidemia rappresentata sia verosimile. [4]

Caso 2 - parametri generati casualmente

Questo caso è gestito dalla funzione “output_parameters”. Si è scelto di generare 4 diversi numeri casuali secondo distribuzioni uniformi in diversi range. Anche in questo caso la scelta è finalizzata a garantire uno sviluppo apprezzabile dell’epidemia, escludendo dunque valori che rendano lo sviluppo eccessivamente lungo o eccessivamente breve. Per la densità si è scelto il range [0.1,0.75], mentre per i due valori percentuali pInf e percInf [0.01,0.2]. Si è visto infatti che oltre questi valori, in particolare se combinati ad un’alta densità e un grande raggio d’azione, l’epidemia termina in un tempo estremamente breve, così come, al di sotto di essi, si ha l’effetto contrario.

Infine per il tempo medio di malattia si è scelto l’intervallo [5,30] e [1,3] per il range d’azione del virus. [4]

La scelta di implementare o meno l’opzione quarantena è lasciata sempre all’utente, anche nel caso 2, attraverso la funzione “input_quarantine”. Essa restituisce una variabile booleana che sarà poi inserita nella funzione “epidemic”. [4]

## ISTRUZIONI PER COMPILAZIONE, ESECUZIONE E TEST

Per la compilazione dei file di codice si è predisposto il file “CMakeLists.txt” da passare al programma “cmake” con le istruzioni di compilazione. È necessario assicurarsi innanzitutto di avere “cmake” e “ROOT” installati sul proprio computer e in seguito creare una cartella denominata “build” per poi entrarvi.
```
mkdir build
cd build
```
All’interno della cartella chiamare la seguente opzione di cmake:
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
Dopo aver eseguito questo comando la prima volta non servirà più ripeterlo, e basterà chiedere a cmake di compilare i files con il comando:
```
make
```
Quest’ultimo genererà tre eseguibili: “run_epidemic_tests” e “run_sfml_tests” sono gli eseguibili dei test, mentre “epidemic-sfml” è l’eseguibile del programma da mandare in esecuzione con il comando:
```
./epidemic-sfml
```
Il programma parte chiedendo all’utente che lo utilizza se desidera inserire i parametri dell’epidemia o preferisce che vengano generati casualmente. Un’ulteriore possibilità lasciata all’utente è quella di scegliere se attivare o meno l’opzione “quarantena”. Dopo l’inserimento delle risposte, la simulazione parte con i dati scelti dall’utente o generati casualmente mostrando l’evoluzione sulla finestra grafica.

Finita la simulazione la finestra grafica viene chiusa automaticamente, e viene avviato il programma “ROOT” su cui caricare la macro “graph.cpp”, che rappresenta i dati raccolti dalla simulazione. Per caricare la macro si inserisca nella linea di comando di “ROOT” il seguente comando, che va a cercare la macro salvata nella cartella superiore a build:
```
.x ../graph.cpp
```
Anche dopo la chiusura del programma e di “ROOT” sono disponibili nella cartella build il file immagine “ModelloSir.gif”, che rappresenta il grafico appena visualizzato, e il file “trend.txt” dove sono elencati il numero di “Susceptible”,  “Infected” e “Recovered” per ogni iterazione, utilizzato per la generazione del grafico.

Per l’esecuzione dei test sugli header abbiamo creato due file di codice, uno per testare “sfml.hpp” e uno per “epidemic.hpp”, chiamati rispettivamente “sfml.test.cpp” e “epidemic.test.cpp”, che utilizza il testing framework “doctest.h”.

Gli eseguibili di questi file vengono generati con l’invio del comando “make” da terminale e se mandati in esecuzione indicano lo stato dei test. In alternativa “cmake” fornisce il comando `ctest`, che esegue automaticamente tutti i file indicati come test in “CMakeLists.txt”.

## DESCRIZIONE DEL FORMATO OUTPUT

Per rappresentare l'andamento dell'epidemia si e' utilizzata la libreria grafica SFML, che fornisce classi e funzioni, da completare con parametri adeguati, per la creazione di due finestre interattive. La prima, chiamata "My epidemic", rappresenta ad ogni iterazione la situazione della griglia; si e' deciso di rappresentare con i seguenti colori i vari stati delle celle: 
* "Susceptible" = Blue
* "Infected" = Red
* "Recovered" = Yellow
* "Quarantine" = White
* "Empty" = Black

/*image*/

La seconda,"Legend", e' la legenda della finestra grafica. Sono presenti anche due frecce, con a fianco le scritte "Slower" e "Faster", per indicare che e' possibile aumentare o diminuire il tempo di attesa tra ogni iterazione attraverso la tastiera. [5]

/*image*/

Le due finestre sono direttamente collegate di modo che la richiesta di chiusura della finestra principale comporta anche la chiusura della legenda, mentre il contrario non avviene. La finestra principale evolve autonomamente, fermandosi quando non sono piu' presenti celle "Infected" nella griglia, terminando dopo un breve arco di tempo (3 s). [1]

A questo punto tutti i dati relativi all'epidemia sono trascritti nel file di testo "trend.txt" dalla funzione "trend". Viene quindi aperto il programma "ROOT" e, eseguendo la macro "graph.cpp", viene creato il file "ModelloSir.gif" che rappresenta l'andamento dei parametri "Susceptible", "Infected" e "Recovered" in funzione del tempo. I colori nel grafico creato da "ROOT" seguono quelli della finestra grafica di SFML. [8]

/*image*/

## STRATEGIA DI TEST

Per i test viene utilizzato il testing framework “doctest”.

Il primo test riguarda le funzioni “contact” e “set”. Viene creata una griglia 5x5 e vengono settati ad “Infected” determinate celle, in modo che la cella centrale sia a contatto diretto con due sole celle infette ma interagisca in un raggio 2 con altre 4 celle infette. Viene dunque testata la funzione “contact” con i due diversi range. 

Il secondo test riguarda la funzione “epidemic”, dunque anche “infection” e “avg_time”.
I parametri dell’epidemia vengono generati casualmente secondo distribuzioni uniformi, questa volte in range più ampi, in modo da testare i casi limite. Il test sfrutta la funzione “situation”, la quale però, come descritto in precedenza, ritorna la situazione della griglia all’iterazione precedente. Per questo motivo il controllo sulla situazione successiva allo scoppio dell’epidemia (“infection”) viene fatta in seguito alla prima evoluzione (“epidemic”). Ad ogni iterazione viene fatto il controllo che il numero totale di celle vive, ossia S+I+R, sia costante, come previsto dal modello. Infine viene verificato, attraverso la funzione “avg_time”, che il tempo medio effettivo sia comparabile (con un’incertezza di 0.5) col parametro di input “avgTime”, e dunque che il metodo scelto (“Hit or Miss”) funzioni correttamente. [6]

Il test “testing sfml” della classe representBoard parte dalla creazione di un vettore di stati Empty, poi passato alla classe che ne genera una griglia, testando la funzione costruttore. I primi controlli verificano che vengano generati abbastanza vertici per la rappresentazione del vettore e che questi siano neri, come previsto per lo stato. In seguito, si modifica il vettore aggiungendo stati “Infected” e si controlla che anche la board converta un numero adeguato di vertici al colore rosso.[7]

## INTERPRETAZIONE DEI RISULTATI OTTENUTI

Di seguito si presentano tre esempi di epidemia che dimostrano come il variare del raggio di azione del virus e l’opzione “quarantena” influiscano sull’evoluzione dell’epidemia.

-Caso 1 (raggio = 1, quarantena)

/*image*/
/*image*/

-Caso 2 (raggio = 1, no quarantena)

/*image*/
/*image*/

Caso 3 (raggio = 3, quarantena)

/*image*/
/*image*/

I primi due casi differiscono solamente per l’opzione quarantena. Si nota che nel caso 1 il numero di suscettibili si stabilizza intorno a 1700, a differenza del caso 2, in cui tende a 0. Dunque l’opzione quarantena garantisce che una parte della popolazione non si infetti, e varia considerevolmente l’andamento dell’epidemia.

Il caso 3 differisce dal caso 2 solamente per il valore del raggio d’azione, settato a 3. Nonostante sia implementata l’opzione quarantena, l’epidemia evolve molto rapidamente al numero massimo di infetti. Infatti il picco di infezione viene raggiunto entro i primi 5 giorni, a differenza del caso 1, circa 15 giorni, e del caso 2, circa 10 giorni.

## INFORMAZIONI AGGIUNTIVE

L’implementazione corrente, rilasciata con la distribuzione Ubuntu 18.04, delle librerie utilizzate per la grafica di SFML ha una gestione degli oggetti che a seguito dell’ analisi di `-fsanitize=address` presenta dei memory leak (si veda appendice A), che non producono errori al runtime e che non possiamo correggere direttamente. Scegliamo quindi di tollerare questi messaggi del “-fsanitize=address” dopo aver verificato che non si traducono in problemi durante l’esecuzione della nostra applicazione.

### REFERENZE

[1] main.cpp

[2] epidemic.cpp

[3] epidemic.hpp \'n\
[4] functions.hpp

[5] sfml.hpp

[6] epidemic.tests.cpp

[7] sfml.tests.cpp

[8] graph.cpp

[9] Modello SIR:

[https://royalsocietypublishing.org/doi/10.1098/rspa.1927.0118](https://royalsocietypublishing.org/doi/10.1098/rspa.1927.0118)

[http://www.science.unitn.it/~anal1/biomat/note/epidem_omog.pdf](http://www.science.unitn.it/~anal1/biomat/note/epidem_omog.pdf)

### Appendice A (output -fsanitize=address):
```
==12759==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 400 byte(s) in 5 object(s) allocated from:
#0 0x7ffe9a91eb40 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb40)
#1 0x7ffe98181d6c  (/usr/lib/x86_64-linux-gnu/libfreetype.so.6+0x11d6c)
Direct leak of 216 byte(s) in 1 object(s) allocated from:
#0 0x7ffe9a91eb40 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb40)
#1 0x7ffe937a3372  (/usr/lib/x86_64-linux-gnu/libGLX_indirect.so.0+0x43372)
Indirect leak of 11232 byte(s) in 52 object(s) allocated from:
#0 0x7ffe9a91eb40 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb40)
#1 0x7ffe937a3372  (/usr/lib/x86_64-linux-gnu/libGLX_indirect.so.0+0x43372)
Indirect leak of 1253 byte(s) in 15 object(s) allocated from:
#0 0x7ffe9a91eb40 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb40)
#1 0x7ffe98183e53  (/usr/lib/x86_64-linux-gnu/libfreetype.so.6+0x13e53)
Indirect leak of 56 byte(s) in 1 object(s) allocated from:
#0 0x7ffe9a91ed28 in __interceptor_calloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xded28)
#1 0x7ffe90615dd2  (<unknown module>)
Indirect leak of 56 byte(s) in 1 object(s) allocated from:
#0 0x7ffe9a91ed28 in __interceptor_calloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xded28)
#1 0x7ffe90615dbb  (<unknown module>)
SUMMARY: AddressSanitizer: 13213 byte(s) leaked in 75 allocation(s).
```
