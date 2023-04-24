# Esercizi-pratici-so
Esercizi di preparazione alla prova pratica dell'esame di sistemi operativi

# System Call usate
- `dlopen`: apertura di una shared lib, ritorna un handler. Corollario: `dlsym`(passato l'handler e un simbolo ne ritorna la funzione associata), `dlerror`(ritorna un errore o NULL) e `dlclose`(performa la corretta chiusura della shared lib). [`/2021-07-15/es1/lancia.c`, `/2021-07-15/es2/autolancia.c`]
- `execv`: esegue un comando passato come parametro con una lista di parametri. Corollario: esistono altre varianti della funzione `exec` (`execv`, `execve`, `execvp`, `execl`, `execlp`, ...), per informazioni dettagliate leggere il manuale. [`/2021-07-15/es2/autolancia.c`]
- `fork`: crea un processo figlio che continuerà l'esecuzione del codice dalla chiamata della funzione in poi. [`/2021-07-15/es2/autolancia.c`]
- `waitpid`: variante di `wait`, attende la terminazione di un processo dato il pid. Ritorna uno stato. Corollario: per la valutazione dello stato di ritorno esistono funzioni utili come `WIFEXITED` (per informazioni dettagliate leggere il manuale). [`/2021-07-15/es2/autolancia.c`]

# Domande
- Come riconoscere un file eseguibile da una libreria shared? [`/2021-07-15/es2/autolancia.c`] [Soluzione](https://github.com/cosimopp/prove-pratiche-so/blob/main/2021.07.15/ex1.c): `exec()`: ritorna solo se è avvenuto un errore; se il parametro sara una libreria shared allora si verificherà un errore.
