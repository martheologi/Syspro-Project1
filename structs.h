#ifndef __STRUCTS__
#define __STRUCTS__

typedef struct Date{
    int day;
    int month;
    int year;
}Date;

typedef struct Time{
    int hour;
    int mins;
}Time;

typedef struct bitcoinListNode{     //lista apo bitcoinIDs
    int bitcoinID;
    int value;                      //to value se $ tou bitcoin
    struct bitcoinListNode* next;   //deikths se epomeno komvo
}bitcoinListNode;

typedef struct Wallet{
    char* userID;                   //to onoma ka8e xrhsth
    int sum;                        //to poso pou dia8etei o xrhsths
    bitcoinListNode* bitCoinList;   //mia lista me ta bitcoin tou xrhsth
}Wallet;

typedef struct transactionListNode{
    char* userID;                       //to onoma tou sender/receiver
    char* transactionID;                //to ID tou transaction
    int transactionValue;               //to poso tou transaction
    Date date;
    Time mytime;
    struct transactionListNode* next;
}transactionListNode;

typedef struct TreeNode {
    int walletID;              //h 8esh tou sender/receiver sto wallet
    int value;                 //to poso ths sunallaghs
    struct TreeNode* left;
    struct TreeNode* right;
}TreeNode;

typedef struct TreeRoot{
    int bitcoinID;         //to bitcoinID tou dentrou
    TreeNode* treenode;    //o komvos tou dentrou pou einai kai riza
}TreeRoot;

typedef struct RootListNode{    //lista apo rizes dentrwn
    TreeRoot* root;
    struct RootListNode* next;
}RootListNode;

typedef struct HashTableBucket{
    int recs;                       //posa records exei to bucket
    void* bucket;                   //h mnhmh tou bucket pou 8a mpoun ta records
    struct HashTableBucket* next;
}HashTableBucket;

typedef struct HT_record{
    int walletID;                         //h 8esh tou xrhsth sto wallet(sto opoio exw ola ta stoixeia tou)
    transactionListNode* transactionList; //mia lista me ta transactions tou
}HT_record;

#endif
