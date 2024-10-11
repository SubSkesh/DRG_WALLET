//#define _CRT_SECURE_NO_WARNINGS
#include "drgwalletlib.h"
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h> // Necessario per leggere le directory (solo Unix-like e Windows con MinGW)

void Info(void) {
    struct dirent *entry;
    DIR *dp = opendir("./");

    if (dp == NULL) {
        perror("Errore nell'apertura della directory corrente");
        return;
    }

    printf("\n\n\t\tINFORMAZIONI SUI WALLET ARCHIVIATI:\n");
    int walletCount = 0;

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            printf("\n- Wallet archivio: %s", entry->d_name);
            walletCount++;
        }
    }

    closedir(dp);

    printf("\n\nNumero totale di wallet archiviati: %d\n", walletCount);
    printf("Gli archivi sono salvati nella directory corrente con il nome specificato al momento dell'archiviazione.\n");
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
            printf("\nDO YOU WANT TO DELETE IT?\n\t1) Yes\t2) No\t3) Cancel\n: ");
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
                printf("\n\nOperazione annullata.\n");
                return l;
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
    FILE *f = fopen("DrugWallet.bin", "wb");
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
    FILE *f = fopen("DrugWallet.bin", "rb");
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

    do {
        printf("\n\nINSERISCI PASSWORD:\t");
        scanf("%s", TryPassword);

        if (strcmp(MyPassword, TryPassword) == 0) {
            printf("\n\tPASSWORD CORRETTA.\n\n");
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
    if (InsertPassword()) {
        FILE *f = fopen("DrugWallet.bin", "wt");
        if (f == NULL) {
            perror("Errore nell'apertura del file");
            exit(EXIT_FAILURE);
        }
        char End[20] = "Everything Has Gone";
        fwrite(End, sizeof(End), 1, f);
        fclose(f);
        printf("\n\t\tEVERYTHING HAS GONE");
        exit(1);
    }
    printf("\n\t\t~AH... NICE TRY\n\n");
}

void Visualizzazione(void) {
    list l = NULL;
    printf("\n\n\t\tWELCOME MR.OSCAR!");
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

void WaitForUser(void) {
    printf("\nPremi INVIO per continuare...");
    getchar();
    getchar();
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
        printf("\n9) Info");
        printf("\n10) Self Destruct");
        printf("\n11) Esci");
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
                Info();
                break;
            case 10:
                SelfDestroy();
                break;
            case 11:
                printf("\nUscita in corso...\n");
                break;
            default:
                printf("\nScelta non valida, riprova.\n");
                break;
        }
        WaitForUser();
    } while (scelta != 11);
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