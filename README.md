# Esercizi-pratici-so
Esercizi di preparazione alla prova pratica dell'esame di sistemi operativi

# System Call usate
- `dlopen`: apertura di una shared lib, ritorna un handler. Corollario: `dlsym`(passato l'handler e un simbolo ne ritorna la funzione associata) e `dlerror`(ritorna un errore o NULL). [`/2021-07-15/es1/lancia.c`, `/2021-07-15/es2/autolancia.c`]

# Domande
- Come riconoscere un file eseguibile da una libreria shared? [`/2021-07-15/es2/autolancia.c`] [Soluzione](https://github.com/cosimopp/prove-pratiche-so/blob/main/2021.07.15/ex1.c): `exec()`: ritorna solo se è avvenuto un errore; se il parametro sara una libreria shared allora si verificherà un errore. Anche se a me non funzione.
