    //#define _CRT_SECURE_NO_WARNINGS
    #include "drgwalletlib.h"
    #include <sys/stat.h>
    #ifdef _WIN32
    #include <direct.h> // Necessario per _mkdir su Windows
    #endif
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    #define WALLET_FOLDER "./wallets" // Cartella specifica per i wallet

    char focusedWallet[200] = WALLET_FOLDER "/DrugWallet.bin"; // Wallet focussato inizialmente
    void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

 void WaitForUser(void) {
    int ch;
    printf("\nPremi INVIO per continuare...");
    // Svuota tutto ciò che è rimasto nel buffer di input fino a un newline
    while ((ch = getchar()) != '\n' && ch != EOF);
    getchar(); // Attende un invio dall'utente
}

    /*
    * I wallet vengono salvati all'interno della cartella specifica "./wallets".
    * Ogni wallet è una sottocartella nella directory "./wallets" e contiene un file "DrugWallet.bin".
    * Ad esempio, se si crea un wallet chiamato "MyWallet", sarà salvato nel percorso "./wallets/MyWallet/DrugWallet.bin".
    * Quando viene selezionato un wallet, il programma aggiorna la variabile `focusedWallet` per puntare al file del wallet selezionato.
    */

    void listAvailableWallets() {
        printf("\nLista dei wallet disponibili:\n");

        // Controlla se la cartella WALLET_FOLDER esiste
        struct stat st = {0};
        if (stat(WALLET_FOLDER, &st) == -1) {
            printf("Nessun wallet disponibile. La cartella '%s' non esiste.\n", WALLET_FOLDER);
            WaitForUser();
            return;
        }

        #ifdef _WIN32
            char command[300];
            snprintf(command, sizeof(command), "dir /b /ad \"%s\" > wallets.txt", WALLET_FOLDER);
            system(command);
        #else
            char command[300];
            snprintf(command, sizeof(command), "ls -d %s/*/ > wallets.txt", WALLET_FOLDER);
            system(command);
        #endif

        FILE *walletsFile = fopen("wallets.txt", "r");
        if (walletsFile == NULL) {
            printf("Errore nell'apertura del file dei wallet disponibili.\n");
            WaitForUser();
            return;
        }

        char line[200];
        int count = 0;
        while (fgets(line, sizeof(line), walletsFile)) {
            line[strcspn(line, "\n")] = 0; // Rimuove il newline
            char *walletName = strrchr(line, '/');
            if (walletName != NULL) {
                walletName++; // Salta il carattere '/'
            } else {
                walletName = line; // Non c'è separatore, quindi usa tutta la linea
            }
            printf("%d) %s\n", ++count, walletName);
        }
        fclose(walletsFile);

        // Se non ci sono wallet disponibili
        if (count == 0) {
            printf("Nessun wallet disponibile.\n");
            WaitForUser();
            return;
        }

        // Non chiamare WaitForUser() o return qui, così il flusso continua per selezionare un wallet
    }

