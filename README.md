# Esercizi-pratici-so
Esercizi di preparazione alla prova pratica dell'esame di sistemi operativi

# System Call usate
- `dlopen`: apertura di una shared lib, ritorna un handler. Corollario: `dlsym`(passato l'handler e un simbolo ne ritorna la funzione associata) e `dlerror`(ritorna un errore o NULL). [`/2021-07-15/es1/lancia.c`]
