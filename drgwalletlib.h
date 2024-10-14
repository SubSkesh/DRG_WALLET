


#define drgwalletlib_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>


typedef char String[10];
typedef struct
{
    int day,month,year;
}
Data;

typedef struct
{
  
    float Euro;
    float grams;
    String Persona;
    Data data;
    int Tag;

}
Element;
typedef struct nodo_lista
{
    Element data;
    struct nodo_lista *next;
}NODO;
typedef NODO *list;
void clearInputBuffer(void);
void listAvailableWallets();
void selectWallet();
int NumCasualBetween(int min, int max);
list cons (list l,Element el);
void truncateString(char *str, int maxLength);
void printElement(Element x);
void showlist(list l);
list delete_node(list l,int el);
void FromListToFile(list l);
list FromFileToList(list l);
void CreateNewWallet();
void Archive_Wallet(const char* walletFilename);
void CreatePassword(void);


int InsertPassword(void);
void Visualizzazione(void);
float CashMade(void);
float GramsSold(void);
void SelfDestroy(void);
list ModifyByTagList(list l,int tag);
list Modify_deal(void);
list Delete_Deal(void);
list Add_Deal(list l);
void OpenArchivedWallet(void);
//void Archive_deals(void);
void HowMuchShouldISell(float spesa,float gramstaken,float gramsoldfor);
void HowMuchDidIEarn(void);
void ExitOrStay_WalletAction(void);
void ExitOrStay_WalletMainMenu(void);
int MenuAggiornaWallet(void);
void AggiornaWallet(void);
void menu(void);




