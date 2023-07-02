//#define _CRT_SECURE_NO_WARNINGS
#include"drgwalletllib.h"

int NumCasualBetween(int min, int max)
{
srand((unsigned int)time(NULL));
return rand() % (max-min+1)+min;
}
list tail_cons(list l,Element el)
{
    list tail=(list)malloc(sizeof(Element));
    tail->data=el; 

    tail->next=NULL;
    list clone ;
    clone=l;
    list prec=malloc(sizeof(Element));
    
   
    if (l==NULL) return tail;
    while (clone!=NULL)
    {
        prec=clone;
        clone=clone->next;
    }
    prec->next=tail;
    free(tail);
    return l;
}


list cons (list l,Element el)
{
    
    list t;
   
    t=(list)malloc(sizeof(Element));
    t = NULL;
    t->data=el;
    t->next=l;
    return t;
}


void printElement(Element x)
{
    printf("\n%0.2f$\t\t%0.2fg\t\t%s\t\t%d/%d/%d\t#%d",x.Euro,x.grams,x.Persona,x.data.day,x.data.month,x.data.year,x.Tag);
}


void showlist(list l)
{
    if (l==NULL) printf("\nFILE VUOTO!!!");
    
    while(l!=NULL)
    {
        printf("\n");
        printElement(l->data);
        
        l=l->next;
    }
    printf("\n\n");
}
list delete_node(list l,int el)
{
    int trovato=0;
    int scelta;
    list patt,prec=NULL;
    patt=l;
    if(l==NULL) return prec;
    
    
    if ((patt->data.Tag)==el)//se l'elemento è primo
    {
        printf("\nCASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG\n");
        printElement(l->data);
        printf("\nDO YOU WANT TO DELETE IT?\n\t1)Yes\t2)No\n:");
        scanf("%d",&scelta);
        if (scelta)
        {
        
        l=l->next;//AGGIORNA LA RADICE
        
        }
        return l;
      
    }
    else
    {
    
    while ((patt!=NULL)&&(!trovato))
    {
       
        if
        (el==patt->data.Tag) trovato=1;
        else
        {
        prec=patt;
        patt=patt->next;
        }
    }
    if (trovato)
    {
        printf("\n\n\t\t\t~GOT DEAL");
        printf("\nCASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG");
        
        printElement(patt->data);
        printf("\n\n\t\t\tDO YOU WANT TO DELETE IT?\n\t1)Yes\t2)No\n:");
        scanf("%d",&scelta);
        if (scelta==1)///quiiii
        {
        printf("\n\n\t\tDEAL #%d CANCELED CORRECTLY",patt->data.Tag);
        prec->next=patt->next;
        free(patt);
        }
        else
        {
            printf("\n\n\t\t\tDEAL #%d NOT CANCELED",patt->data.Tag);
        }
        return l;
    }
        
    else
            { printf("\n\t\t\tTAG NON TROVATO!"); return l;}// se non c'è lelemento
   
    }///guarda l'if esterno
}
void FromListToFile(list l)
{
    FILE *f=fopen("DrugWallet.bin", "wb");
    while (l!=NULL)
    {
        fwrite(&l->data, sizeof(Element), 1, f);
        l=l->next;
    }
    fclose(f);
    
}

