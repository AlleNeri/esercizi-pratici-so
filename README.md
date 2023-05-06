# Esercizi-pratici-so
Esercizi di preparazione alla prova pratica dell'esame di sistemi operativi. [Testi delle prove](http://www.cs.unibo.it/~renzo/so/provapratica.shtml) qui svolte e di mooolte altre.

# System Call usate
- `dlopen`: apertura di una shared lib, ritorna un handler. Corollario: `dlsym`(passato l'handler e un simbolo ne ritorna la funzione associata), `dlerror`(ritorna un errore o NULL) e `dlclose`(performa la corretta chiusura della shared lib). [`/2021-07-15/es1/lancia.c`, `/2021-07-15/es2/autolancia.c`]
- `execv`: esegue un comando passato come parametro con una lista di parametri. Corollario: esistono altre varianti della funzione `exec` (`execv`, `execve`, `execvp`, `execl`, `execlp`, ...), per informazioni dettagliate leggere il manuale. [`/2021-07-15/es2/autolancia.c`]
- `execvp`: esegue un comando con parametri nel formato: parametro1=comando da eseguire, parametro2=array argv(col comando stesso come primo elemento e possibilmente NULL come ultimo, ma non sembra catastrofico non farlo). Corollario: vedere `execv`. [`/2019-09-18/es1/para.c`, `/2019-09-18/es2/paran3.c`]
- `fork`: crea un processo figlio che continuerà l'esecuzione del codice dalla chiamata della funzione in poi. [`/2021-07-15/es2/autolancia.c`, `/2019-09-18/es1/para.c`, `/2019-09-18/es2/paran3.c`]
- `wait`: attende la terminazione di un processo filgio e ritorna il numero di figli ancora non terminati. [`/2019-09-18/es1/para.c`, `/2019-09-18/es2/paran3.c`]
- `waitpid`: variante di `wait`, attende la terminazione di un processo dato il pid. Ritorna uno stato. Corollario: per la valutazione dello stato di ritorno esistono funzioni utili come `WIFEXITED` (per informazioni dettagliate leggere il manuale). [`/2021-07-15/es2/autolancia.c`]
- `remove`: un'interfaccia unificata per `unlink`(rimuove il nome di un file dal file system) e `rmdir`(rimuove una directory). [`2020-02-21/es1/abslink.c`]
- `symlink`: crea un link simbolico di un file con un nome specificato. Corollario: esiste anche `link` che fa la stessa identica cosa, ma crea un link fisico. [`2020-02-21/es1/symlink.c`]
- `realpath`: ritorna il path assoluto di un file. Risolve inoltre i link simbolici. Corollario: non è una system call, ma è spesso utile :). [`2020-02-21/es1/abslink.c`, `2020-02-21/es2/absls.c`]
- `readdir`: legge una per volta le entry di una directory. Il formato di lettura è `struct dirent` che contiene inode, tipo, nome e altre informazioni sul file. Usata con `opendir`, `closedir` per aprire e chiudere la directory. Corollario: ne esiste anche la versione con gestione tramite file descriptor, `fdopendir`. Anche questa non è una system call, ma è spesso utile :P. [`2020-02-21/es2/absls.c`]

# Domande
- Come riconoscere un file eseguibile da una libreria shared? [`/2021-07-15/es2/autolancia.c`] [Soluzione](https://github.com/cosimopp/prove-pratiche-so/blob/main/2021.07.15/ex1.c): `exec()`: ritorna solo se è avvenuto un errore; se il parametro sara una libreria shared allora si verificherà un errore. **Soluzione fornita dal prof**: "utilizzare fantasia e soluzioni proposte online"; in particolare in aula ha proposto l'approccio contrario, ovvero provare ad aprire il file come libreria shared e, se nel caso non funzioni, eseguire come normale eseguibile.