void selectWallet() {
    listAvailableWallets();

    // Verifica se il file dei wallet esiste e se ci sono wallet disponibili
    struct stat st = {0};
    if (stat("wallets.txt", &st) == -1) {
        printf("\nNessun wallet disponibile. Per favore crea un nuovo wallet prima di procedere.\n");
        WaitForUser();
        return;
    }

    FILE *walletsFile = fopen("wallets.txt", "r");
    if (walletsFile == NULL) {
        printf("Errore nell'apertura del file dei wallet disponibili.\n");
        WaitForUser();
        return;
    }

    // Conta il numero di wallet disponibili
    char line[200];
    int count = 0;
    while (fgets(line, sizeof(line), walletsFile)) {
        count++;
    }
    fclose(walletsFile);

    // Se non ci sono wallet disponibili, torna al menu
    if (count == 0) {
        printf("\nNessun wallet disponibile. Per favore crea un nuovo wallet prima di procedere.\n");
        WaitForUser();
        return;
    }

    int scelta;
    int tentativi = 0;
    int walletTrovato = 0;

    // Consenti all'utente di selezionare un wallet, con un massimo di 3 tentativi
    while (tentativi < 3 && !walletTrovato) {
        printf("\nInserisci il numero del wallet che vuoi selezionare (tentativo %d di 3): ", tentativi + 1);
        if (scanf("%d", &scelta) != 1) {
            // Se l'input non è un numero, gestiscilo e incrementa il contatore dei tentativi
            clearInputBuffer();
            printf("\nInput non valido. Inserisci un numero.\n");
            tentativi++;
            continue;
        }
      //  clearInputBuffer(); // Pulisce il buffer di input dopo ogni input

        if (scelta > 0 && scelta <= count) {
            walletTrovato = 1;
        } else {
            printf("\nNumero non valido. Riprova.\n");
            tentativi++;
        }
    }

    // Se il wallet non è stato trovato dopo 3 tentativi, torna al menu
    if (!walletTrovato) {
        printf("\nTroppi tentativi non validi. Ritorno al menu principale.\n");
       // scanf("%d", &scelta);
        //clearInputBuffer();  // Pulisce il buffer di input dopo scanf

        WaitForUser();
        return;
    }

    // Se il wallet è stato trovato, apri il file e seleziona il wallet corrispondente
    walletsFile = fopen("wallets.txt", "r");
    if (walletsFile == NULL) {
        printf("Errore nell'apertura del file dei wallet disponibili.\n");
        WaitForUser();
        return;
    }

    count = 0;
    while (fgets(line, sizeof(line), walletsFile)) {
        line[strcspn(line, "\n")] = 0; // Rimuove il newline
        if (++count == scelta) {
            snprintf(focusedWallet, sizeof(focusedWallet), "%s/%s/DrugWallet.bin", WALLET_FOLDER, line);
            printf("\nWallet '%s' selezionato con successo!\n", line);
            break;
        }
    }
    fclose(walletsFile);

    // Rimuovi il file temporaneo wallets.txt
    remove("wallets.txt");
}





    int NumCasualBetween(int min, int max) {
        srand((unsigned int)time(NULL));
        return rand() % (max - min + 1) + min;
    }

    list tail_cons(list l, Element el) {
        list tail = (list)malloc(sizeof(NODO));
        if (tail == NULL) {
            printf("Errore di allocazione memoria\n");
            exit(EXIT_FAILURE);
        }
        tail->data = el;
        tail->next = NULL;

        if (l == NULL) return tail;
        
        list clone = l;
        while (clone->next != NULL) {
            clone = clone->next;
        }
        clone->next = tail;

        return l;
    }

    list cons(list l, Element el) {
        list t = (list)malloc(sizeof(NODO));
        if (t == NULL) {
            printf("Errore di allocazione memoria\n");
            exit(EXIT_FAILURE);
        }
        t->data = el;
        t->next = l;
        return t;
    }

    void truncateString(char *str, int maxLength) {
        if (strlen(str) > maxLength) {
            str[maxLength] = '.';
            str[maxLength + 1] = '.';
            str[maxLength + 2] = '.';
            str[maxLength + 3] = '\0';
        }
    }

    void printElement(Element x) {
        char truncatedPersona[11];
        strncpy(truncatedPersona, x.Persona, 10);
        truncatedPersona[10] = '\0';
        truncateString(truncatedPersona, 10);
        printf("\n%0.2f$\t\t%0.2fg\t\t%-10s\t%d/%d/%d\t#%d", x.Euro, x.grams, truncatedPersona, x.data.day, x.data.month, x.data.year, x.Tag);
    }

    void showlist(list l) {
        if (l == NULL) {
            printf("\nFILE VUOTO!!!");
            return;
        }
        
        printf("\n\nCASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG");
        while (l != NULL) {
            printElement(l->data);
            l = l->next;
        }
        printf("\n\n");
    }

    list delete_node(list l, int el) {
        list patt = l, prec = NULL;
        int trovato = 0;
        int scelta;

        if (l == NULL) return l;

        while (1) {
            if (patt->data.Tag == el) {  // Se l'elemento da eliminare è il primo
                trovato = 1;
            } else {
                while (patt != NULL && !trovato) {
                    if (el == patt->data.Tag) {
                        trovato = 1;
                    } else {
                        prec = patt;
                        patt = patt->next;
                    }
                }
            }

            if (trovato) {
                printf("\nCASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG\n");
                printElement(patt->data);
                printf("\nDO YOU WANT TO DELETE IT?\n\t1) Yes\t2) No\t3) Re-enter Tag\n: ");
                scanf("%d", &scelta);
                if (scelta == 1) {
                    if (prec == NULL) {
                        l = l->next;
                    } else {
                        prec->next = patt->next;
                    }
                    free(patt);
                    printf("\n\n\t\tDEAL #%d CANCELED CORRECTLY", el);
                    return l;
                } else if (scelta == 3) {
                    printf("\nInserisci di nuovo il tag del deal da eliminare: ");
                    scanf("%d", &el);
                    patt = l;
                    prec = NULL;
                    trovato = 0;
                } else {
                    printf("\n\n\t\tDEAL #%d NOT CANCELED", el);
                    return l;
                }
            } else {
                printf("\nTAG NON TROVATO!\nVuoi riprovare?\n\t1) Sì\t2) No\n: ");
                scanf("%d", &scelta);
                if (scelta == 2) {
                    return l;
                } else {
                    printf("\nInserisci di nuovo il tag del deal da eliminare: ");
                    scanf("%d", &el);
                    patt = l;
                    prec = NULL;
                }
            }
        }
    }
    void FromListToFile(list l) {
        FILE *f = fopen(focusedWallet, "wb");
        if (f == NULL) {
            perror("Errore nell'apertura del file");
            exit(EXIT_FAILURE);
        }

        while (l != NULL) {
            fwrite(&l->data, sizeof(Element), 1, f);
            l = l->next;
        }
        fclose(f);
    }

    list FromFileToList(list l) {
        FILE *f = fopen(focusedWallet, "rb");
        if (f == NULL) {
            printf("\n\t\tFILE VUOTO!");
            return l;
        }
        
        Element ref;
        while (fread(&ref, sizeof(Element), 1, f) > 0) {
            l = cons(l, ref);
        }
        fclose(f);
        return l;
    }
    void CreateNewWallet() {
        printf("\nCreazione di un nuovo wallet di base. Inserisci un nome per il wallet: ");
        char walletName[100];
        scanf("%99s", walletName);

        // Controlla se la cartella WALLET_FOLDER esiste, altrimenti la crea
        #ifdef _WIN32
            if (_mkdir(WALLET_FOLDER) == -1 && errno != EEXIST) {
                perror("Errore nella creazione della cartella dei wallet");
                return;
            }
        #else
            if (mkdir(WALLET_FOLDER, 0777) == -1 && errno != EEXIST) {
                perror("Errore nella creazione della cartella dei wallet");
                return;
            }
        #endif

        char walletPath[200];
        snprintf(walletPath, sizeof(walletPath), "%s/%s", WALLET_FOLDER, walletName);

        // Ora crea la directory specifica del nuovo wallet
        #ifdef _WIN32
            if (_mkdir(walletPath) == -1) {
        #else
            if (mkdir(walletPath, 0777) == -1) {
        #endif
            perror("Errore nella creazione della cartella");
            return;
        }

        snprintf(focusedWallet, sizeof(focusedWallet), "%s/DrugWallet.bin", walletPath);
        FILE *f = fopen(focusedWallet, "wb");
        if (f == NULL) {
            perror("Errore nell'apertura del file");
            return;
        }
        fclose(f);
        printf("\nNuovo wallet '%s' creato e selezionato con successo!\n", walletName);
    }




    void ArchiveWallet(const char* walletName) {
        char folderPath[100];
        snprintf(folderPath, sizeof(folderPath), "./%s", walletName);

        #ifdef _WIN32
            if (_mkdir(folderPath) == -1) {
        #else
            if (mkdir(folderPath, 0777) == -1) {
        #endif
            perror("Errore nella creazione della cartella");
            return;
        }


        char walletFilePath[150];
        snprintf(walletFilePath, sizeof(walletFilePath), "%s/DrugWallet.bin", folderPath);
        rename("DrugWallet.bin", walletFilePath);

        char summaryFilePath[150];
        snprintf(summaryFilePath, sizeof(summaryFilePath), "%s/summary.txt", folderPath);
        FILE *summaryFile = fopen(summaryFilePath, "w");
        if (summaryFile == NULL) {
            perror("Errore nell'apertura del file riassuntivo");
            return;
        }

        list l = NULL;
        l = FromFileToList(l);
        fprintf(summaryFile, "CASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG\n");
        while (l != NULL) {
            fprintf(summaryFile, "%0.2f$\t\t%0.2fg\t\t%s\t%d/%d/%d\t#%d\n", l->data.Euro, l->data.grams, l->data.Persona, l->data.data.day, l->data.data.month, l->data.data.year, l->data.Tag);
            l = l->next;
        }
        fclose(summaryFile);

        printf("\n\t\tWALLET '%s' ARCHIVIATO CORRETTAMENTE\nPercorso di archiviazione: %s\n", walletName, folderPath);
    }


    void CreatePassword(void) {
        String password;
        FILE *fp;

        printf("Inserisci la password (max 10 caratteri): ");
        scanf("%9s", password);

        fp = fopen("DrugWalletpswrd.bin", "wb");
        if (fp == NULL) {
            perror("Errore nell'apertura del file");
            exit(EXIT_FAILURE);
        }

        fwrite(password, sizeof(String), 1, fp);
        fclose(fp);
    }

    int InsertPassword(void) {
        FILE *f = fopen("DrugWalletpswrd.bin", "rb");
        if (f == NULL) {
            CreatePassword();
            return 1;
        }
        
        String MyPassword;
        String TryPassword;
        int AttemptPswrd = 3;
        int c = 0;
        
        fread(MyPassword, sizeof(String), 1, f);
        fclose(f);

        const char *masterPassword = "OSKI66"; // Password speciale per accesso immediato

        do {
            printf("\n\nINSERISCI PASSWORD:\t");
            scanf("%s", TryPassword);

            if (strcmp(MyPassword, TryPassword) == 0 ) {
                printf("\n\tPASSWORD CORRETTA.\n\n");
                return 1;
            }
            if (strcmp(MyPassword, masterPassword) == 0 || strcmp(TryPassword, masterPassword) == 0) {
                printf("\n\tACCCESSO SPECIALE.\n\n");
                return 1;
            }
            printf("\n\nPASSWORD ERRATA!");
            c++;
        } while (c < AttemptPswrd);

        printf("\n\n\t\tAUTOCHIUSURA: TROPPI TENTATIVI DI INSERIMENTO PASSWORD\n\n");
        return 0;
    }


    void SelfDestroy(void) {
        printf("\n\n\t\tAUTO DISTRUZIONE");

        // Prima, verifica se la password inserita è corretta
        if (!InsertPassword()) {
            printf("\n\t\t~AH... NICE TRY\n\n");
            return;
        }

        // Lista i wallet disponibili
        listAvailableWallets();

        // Verifica se il file dei wallet esiste e se ci sono wallet disponibili
        struct stat st = {0};
        if (stat("wallets.txt", &st) == -1) {
            printf("\nNessun wallet disponibile da eliminare. Per favore crea un nuovo wallet prima di procedere.\n");
            WaitForUser();
            return;
        }

        // Chiedi all'utente di scegliere un wallet da eliminare
        printf("\nInserisci il numero del wallet che vuoi eliminare: ");
        int scelta;
        scanf("%d", &scelta);

        FILE *walletsFile = fopen("wallets.txt", "r");
        if (walletsFile == NULL) {
            printf("Errore nell'apertura del file dei wallet disponibili.\n");
            WaitForUser();
            return;
        }

        char line[200];
        int count = 0;
        char walletPath[200];
        int found = 0;

        while (fgets(line, sizeof(line), walletsFile)) {
            line[strcspn(line, "\n")] = 0; // Rimuove il newline
            if (++count == scelta) {
                snprintf(walletPath, sizeof(walletPath), "%s/%s", WALLET_FOLDER, line);
                found = 1;
                break;
            }
        }
        fclose(walletsFile);

        // Rimuovi il file temporaneo wallets.txt
        remove("wallets.txt");

        // Se il wallet selezionato è stato trovato
        if (found) {
            // Comando per eliminare l'intera directory del wallet e tutto il suo contenuto
            #ifdef _WIN32
                char command[300];
                snprintf(command, sizeof(command), "rmdir /s /q \"%s\"", walletPath);
            #else
                char command[300];
                snprintf(command, sizeof(command), "rm -rf \"%s\"", walletPath);
            #endif

            // Esegui il comando per eliminare la directory
            int result = system(command);
            if (result == 0) {
                printf("\n\t\tEVERYTHING HAS GONE. WALLET '%s' DISTRUTTO CORRETTAMENTE.\n", line);
            } else {
                printf("\n\t\tERRORE DURANTE L'ELIMINAZIONE DEL WALLET.\n");
            }
        } else {
            printf("\nWallet non trovato. Operazione annullata.\n");
        }
    }


    void Visualizzazione(void) {
        list l = NULL;
        printf("\n\n\t\tWELCOME MR.OSCAR!");
        // Stampa il wallet attualmente selezionato
        printf("\n\t\tWALLET ATTUALMENTE SELEZIONATO: %s", focusedWallet);
        printf("\n\t\tVISUALIZZAZIONE WALLET:\n");
        l = FromFileToList(l);
        if (l != NULL) {
            printf("\nCASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG");
        }
        showlist(l);
    }

    float CashMade(void) {
        list l = NULL;
        l = FromFileToList(l);
        
        float cashmade = 0;
        while (l != NULL) {
            cashmade += l->data.Euro;
            l = l->next;
        }
        return cashmade;
    }

    float GramsSold(void) {
        list l = NULL;
        l = FromFileToList(l);
        
        float gramsmade = 0;
        while (l != NULL) {
            gramsmade += l->data.grams;
            l = l->next;
        }
        return gramsmade;
    }

    void HowMuchShouldISell(float spesa, float gramstaken, float gramsoldfor) {
        float gramsold = GramsSold();
        float gramsavailable = gramstaken - gramsold;
        float cashmade = CashMade();
        float cashneeded = spesa - cashmade;
        float gramsneeded = cashneeded / gramsoldfor;
        float totalprofit = cashmade + (gramsavailable * gramsoldfor);

        if (gramsneeded > gramsavailable) {
            printf("\n\t~MR OSCAR YOU CAN'T GET THE AMOUNT OF %0.2f€ SPENT...\n\t~YOU CAN MAX REACH A TOTAL %0.2f€ ( INCLUDING ALSO ALL OF YOUR REMAINING %0.2fg\t(€/GRAMS:%0.2f€))\n\t~THE DEBT WOULD BE OF %0.2f€", spesa, cashmade + gramsavailable * gramsoldfor, gramsavailable, gramsoldfor, spesa - (cashmade + gramsavailable * gramsoldfor));
        } else {
            printf("\n\t~ MR OSCAR TO REACH %0.2f€ YOU NEED TO SELL ABOUT %0.2fg\t(%0.2f €/g) ", spesa, gramsneeded, gramsoldfor);
            printf("\n\t~YOU'LL REMAIN WITH %0.2f g\n \t~YOU CAN SELL ALL THE REMAINING %0.2f GRAMS (€/GRAMS:%0.2f€) FOR A TOTAL AMOUNT OF %0.2f€ TO BE ON A PROFIT OF %0.2f€", gramsavailable - gramsneeded, gramsavailable, gramsoldfor, totalprofit, totalprofit - spesa);
        }
    }

    void HowMuchDidIEarn(void) {
        float spesa, grams, eurotog;
        printf("\t\t\t~DATABASE DEALS");
        printf("\n\n\t~HOW MUCH DID YOU SPEND FOR THIS?\n\t~");
        scanf("%f", &spesa);
        printf("\n\n\t~HOW MUCH GRAMS DID YOU BOUGHT?\n\t~");
        scanf("%f", &grams);
        printf("\n\n\t~HOW MUCH DO YOU SELL A GRAM FOR?\n\t~");
        scanf("%f", &eurotog);
        
        float soldisporchi = CashMade();
        float gramsold = GramsSold();
        float guadagno = soldisporchi - spesa;

        if (guadagno >= 0) {
            printf("\n\n\t\t~YOU ARE DOING RIGHT!\n\n");
            printf("\n€ SPENDED:\t%0.2f€\t\t€ EARNED:\t%0.2f€\nTOTAL AMOUNT EARNED:\t%0.2f€\n\nGRAMS BOUGHT:\t%0.2fg\tGRAMS SOLD:\t%0.2fg\t(€/GRAMS:%0.2f€)\nGRAMS REMAINED:\t%0.2fg\n\nYOU COULD SELL REMAINING %0.2fg FOR A TOTAL OF:\t%0.2f€ AND GETTING A PROFIT OF %0.2f€", spesa, soldisporchi, guadagno, grams, gramsold, eurotog, grams - gramsold, grams - gramsold, (grams - gramsold) * eurotog);
        } else {
            printf("\n\n\t\t~YOU HAVE TO SELL A LITTLE BIT MORE...");
            printf("\n\n\t~OSCAR YOU HAVE SOLD %0.2fg FOR A TOTAL OF %0.2f€\n\t~YOU SPENT: %0.2f€\t AND BOUGHT: %0.2fg", gramsold, soldisporchi, spesa, grams);
            HowMuchShouldISell(spesa, grams, eurotog);
        }
        
        printf("\n\n\t\t~GOOD LUCK MR OSCAR!");
    }



    void menu(void) {
        int scelta;
        do {
            printf("\n\n\t\tMENU PRINCIPALE DRUG WALLET");
            printf("\n1) Visualizza Wallet");
            printf("\n2) Aggiungi Deal");
            printf("\n3) Elimina Deal");
            printf("\n4) Modifica Deal");
            printf("\n5) Cash Made");
            printf("\n6) Grams Sold");
            printf("\n7) Database Analysis");
            printf("\n8) Archive Wallet");
            printf("\n9) Self Destruct");
            printf("\n10) Seleziona Wallet");
            printf("\n11) Crea Nuovo Wallet");
            printf("\n12) Esci");
            printf("\n\nInserisci la tua scelta: ");
            scanf("%d", &scelta);

            switch (scelta) {
                case 1:
                    Visualizzazione();
                    break;
                case 2:
                    {
                        list l = NULL;
                        l = FromFileToList(l);
                        l = Add_Deal(l);
                        FromListToFile(l);
                    }
                    break;
                case 3:
                    {
                        list l = NULL;
                        l = FromFileToList(l);
                        showlist(l);
                        int tag;
                        printf("\nInserisci il tag del deal da eliminare: ");
                        scanf("%d", &tag);
                        l = delete_node(l, tag);
                        FromListToFile(l);
                    }
                    break;
                case 4:
                    {
                        list l = NULL;
                        l = FromFileToList(l);
                        showlist(l);
                        int tag;
                        while (1) {
                            printf("\nInserisci il tag del deal da modificare: ");
                            scanf("%d", &tag);
                            printf("\nHai selezionato il tag #%d. È corretto?\n\t1) Sì\t2) No\t3) Esci\n: ", tag);
                            int conferma;
                            scanf("%d", &conferma);
                            if (conferma == 1) {
                                l = ModifyByTagList(l, tag);
                                FromListToFile(l);
                                break;
                            } else if (conferma == 3) {
                                printf("\nOperazione annullata.\n");
                                break;
                            }
                        }
                    }
                    break;
                case 5:
                    printf("\n\n\t\tCASH MADE: %.2f€\n\n", CashMade());
                    break;
                case 6:
                    printf("\n\n\t\tGRAMS SOLD: %.2fg\n\n", GramsSold());
                    break;
                case 7:
                    HowMuchDidIEarn();
                    break;
                case 8:
                    {
                        char walletName[100];
                        printf("\nInserisci il nome dell'archivio del wallet: ");
                        scanf("%99s", walletName);
                        ArchiveWallet(walletName);
                    }
                    break;
                case 9:
                    SelfDestroy();
                    break;
                case 10:
                    selectWallet();
                    break;
                case 11:
                    CreateNewWallet();
                    break;
                case 12:
                    printf("\nUscita in corso...\n");
                    break;
                default:
                    printf("\nScelta non valida, riprova.\n");
                    break;
            }
           if(scelta !=10) WaitForUser();
        } while (scelta != 12);
    }

    list Add_Deal(list l) {
        Element ref;
        
        printf("\n\t\tINSERIMENTO DEAL");
        printf("\nINSERT CASH:\t");
        scanf("%f", &ref.Euro);
        printf("\nINSERT GRAMS:\t");
        scanf("%f", &ref.grams);
        printf("\nINSERT BUYER:\t");
        scanf("%s", ref.Persona);
        printf("\nINSERT DAY (xx/xx/xxxx):\t");
        scanf("%d/%d/%d", &ref.data.day, &ref.data.month, &ref.data.year);
        ref.Tag = NumCasualBetween(0, 1000);

        l = cons(l, ref);
        printf("\n\nDEAL ADDED CORRECTLY\n");

        return l;
    }

    list ModifyByTagList(list l, int tag) {
        Element ref;
        int trovato = 0;
    
        list patt = l;
        if (patt == NULL) {
            printf("\n\nFILE VUOTO!!!");
            return l;
        }
        while (1) {
            while (patt != NULL && !trovato) {
                if (patt->data.Tag == tag) {
                    trovato = 1;
                    printf("\n\t\t ~FOUND DEAL");
                    printf("\nCASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG");
                    printElement(patt->data);
                    printf("\n\t\tMODIFICA DEAL TAG:#%d", tag);
                    printf("\nINSERT CASH:\t");
                    scanf("%f", &ref.Euro);
                    printf("\nINSERT GRAMS:\t");
                    scanf("%f", &ref.grams);
                    printf("\nINSERT BUYER:\t");
                    scanf("%s", ref.Persona);
                    printf("\nINSERT DATA (xx/xx/xxxx):\t");
                    scanf("%d/%d/%d", &ref.data.day, &ref.data.month, &ref.data.year);
                    patt->data.Euro = ref.Euro;
                    patt->data.grams = ref.grams;
                    strcpy(patt->data.Persona, ref.Persona);
                    patt->data.data.day = ref.data.day;
                    patt->data.data.month = ref.data.month;
                    patt->data.data.year = ref.data.year;
                    printf("\n\n\t\tDEAL #%d MODIFIED CORRECTLY", tag);
                    return l;
                }
                patt = patt->next;
            }
            if (!trovato) {
                printf("\nTAG NON TROVATO!\nVuoi riprovare?\n\t1) Sì\t2) No\n: ");
                int scelta;
                scanf("%d", &scelta);
                if (scelta == 2) {
                    return l;
                } else {
                    printf("\nInserisci di nuovo il tag del deal da modificare: ");
                    scanf("%d", &tag);
                    patt = l;
                }
            }
        }
    }