list FromFileToList(list l)
{
    
    FILE *f=fopen("DrugWallet.bin", "rb");
    if (f==NULL)
    {
        printf("\n\t\tFILE VUOTO!");
        fclose(f);
        return l;
    }
    Element ref;
    
    while(fread(&ref, sizeof(Element), 1, f)>0)
        {
        l= cons(l, ref);
        }

    
    fclose(f);
    return l;
}
void CreatePassword(void)
{
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

int InsertPassword(void)//(restituisce 1 se è andata bene ,0 se non se si ha una pw -1 se buggato)
{
    FILE *f=fopen("DrugWalletpswrd.bin", "rb");
if (errno == ENOENT)
{
    CreatePassword();
    return 1;
}
    String MyPassword;
    String TryPassword;
    int AttemptPswrd=3;
    int c=0;
  fread(MyPassword, sizeof(String), 1, f);
   // printf("%s",MyPassword);  PER VEDERE LA PW ABILITA QUESTA RIGA
    fclose(f);
    printf("\n\nINSERISCI PASSWORD:\t");
    scanf("%s",TryPassword);

        while ((strcmp(MyPassword, TryPassword)!=0)&&(c<AttemptPswrd-1))
            {
             c++;
            printf("\n\nPASSWORD ERRATA!");
            printf("\n\nINSERISCI PASSWORD:\t");
            scanf("%s",TryPassword);
            }
            if (c==AttemptPswrd-1)
            {
                printf("\n\n\n\t\t\tAUTOCHIUSURA:TROPPI TENTATIVI DI INSERT PASSWORD\n\n\t\t\t*INCULATT*\n");
               return 0;
            }
            else
            {
  
        printf("\n\tPASSWORD CORRETTA.\n\n");
           
        return 1;
            }

}

void SelfDestroy(void)
{
    printf("\n\n\n\n\n\n\n\n\n\t\t\tAUTO DISTRUZIONE");
    if (InsertPassword())
    {
    FILE *f =fopen("DrugWallet.bin", "wt");
        char End[20]="Everything Has gone";
    fwrite(End, sizeof(End), 1, f);
        fclose(f);
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\tEVERYTHING HAS GONE†");
        exit(1);
    }
    printf("\n\n\n~AH...I THOUGHT COPS MAY GOT YOU,NICE TRY\n\n\n");
    
}

void Visualizzazione(void)
{

    list l=NULL;
    
 

      printf("\n\n\t\tWELCOME MR.OSCAR!\n");
      printf("\n\t\tVISUALIZZAZIONE WALLET:\n");
      l=FromFileToList(l);
      if (l!=NULL)
      {
          printf("\nCASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG");
      }
      showlist(l);

  
}




float CashMade(void)
{
  
    list  l=NULL;
    l=FromFileToList(l);
    
    float cashmade=0;
    if (l==NULL)
    {
        printf("\n\n\n\n\n\n\n\t\t\t\tERROR !*CASHMADE*NOCASH");
        exit(1);
    }
    while (l!=NULL)
           {
               cashmade=cashmade+(l->data.Euro);
               l=l->next;
    }
    return cashmade;
    
}
float GramsSold(void)
{
  
    list  l=NULL;
    l=FromFileToList(l);
    
    float gramsmade=0;
    if (l==NULL)
    {
        printf("\n\n\n\n\n\n\n\t\t\t\tERROR !*GRAMSMADE*NOGRAMS");
        exit(1);
    }
    while (l!=NULL)
           {
               gramsmade=gramsmade+(l->data.grams);
               l=l->next;
    }
    return gramsmade;
    
}
list ModifyByTagList(list l,int tag)
{
    Element ref;
    int trovato=0;
   
    list patt=l;
    if (patt==NULL)
    {
        printf("\n\nFILE VUOTO!!!");
    }
    while ((patt!=NULL)&&(!trovato))
    {
        if (patt->data.Tag==tag)
        {
            
            printf("\n\t\t\t ~FOUND DEAL");
            printf("\nCASH\t\tGRAMS\t\tWHO\t\tWHEN\t\tTAG");
             printElement(patt->data);
            printf("\n\t\tMODIFICA DEAL TAG:#%d",tag);
            printf("\nINSERT CASH:\t");
            scanf("%f",&ref.Euro);
            printf("\nINSERT GRAMS:\t");
            scanf("%f",&ref.grams);
            printf("\nINSERT BUYER:\t");
            scanf("%s",ref.Persona);
            printf("\nINSERT DATA(xx/xx/xxxxx):\t");
            scanf("%d/%d/%d",&ref.data.day,&ref.data.month,&ref.data.year);
            patt->data.Euro=ref.Euro;
            patt->data.grams=ref.grams;
            strcpy(patt->data.Persona, ref.Persona);
            patt->data.data.day=ref.data.day;
            patt->data.data.month=ref.data.month;
            patt->data.data.year=ref.data.year;
            printf("\n\n\t\t\tDEAL #%d MODIFIED CORRECTLY",ref.Tag);
        }
        patt=patt->next;
    }
    return l;
}    
    
list Modify_Deal(void)
{
    int tag;
    list l=NULL;
    l=FromFileToList(l);
    printf("----------------------------------------------");
    showlist(l);
    if (l!=NULL)
    {
        printf("\n\t\t\tMODIFICA DEAL");
        printf("\nINSERISCI IL TAG DELL DEAL DA MODIFICARE\n\t:#");
        scanf("%d",&tag);
        ModifyByTagList(l,tag);
       
        
    }
    return l;
}
list Delete_Deal(void)
{
    int tag;
    list l=NULL;
    l=FromFileToList(l);
    printf("----------------------------------------------");
    showlist(l);
    if (l!=NULL)
    {
      
        printf("----------------------------------------------");
        printf("\n\n\n");
        printf("\n\t\t\tELIMINAZIONE DEAL");
        printf("\nINSERISCI IL TAG DELL DEAL DA ELIMNARE\n\t:#");
        scanf("%d",&tag);
        l=delete_node(l, tag);
        
        printf("\n\n\t\t\t CHIUSURA DELETE_DEAL");
     
        printf("\n\n\n\n\n\n\n");
       
    }
    return l;
}

list Add_Deal(void)
{

    list l=NULL;
    Element ref;
    //l=FromFileToList(l);
    
    
    printf("\n\t\tINSERIMENTO DEAL");
    printf("\nINSERT CASH:\t");
    scanf("%f",&ref.Euro);
    printf("\nINSERT GRAMS:\t");
    scanf("%f",&ref.grams);
    printf("\nINSERT BUYER:\t");
    scanf("%s",ref.Persona);
    printf("\nINSERT DAY(xx/xx/xxxxx):\t");
    scanf("%d/%d/%d",&ref.data.day,&ref.data.month,&ref.data.year);
    ref.Tag=NumCasualBetween(0, 1000);
    
    
    FILE *f2=fopen("DrugWallet.bin", "ab");
    //int control=fseek(f2, 0, SEEK_END);
 //   if(control!=0) printf("errore!*fseek*");
    fwrite(&ref, sizeof(Element), 1, f2);
    fclose(f2);
    l=FromFileToList(l);
  //  l=cons(l, ref);
    printf("\n\n\nDEAL ADDED CORRECTLY");

    printf("\n\n\t\t\t CLOSING ADD DEAL\n\n");
    printf("----------------------------------------------");
    printf("\n\n\n\n\n\n\n");

    return l;
    
}
void Archive_deals(const char* walletFilename)
{
    int risposta;
    char NameWallet[100];
    list l = NULL;
    l = FromFileToList(l);
    printf("\n\n\t\tDO YOU WANT TO ARCHIVE THIS DRUGWALLETT?\n1)YES\t\t2)NO\n\n\t~");
    scanf("%d", &risposta);
    if (risposta == 1)
    {
        printf("\n\n\t\tHOW THIS DRUG WALLET HAS TO BE CALLED?\n\t~");
        scanf("%99s", NameWallet); // Limit input to 99 characters to avoid buffer overflow
        char backupFilename[200];
        sprintf(backupFilename, "%s.bak", walletFilename); // Create backup filename by appending ".bak"
        if (rename(walletFilename, backupFilename) != 0) { // Rename wallet file and check for errors
            printf("\n\n\t\t~ERROR: COULD NOT ARCHIVE DRUG WALLET\n\n\t\tGOODBYE MY OSCAR...");
            exit(1);
        }
        if (rename(backupFilename, NameWallet) != 0) { // Rename backup file to new name and check for errors
            // If renaming fails, restore the original wallet file and exit
            printf("\n\n\t\t~ERROR: COULD NOT RENAME DRUG WALLET TO NEW NAME\n\n\t\tRESTORING ORIGINAL WALLET FILE...");
            if (rename(backupFilename, walletFilename) != 0) {
                printf("\n\n\t\t~ERROR: COULD NOT RESTORE ORIGINAL WALLET FILE\n\n\t\tGOODBYE MY OSCAR...");
                exit(1);
            }
            printf("\n\n\t\t~ORIGINAL WALLET FILE RESTORED SUCCESSFULLY\n\n\t\tGOODBYE MY OSCAR...");
            exit(1);
        }
        printf("\n\n\t\t~DRUG WALLET ARCHIVED SUCCESSFULLY\n\n\t\tGOODBYE MY OSCAR...");
        exit(1);
    }
    else
    {
        printf("\n\n\t\t~DONT WORRY, YOU'LL DO IT NEXT TIME OSCAR...");
    }
}

/*void Archive_deals()
{
  
    int risposta;
    String NameWallet;
    list l=NULL;
    l=FromFileToList(l);
    printf("\n\n\t\tDO YOU WANT TO ARCHIVE THIS DRUGWALLETT?\n1)YES\t\t2)NO\n\n\t~");
    scanf("%d",&risposta);
    if (risposta==1)
    {
        printf("\n\n\t\tHOW THIS DRUG WALLET HAS TO BE CALLED?\n\t~");
        scanf("%s",NameWallet);
        rename("DrugWallet.bin", NameWallet);
        printf("\n\n\t\t~DRUGWALLET ARCHIVED SUCCESSFULLY\n\n\t\tGOODBYE MY OSCAR...");
        exit(1);
    }
    else
    {
        printf("\n\n\t\t~DONT WORRY, YOU'LL DO IT NEXT TIME OSCAR...");
    }
    
    
}*/
void HowMuchShouldISell(float spesa,float gramstaken,float gramsoldfor)
{

    float grammivenduti=GramsSold();
    float gramsavailable=gramstaken-grammivenduti;
    float cashmad=CashMade();
    float cash=spesa-cashmad;
    float c;
    float totalprofit=cashmad+gramsavailable*gramsoldfor;
    c=cash/gramsoldfor;
    if (c>gramsavailable)
    {
    
        printf("\n\t~MR OSCAR YOU CAN'T GET THE AMOUNT OF %0.2f€ SPENT...\n\t~YOU CAN MAX REACH A TOTAL %0.2f€ ( INCLUDING ALSO ALL OF YOUR REMAINING %0.2fg\t(€/GRAMS:%0.2f€))\n\t~THE DEBT WOULD BE OF %0.2f€",spesa,cashmad+gramsavailable*gramsoldfor,gramsavailable,gramsoldfor,spesa-(cashmad+gramsavailable*gramsoldfor));
        
    
    }
    else
    {
        printf("\n\t~ MR OSCAR TO REACH %0.2f€ YOU NEED TO SELL ABOUT OTHERS %0.2fg\t(%0.2f €/g) ",spesa,c,gramsoldfor);
        printf("\n\t~YOU'LL REMAIN WITH %0.2f g\n \t~YOU CAN SELL ALL THE REMAINING %0.2f GRAMS (€/GRAMS:%0.2f€) FOR A TOTAL AMOUNT OF %0.2f€ TO BE ON A PROFIT OF %0.2f€",gramsavailable-c,gramsavailable,gramsoldfor,totalprofit,totalprofit-spesa);

  
    }

    
}
void HowMuchDidIEarn(void)
{
    float spesa,grams,gramsold,soldisporchi,guadagno,eurotog;
    
    printf("\t\t\t~DATABASE DEALS");
    printf("\n\n\t~HOW MUCH DID YOU SPEND FOR THIS?\n\t~");
    scanf("%f",&spesa);
    printf("\n\n\t~HOW MUCH GRAMS DID YOU BOUGHT?\n\t~");
    scanf("%f",&grams);
    printf("\n\n\t~HOW MUCH DO YOU SELL A GRAM FOR?\n\t~");
    scanf("%f",&eurotog);
    
    soldisporchi=CashMade();
    gramsold=GramsSold();
    guadagno=spesa-soldisporchi;

    
    
    
    if (guadagno<0)
    {
        printf("\n\n\t\t~YOU ARE DOING RIGHT!\n\n");
        printf("\n€ SPENDED:\t%0.2f€\t\t€ EARNED:\t%0.2f€\nTOTAL AMOUNT EARNED:\t%0.2f€\n\nGRAMS BOUGHT:\t%0.2fg\tGRAMS SOLD:\t%0.2fg\t(€/GRAMS:%0.2f€)\nGRAMS REMAINED:\t%0.2fg\n\nYOU COULD SELL REMAINING %0.2fg FOR A TOTAL OF:\t%0.2f€ AND GETTING A PROFIT OF %0.2f€(+%0.2f€)",spesa,soldisporchi,-1*guadagno,grams,GramsSold(),eurotog,grams-gramsold,grams-gramsold,(grams-gramsold)*eurotog,-1*guadagno+(grams-gramsold)*eurotog,(-1*guadagno+(grams-gramsold)*eurotog)-(-1*guadagno));
    }
    else
    {
        printf("\n\n\t\t~YOU HAVE TO SOLD A LITTLE BIT MORE...");
        printf("\n\n\t~OSCAR YOU HAVE SOLD %0.2fg FOR A TOTAL OF %0.2f€\n\t~YOU SPENT: %0.2f€\t AND BOUGHT: %0.2fg",gramsold,soldisporchi,spesa,grams);
        HowMuchShouldISell(spesa, grams, eurotog);
        
    }
    
    printf("\n\n\t\t~GOOD LUCK MR OSCAR!");
    
    
    
}
void ExitOrStay_WalletAction(void)
{
    int risposta;
    printf("\n\n\t\t~NEED SOMETHING ELSE IN WALLET ACTION?†\n\t1)Yes\t\t2)No\n\t:");
    scanf("%d",&risposta);



    if (risposta==1)   return AggiornaWallet();
   
}
void ExitOrStay_WalletMainMenu(void)
{
    int risposta;
    printf("\n\n\t\t~NEED SOMETHING ELSE IN WALLET?†\n\t1)Yes\t\t2)No\n\t:");
    scanf("%d",&risposta);
    printf("----------------------------------------------");
    printf("\n\n\n\n\n\n\n");
    if (risposta==1)   return menu();
    else
    {
        printf("\n\n\n\n\n\t\t\t\tDRUGWALLET CLOSED.");
        printf("\n\t\t\t\t~Bye Mr Oscar\n\n");
        exit(1);
    
        
    }
}
    
int MenuAggiornaWallet(void)
{
    int scelta;
    printf("----------------------------------------------");
    printf("\n\n\n\n\n\n\n");
    printf("\n\t\tMENU ACTION WALLET");
    printf("\n1)AGGIUNGI DEAL\n2)RIMUOVI DEAL\n3)MODIFICA DEAL\n4)CASH MADE\n5)GRAMS SOLD\n6)DATABASE\n7)BACK\n:\t");
    
    scanf("%d",&scelta);
    return scelta;
    
}
void menu(void)
{
    int risposta;
   // int block=0;

   // do
    
    printf("\n\n\t\tWELCOME MR.OSCAR!\n");
    printf("\t\tMENU DRUGWALLET©\n\n");
    printf("\n1)VISUALIZZA WALLET\n2)AZIONI WALLET\n3)SELFDESTROY\n4)ARCHIVE DRUGWALLET\n5)EXIT\n");
    printf(":\t");
    scanf("%d",&risposta);
    if ((risposta<1)&&(risposta>3))
    {
       return menu();
    }
    //while (risposta<1&&risposta>4);
        
    
    switch (risposta)
    {
        case 1:
            printf("\n\n\n\n\n\n\n");
            Visualizzazione();
            break;
        case 2 :
            printf("\n\n\n\n\n\n");
            AggiornaWallet();
            break;
        case 3:
            SelfDestroy();
            break;
        case 4:
            Archive_deals("C:\\Users\\Oski\\Desktop\\OscarCodiinC\\C\\DrugWallet.bin");
        case 5:
            printf("\n\n\n\n\n\t\t\t\tDRUGWALLET CLOSED.");
            printf("\n\t\t\t\t~Bye Mr Oscar\n\n");
                    exit(1);
            break;
            
            
    }
    ExitOrStay_WalletMainMenu();
}
                                                                                


void AggiornaWallet(void)
{
    list l=NULL;
    int menu_x;
    //FILE *f=fopen("DrugWallet.bin", "rb");

 
        menu_x=MenuAggiornaWallet();
        switch (menu_x)
        {
        
                
            case 1:
                 l=Add_Deal();
                FromListToFile(l);
                ExitOrStay_WalletAction();
                
                
                
                
               
                break;
                
            case 2:
                l=Delete_Deal();
                FromListToFile(l);
                ExitOrStay_WalletAction();

                break;
            
                
            case 3:
                l=Modify_Deal();
                FromListToFile(l);
                ExitOrStay_WalletAction();

                
                break;
            
            case 4:
                
              printf("\n\n\n\n\n\n\t\t\tCASH MADE: %.2f€\n\n\n\n\n\n",CashMade());
                ExitOrStay_WalletAction();

                break;
                
            case 5:
                    
                printf("\n\n\n\n\n\n\t\t\tGRAMS SOLD: %.2fg\n\n\n\n\n\n",GramsSold());
                ExitOrStay_WalletAction();

                    break;
            case 6:
                HowMuchDidIEarn();
                ExitOrStay_WalletAction();
                break;
           
            case 7:
                printf("\n\n\n\n\n");
                menu();
                break;
        }
    
    
    
    
    
}
                                                                 
