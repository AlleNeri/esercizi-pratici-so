# Esercizi-pratici-so
Esercizi di preparazione alla prova pratica dell'esame di sistemi operativi. [Testi delle prove](http://www.cs.unibo.it/~renzo/so/provapratica.shtml) qui svolte e di mooolte altre.

# System Call e funzioni utili usate in C
- `dlopen`: apertura di una shared lib, ritorna un handler. Corollario: `dlsym`(passato l'handler e un simbolo ne ritorna la funzione associata), `dlerror`(ritorna un errore o NULL) e `dlclose`(performa la corretta chiusura della shared lib). [`/2021-07-15/es1/lancia.c`, `/2021-07-15/es2/autolancia.c`]
- `execv`: esegue un comando passato come parametro con una lista di parametri. Corollario: esistono altre varianti della funzione `exec` (`execv`, `execve`, `execvp`, `execl`, `execlp`, `execs`, ...), per informazioni dettagliate leggere il manuale. [`/2021-07-15/es2/autolancia.c`, `/2023-01-19/es1/stdin2pipe.c`, `/2023-01-19/es2/stdin2pipe.c`, `/2022-09-07/es1/rilancia.c`, `/2022-09-07/es2/rilancia.c`, `/2023-05-31/es2/pidrerun.c`, `/2021-09-16/es1/execname.c`, `/2021-09-16/es2/execname2.c`]
- `execvp`: esegue un comando con parametri nel formato: parametro1=comando da eseguire, parametro2=array argv(col comando stesso come primo elemento e possibilmente NULL come ultimo, ma non sembra catastrofico non farlo). Corollario: vedere `execv`. [`/2019-09-18/es1/para.c`, `/2019-09-18/es2/paran3.c`, `/2018-07-18/es2/count.c`]
- `fork`: crea un processo figlio che continuerà l'esecuzione del codice dalla chiamata della funzione in poi. Ha come valore di ritorno 0 per il figlio e il pId del figlio per il padre. [`/2022-07-15/es2/autolancia.c`, `/2019-09-18/es1/para.c`, `/2019-09-18/es2/paran3.c`, `/2018-05-29/es1/pcmp.c`, `/2018-05-29/es1/pcmp.c`, `/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`, `/2023-01-19/es1/stdin2pipe.c`, `/2023-01-19/es2/stdin2pipe.c`, `/2022-09-07/es1/rilancia.c`, `/2022-09-07/es2/rilancia.c`]
- `wait`: attende la terminazione di un processo filgio e ritorna il numero di figli ancora non terminati. [`/2019-09-18/es1/para.c`, `/2019-09-18/es2/paran3.c`, `/2018-05-29/es1/pcmp.c`, `/2018-05-29/es1/pcmp.c`, `/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`]
- `waitpid`: variante di `wait`, attende la terminazione di un processo dato il pid. Ritorna uno stato. Corollario: per la valutazione dello stato di ritorno esistono funzioni utili come `WIFEXITED` (per informazioni dettagliate leggere il manuale). [`/2021-07-15/es2/autolancia.c`, `/2018-07-18/es2/count.c`, `/2023-01-19/es1/stdin2pipe.c`, `/2023-01-19/es2/stdin2pipe.c`, `/2022-09-07/es1/rilancia.c`, `/2022-09-07/es2/rilancia.c`]
- `remove`: un'interfaccia unificata per `unlink`(rimuove il nome di un file dal file system) e `rmdir`(rimuove una directory). [`/2020-02-21/es1/abslink.c`, `/2023-01-23/es2/fifosig.c`, `/2022-01-18/es1/abssymlink.c`, `/2022-01-18/es2/cpsymlink.c`, `/2022-02-17/es1/nonesr_symlink.c`, `/2022-02-17/es2/nest2hard.c`, `/2022-06-08/es1/invsymlink.c`, `/2022-06-08/es2/invsymlink.c`, `/2021-09-16/es1/execname.c`]
- `symlink`: crea un link simbolico di un file con un nome specificato. Corollario: esiste anche `link` che fa la stessa identica cosa, ma crea un link fisico. [`/2020-02-21/es1/symlink.c`, `/2022-01-18/es1/abssymlink.c`, `/2022-01-18/es1/abssymlink.c`, `/2022-06-08/es1/invsymlink.c`, `/2022-06-08/es2/invsymlink.c`]
- `link`: creazione di un hard link. [`/2022-02-17/es2/nest2hard.c`, `/2022-06-08/es1/invsymlink.c`, `/2022-06-08/es2/invsymlink.c`]
- `sigemptyset`: rimuove l'handling di tutti i segnali da un set(di tipo `sigset_t`) passato in input(una maschera). Corollario: vedere altri comandi sui segnali e il manuale. [`/2018-07-18/es1/sigcounter.c`, `/2023-01-23/es2/test.c`, `/2022-06-22/es1/es1/rx.c`]
- `sigaddset`: aggiunge un segnale al set(di tipo `sigset_t`) passato in input(una maschera). Corollario: vedere altri comandi sui segnali e il manuale. [`/2018-07-18/es1/sigcounter.c`, `/2023-01-23/es2/test.c`]
- `signalfd`: crea un file descriptor che può ricevere segnali per conto del processo. Parametri: un file descriptor(se -1 ne viene creato uno), set(di tipo `sigset_t`) di segnali accettati(una maschera creata con le apposite chiamate a funzioni `sigaddset`, `sigemptyset`, `sigdelset`, etc...) e le flags(in genere si può inizializzare a 0). Corollario: vedere altri comandi sui segnali e il manuale. [`/2018-07-18/es1/sigcounter.c`, `/2023-01-23/es2/test.c`]
- `sigqueue`: invio di un segnale a un altro processo con l'aggiunta di un messaggio. Corollario: questo sistema permette di scambiare anche tutte le informazioni di contesto per permettere all processo ricevente di ottenere il messaggio scambiato. [`/2022-06-22/es1/tx.c`]
- `sigaction`: assegnamento di un azione specifica all'arrivo di un segnale. [`/2022-06-22/es1/rx.c`]
- `sigaltstack`: scambio di stack. Corollario: utilizzato per condividere memoria tra i provessi che si scambiano messaggi. [`/2022-06-22/es1/rx.c`]
- `open`: restituisce il file descriptor di un file. Corollario: esistono funzioni alcune varianti come `fopen` e `openat`. [`/2018-05-29/es1/pcmp.c`, `/2018-05-29/es1/pcmp.c`, `/2023-01-23/es1/fifotext.c`, `/2023-01-23/es2/fifosig.c`, `/2023-01-23/es2/test.c`, `/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`, `/2022-07-22/es1/tree.c`, `/2023-05-31/es1/pidcmd.c`, `/2023-05-31/es1/pidcmd2.c`, `/2023-05-31/es2/pidrerun.c`]
- `lseek`: posiziona il file descriptor un file aperto all'offset specificato. Corollario: si usano anche funzioni come `fseek`. [`/2018-05-29/es1/pcmp.c`, `/2018-05-29/es1/pcmp.c`, `/2023-01-23/es1/fifotext.c`, `/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`]
- `read`: lettura di un file descriptor e inserimento dei dati in un buffer con una grandezza specificata. Corollario: utilizzato in situazioni particolari come lettura di un file descriptor che lavora come un collettore di segnali(con l'utilizzo di un buffer di tipo `struct signalfd_siginfo`). [`/2018-07-18/es2/sigcount.c`, `/2023-01-23/es1/fifotext.c`, `/2023-01-23/es2/test.c`, `/2023-01-23/es2/fifosig.c`, `/2023-01-19/es1/stdin2pipe.c`, `/2023-01-19/es2/stdin2pipe.c`, `/2023-05-31/es1/pidcmd.c`, `/2023-05-31/es1/pidcmd2.c`, `/2023-05-31/es2/pidrerun.c`, `/2021-09-16/es1/execname.c`, `/2021-09-16/es2/execname2.c`]
- `write`: write a buffer on a file descriptor. [`/2023-01-23/es2/test.c`, `/2023-01-19/es1/stdin2pipe.c`, `/2023-01-19/es2/stdin2pipe.c`]
- `kill`: manda segnali ai processi tra cui quello per la terminazione. Corollario: esistono costanti per l'identificazione dei segnali come `SIGKILL`, `SIGUSR1`, `SIGUSR2`, etc... [`/2018-07-18/es2/count.c`, `/2018-05-29/es1/pcmp.c`, `/2018-05-29/es1/pcmp.c`, `/2023-01-23/es2/fifosig.c`, `/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`]
- `realpath`: ritorna il path assoluto di un file. Risolve inoltre i link simbolici. Corollario: non è una system call, ma è spesso utile :). [`/2020-02-21/es1/abslink.c`, `/2020-02-21/es2/absls.c`, `/2022-01-18/es1/abssymlink.c`, `/2022-01-18/es2/cpsymlink.c`, `/2022-06-08/es1/invsymlink.c`, `/2022-06-08/es2/invsymlink.c`]
- `readdir`: legge una per volta le entry di una directory. Il formato di lettura è `struct dirent` che contiene inode, tipo, nome e altre informazioni sul file. Usata con `opendir`, `closedir` per aprire e chiudere la directory. Corollario: ne esiste anche la versione con gestione tramite file descriptor, `fdopendir`. Anche questa non è una system call, ma è spesso utile :P. [`/2020-02-21/es2/absls.c`, `/2022-01-18/es1/abssymlink.c`, `/2022-01-18/es2/cpsymlink.c`, `/2022-07-22/es1/tree.c`, `/2023-05-31/es1/pidcmd.c`, `/2023-05-31/es1/pidcmd2.c`, `/2022-02-17/es1/nonesr_symlink.c`, `/2022-02-17/es2/nest2hard.c`, `/2022-06-08/es2/invsymlink.c`]
- `memcmp`: compara la memoria byte per byte. Corollario: utile a gestire stringhe che contengono caratteri come `\0`. [`/2023-05-31/es1/pidcmd2.c`]
- `memcpy`: copia la memoria byte per byte. Corollario: utile a gestire stringhe che contengono caratteri come `\0`. [`/2023-05-31/es1/pidcmd2.c`]
- `mkfifo`: creazione di una named pipe. [`/2023-01-23/es1/fifotext.c`, `/2023-01-23/es2/fifosig.c`]
- `pipe`: creazione di una pipe. [`/2023-01-19/es1/stdin2pipe.c`, `/2023-01-19/es2/stdin2pipe.c`]
- `fgets`: lettura attraverso buffer; come gets, ma sicura. [`/2022-01-18/es2/cpsymlink.c`]
- `creat`: creazione di un file. [`/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`]
- `truncate`: dimensionamento di un file. [`/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`]
- `fread`: lettura di byte da una stream. [`/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`]
- `fwrite`: scrittura di byte su una stream. [`/2023-02-16/es1/pcp.c`, `/2023-02-16/es2/pcp.c`]
- `dup2`: duolica un file descriptor. [`/2023-01-19/es1/stdin2pipe.c`, `/2023-01-19/es2/stdin2pipe.c`, `/2021-09-16/es2/execname2.c`]
- `time`: restituzione dei secondi passati da Epoch(primogennaiomillenovecentoblablabla...). [`/2022-09-07/es2/rilancia.c`]
- `timerfd_create`: creazione del file descriptor di un timer. Corollario: utilizzato assieme a `timerfd_settime`, `struct itimerspec` e altre primitive o strutture che gestiscono il timer. [`/2023-06-14/es1/reprint.c`, `/2023-06-14/es2/multireprint.c`]
- `timerfd_settime`: settagio del timer identificato dal file descriptor. All'esecuzione della funzione il timer sarà avviato. Corollario: i parametri più importanti sono il descrittore del timer da inizializzare e l'impostazione del timer di tipo `struct itimerspec` a sua volta composta da una struttura di tipo `struct timespec`, `it_value` rappresenta il primo scadere del timer, `it_interval` il periodo delle scadenze successive. [`/2023-06-14/es1/reprint.c`, `/2023-06-14/es2/multireprint.c`]
- `clock_gettime`: restituzione di un tempo in secondi e in nanosecondi. Consiglio: nell'esercizio `/2023-06-14/es1/reprint.c` c'è una funzione wrapper che permette di avere il tempo passato dalla prima chiamata. [`/2023-06-14/es1/reprint.c`, `/2023-06-14/es2/multireprint.c`]
- `poll`: monitoraggio di più file descriptor contemporaneamente. Corollario: fa uso della struttura dati `struct pollfd` nella quale inserire i file descriptor, i tipi di eventi da monitorare e leggere l'evento accaduto. La il parametro `timeout` può essere settato al valore `-1` per far in modo che la chiamata sia completamente bloccante. [`/2023-06-14/es2/multireprint.c`]
- `readlink`: restituzione del file a cui punta un link simbolico; come un realpath che però segue solo il primo salto del link. [`/2022-02-17/es1/nonesr_symlink.c`, `/2022-02-17/es2/nest2hard.c`, `/2022-06-08/es1/invsymlink.c`, `/2022-06-08/es2/invsymlink.c`]
- `stat`: restituzione di informazioni dello stato di un file. Corollario: esistono varianti come `lstat` che non segue i link simbolici. Si utilizzano delle macto per analizzare il contenuto di alcuni campi come `S_ISLNK` o `S_ISDIR`. [`/2022-02-17/es1/nonesr_symlink.c`, `/2022-02-17/es2/nest2hard.c`, `/2022-06-08/es1/invsymlink.c`, `/2022-06-08/es2/invsymlink.c`]
- `inotify__`: insieme di primitive per il monitoraggio di elementi del file system. Corollario: possono essere creati eventi bloccanti e non bloccanti. `read` sul file descriptor restituito dall'inotify permette di mettersi in ascolto di un questi eventi. [`/2021-09-16/es1/execname.c`, `/2021-09-16/es2/execname2.c`]

# System Call e funzioni utili usate in python
- `os.stat`: ritorna la stat del file nel path specificato. Corollario: esistono alcune funzioni per indagare il tipo di file della stat, come `S_ISDIR`, nel modulo `stat`. [`/2021-09-18/es3/searchln`, `/2019-07-15/es3/ultimoPrimoModificato`, `/2022-01-18/es3/patternInSubtree`, `/2023-05-31/es3/sameFile`, `/2022-02-17/es3/treeGrep`, `/2022-06-08/es3/myProcOccupation`]
- `os.listdir`: elenca le entry di una directory. [`/2019-09-18/es3/searchln`, `/2019-07-15/es3/ultimoPrimoModificato`, `/2018-05-29/es3/usrProcCounter`, `/2023-01-23/es3/difdr`, `/2022-01-18/es3/patternInSubtree`, `/2023-02-16/es3/ccpl`, `/2022-09-07/es3/elfWidth`, `/2022-07-22/es3/execNoElf`, `/2022-06-22/es3/category`, `/2023-05-31/es3/sameFile`, `/2023-06-14/es3/rmRelativeSymlink`, `/2022-02-17/es3/treeGrep`, `/2022-06-08/es3/myProcOccupation`, `/2021-09-16/es3/rlsr`]
- `os.path.is_`: insieme di funzioni per distinguere il tipo di file dato il path; `abs`, `dir`, `file`, `link`, etc... [`/2020-02-21/es3/listingDir`, `/2018-05-29/es3/usrProcCounter`, `/2023-01-23/es3/difdr`, `/2023-01-19/es3/mergeDir`, `/2022-09-07/es3/elfWidth`, `/2022-07-22/es3/execNoElf`, `/2022-06-22/es3/category`, `/2023-05-31/es3/sameFile`, `/2023-06-14/es3/rmRelativeSymlink`, `/2022-02-17/es3/treeGrep`, `/2021-09-16/es3/rlsr`]
- `os.path.splitext`: splitta l'estensione dal path. [`/2020-02-21/es3/listingDir`]
- `os.path.split`: splitta il path in file e directory path. [`/2022-06-08/es3/myProcOccupation`]
- `os.path.join`: molto utile a concatenare il path ti una directory al nome di un file. [`/2019-09-18/es3/searchln`, `/2019-09-18/es3/searchln`, `/2019-07-15/es3/ultimoPrimoModificato`, `/2018-05-29/es3/usrProcCounter`, `/2023-01-23/es3/difdr`, `/2022-01-18/es3/patternInSubtree`, `/2022-09-07/es3/elfWidth`, `/2022-07-22/es3/execNoElf`, `/2022-06-22/es3/category`, `/2023-05-31/es3/sameFile`, `/2023-06-14/es3/rmRelativeSymlink`, `/2022-02-17/es3/treeGrep`, `/2022-06-08/es3/myProcOccupation`, `/2021-09-16/es3/rlsr`]
- `os.path.exists`: verifica se esiste il path passato come parametro. [`/2023-01-23/es3/difdr`, `/2022-06-08/es3/myProcOccupation`]
- `os.path.dirname`: restituisce la parte di path in una stringa che rappresenta a sua volta un path. [`/2023-01-23/es3/difdr`]
- `os.path.getsize`: restituisce la dimensione di un file in byte. [`/2022-09-07/es3/elfWidth`]
- `os.readlink`: restituisce il path indicato da un link. [`/2023-06-14/es3/rmRelativeSymlink`, `/2022-06-08/es3/myProcOccupation`]
- `os.remove`: rimozione del file indicato. [`/2023-06-14/es3/rmRelativeSymlink`]
- `shutil.copy`: creazione della copia di un file. [`/2023-01-23/es3/difdr`, `/2023-01-19/es3/mergeDir`]
- `shutil.copytree`: creazione della copia di un intero sottoalbero. Corollario: è possibile personalizzare la funzione di copia. [`/2023-01-19/es3/mergeDir`]
- `os.makedirs`: creazione di una directory. [`/2023-01-23/es3/difdr`]
- `os.getcwd`: ritorna il nome della directory nella quale viene lanciato l'eseguibile. [`/2022-01-18/es3/patternInSubtree`, `/2022-09-07/es3/elfWidth`, `/2022-07-22/es3/execNoElf`, `/2023-05-31/es3/sameFile`]
- `open`: apertura di un file, simile a C. Corollario: spesso utilizzata assieme a `read` che legge l'intero contenuto o `readlines` che ritorno aun array con tutte le linee del file. [`/2022-01-18/es3/patternInSubtree`, `/2023-02-16/es3/ccpl`, `/2023-01-19/es3/mergeDir`, `/2022-09-07/es3/elfWidth`, `/2022-07-22/es3/execNoElf`, `/2022-02-17/es3/treeGrep`, `/2022-06-08/es3/myProcOccupation`]
- `sys.stdin`: lettura diretta dello standard input. [`/2018-07-18/es3/minimalShell`]
- `subprocess.run`: esecuzione di un comando shell. [`/2018-07-18/es3/minimalShell`, `/2022-07-22/es3/execNoElf`]
- `binascii.hexlify`: rappresentazione ascii del binario. [`/2022-09-07/es3/elfWidth`, `/2022-07-22/es3/execNoElf`]
- `magic.from_file`: restituisce il tipo del file passato. [`/2022-06-22/es3/category`]
- `getpass.getuser`: restituisce il nome dell'utente che ha eseguito lo script. [`/2022-06-08/es3/myProcOccupation`]
- `pwd.getpwuid`: restituisce il nome dell'utente proprietario di un file. [`/2022-06-08/es3/myProcOccupation`]

## Domande
- Come riconoscere un file eseguibile da una libreria shared? [`/2021-07-15/es2/autolancia.c`] [Soluzione](https://github.com/cosimopp/prove-pratiche-so/blob/main/2021.07.15/ex1.c): `exec()`: ritorna solo se è avvenuto un errore; se il parametro sara una libreria shared allora si verificherà un errore. **Soluzione fornita dal prof**: "utilizzare fantasia e soluzioni proposte online"; in particolare in aula ha proposto l'approccio contrario, ovvero provare ad aprire il file come libreria shared e, se nel caso non funzioni, eseguire come normale eseguibile.
