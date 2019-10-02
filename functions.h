//MAINFUNCTIONS.c
void TakeTransactionsInfo(char* transaction, char** trID, char** sender, char** receiver, int* value, Date* date, Time* mytime);

void checkTransactionsInfo(char* transaction, char** trID, char** sender, char** receiver, int* trValue, Date* date, Time* mytime, int* maxID);

void ExecuteTransactions(HashTableBucket** senderHT, HashTableBucket** receiverHT, char* sender, char* receiver, int sender_ht_size, int receiver_ht_size, int counter, Wallet* wallet, RootListNode* rootList, int bucketSize, char* trID, int trValue, Date date, Time mytime);

void findEarnigsOrPayments(HashTableBucket** ht, char* user, int walletPos, int ht_size, char* timedate);

void bitcoinStatus(RootListNode* rootList, Wallet* wallet, int bitcoinID);

void traceCoin(RootListNode* rootList, int bitcoinID);


//LISTS.c
bitcoinListNode* initializeBitcoinList();

void BitcoinListAppend(bitcoinListNode* ptr,int bitcoinId, int value);

void PrintBitcoinList(Wallet* wallet, int counter);

void FreeBitcoinList(bitcoinListNode* ptr);

transactionListNode* initializeTransactionList(char* userID, char* transactionID, int transactionValue, Date date, Time mytime);

void TransactionListAppend(transactionListNode* ptr, char* userID, char* transactionID, int transactionValue, Date date, Time mytime);

void FreeTransactionList(transactionListNode* ptr);

RootListNode* initializeRootList();

void RootListAppend(RootListNode* ptr, TreeRoot* root);

void PrintRootList(RootListNode* ptr);

TreeRoot* FindRoot(RootListNode* rootList, int bitcoinID);

void UpdateRootList(RootListNode** rootList, TreeRoot* root);

void FreeRootList(RootListNode* ptr);


//FUNCTIONS.c
void UserExistion(Wallet* wallet, char* userID, int lastUser);

int hash_function(char* walletID, int ht_size);

HashTableBucket** initializeHT(int ht_size);

int findUserInBucket(void* bucket, int walletPos, int recs);

void FreeHashTable(HashTableBucket** ht, int ht_size);

int findWalletPosition(Wallet* wallet, char* userID, int counter);

Wallet* WalletUpdate(Wallet* wallet, int sender_walletPos, int receiver_walletPos, int value, RootListNode** rootList);

HashTableBucket** insertRecord(HashTableBucket** ht, int walletPos, int hash_result, int bucketSize, char* userID, char* transactionID, int transactionValue, Date date, Time mytime);

int DateValidation(Date startDate, Date endDate, Date nodeDate);

int TimeValidation(Time startTime, Time endTime, Time nodeTime);


//TREE.c
TreeRoot* initializeRoot(int bitcoinID, int walletID, int startValue);

TreeNode* initializeTreeNode(int walletID, int value);

TreeNode* findTreeNode(TreeNode* node, int walletID);

TreeNode* BitcoinTreeAppend(TreeNode* node, int SwalletID, int RwalletID, int value);

void printTreein(TreeNode *root, int space);

void printTree(TreeNode *root);

void FreeTree(TreeNode *node);
