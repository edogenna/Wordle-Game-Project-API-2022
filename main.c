#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_ALF 64
#define L_VET 200

typedef struct node_{
    struct node_* left;
    struct node_* right;
    struct node_* next;
    char* key;
}node_t;

short charToNum(char c);
char numToChar(short n);

int ordLG(char* a, char* b);

void BSTInsert(char* s, short check);
node_t* BSTSearch(node_t* n, char* k);

short wordComparison(char* attempt, char* reference, char* output);
int isCorrect(char *output);
short printable(char *s);
void printFiltered(node_t* n);
void printAll(node_t* n);

void allocateNode();

int updateList();
int initList(node_t*n);

void initConstraints();

//GLOBAL VARIABLE
node_t *nil, *root, *lastList, *firstList;
int wordLength = 0, indAr;
node_t *arNode;

//constraints
short **matr;
int nOcc[N_ALF]; //0 if I still don't understand anything about that letter, >0 if it is minimum number, <0 if it is exact

int main() {
    lastList = malloc(sizeof(node_t));
    nil = malloc(sizeof(node_t));
    nil->right = NULL;
    nil->left = NULL;
    nil->next = NULL;
    root = nil;

    int lives, isOk, lenTmp = 0, cRead = 1;
    char *reference, *input, *output;
    int newWords, newVinc, numPrintable, noAttempt;

    allocateNode();

    cRead = scanf(" %d", &wordLength);

    if(cRead > 0){
        //the most long command is '+stampa_filtrate', which is long 17 char
        if(wordLength > 17)
            lenTmp = wordLength;
        else
            lenTmp = 17;
    }


    reference = (char*) malloc(sizeof(char)*(wordLength+1));
    output = (char*) malloc(sizeof(char)*(wordLength+1));
    input = (char*) malloc(sizeof(char)*(lenTmp+1));

    matr = (short**) malloc(sizeof(short*)*wordLength);
    for(int i = 0; i < wordLength; i++){
        matr[i] = (short*) malloc(sizeof(short)*N_ALF);
    }

    cRead = scanf(" %s", input);
    while(cRead != 0 && input[0] != '+'){
        BSTInsert(input,0);
        cRead = scanf(" %s", input);
    }

    while(cRead > 0){
        if(input[0] == '+' && input[1] == 'n'){
            cRead = scanf(" %s", reference);
            cRead = scanf("%d", &lives);
            initConstraints();

            isOk = 1;
            noAttempt = 1;
            newWords = 0;
            while(isOk) {
                cRead = scanf(" %s", input);

                if (input[0] == '+' && input[11] == 'i') {
                    newWords = 1;
                    cRead = scanf(" %s", input);
                    while (cRead > 0 && input[0] != '+') {
                        BSTInsert(input, 1);
                        cRead = scanf(" %s", input);
                    }
                }else if(input[0] == '+' && input[1] == 's'){
                    if(noAttempt == 1){
                        printAll(root);
                    }else{
                        printFiltered(root);
                    }
                }else{
                    if (BSTSearch(root, input) == NULL)
                        printf("not_exists\n");
                    else {
                        newVinc = wordComparison(input, reference, output);
                        if (isCorrect(output)) {
                            printf("ok\n");
                            isOk = 0;
                        } else {
                            printf("%s\n", output);
                            if(newVinc || newWords){
                                if(noAttempt){
                                    firstList = lastList;
                                    numPrintable = initList(root);
                                    noAttempt = 0;
                                }else{
                                    numPrintable = updateList(root);
                                }
                            }
                            printf("%d\n", numPrintable);
                            lives--;
                            if(lives == 0){
                                printf("ko\n");
                                isOk = 0;
                            }
                        }

                    }
                }

            }
        }else if (input[0] == '+' && input[11] == 'i') {
            cRead = scanf(" %s", input);
            while (input[0] != '+') {
                BSTInsert(input, 0);
                cRead = scanf(" %s", input);
            }
        }

        cRead = scanf(" %s", input);
    }


    return 0;
}

short charToNum(char c){
    if(c >= 'A' && c <= 'Z')
        return c - 54;

    if(c >= 'a' && c <= 'z')
        return c - 59;

    if(c >= '0' && c <= '9')
        return c - 47;

    if(c == '_')
        return 37;

    return 0;
}
char numToChar(short n){
    if(n == 0)
        return '-';

    if(n >= 1 && n <= 10)
        return n + 47;

    if(n >= 11 && n <= 36)
        return n + 54;

    if(n == 37)
        return '_';

    if(n >= 36 && n <= 63)
        return n + 59;

    return '?';
}

int ordLG(char* a, char* b){
    int i = 0;
    while(i < wordLength){
        if(a[i] < b[i])
            return i+1; //the string a is before b, returned a positive value
        else if(a[i] > b[i])
            return -i-1; //otherwise a negative value
        i++;
    }
    return 0; //0 means that the string are equal
}

void BSTInsert(char* s, short check){
    node_t *x, *y, *z;
    y = nil;
    x = root;

    if(indAr == L_VET){
        allocateNode();
    }

    z = &arNode[indAr];
    indAr++;

    z->key = (char *) malloc(sizeof(char) * (wordLength+1));
    strcpy(z->key, s);
    z->left = nil;
    z->right = nil;

    if(check == 1){
        if(printable(z->key)){
            z->next = firstList;
            firstList = z;
        }else{
            z->next = NULL;
        }
    }else{
        z->next = NULL;
    }

    while (x != nil) {
        y = x;
        if(ordLG(z->key,x->key) > 0){
            //z is before x
            x = x->left;
            //x is before z
        }else{
            x = x->right;
        }
    }

    if (y == nil) {
        root = z; //tree is void
    } else if(ordLG(z->key,y->key) > 0){
        y->left = z;
    }else{
        y->right = z;
    }

}
node_t* BSTSearch(node_t* n, char* k){
    if(n == nil)
        return  NULL;

    short result = ordLG(k,n->key);
    if(result == 0)
        return n;

    if(result > 0)
        return BSTSearch(n->left, k);
    return BSTSearch(n->right, k);
}

short wordComparison(char* attempt, char* reference, char* output) {
    int count[N_ALF] = {0};
    int countMax[N_ALF] = {0};
    int countMin[N_ALF] = {0};
    int i, c;
    short changes = 0;

    //counting characters in the reference
    for(i = 0; i < wordLength; i++){
        c = charToNum(reference[i]);
        count[c]++;
        countMax[c]++;
        output[i] = '?';
    }

    //right characters
    for(i = 0; i < wordLength; i++){
        if (attempt[i] == reference[i]) {
            c = charToNum(attempt[i]);
            for(int j = 0; j < N_ALF; j++){
                if(c != (char)j){
                    if(matr[i][j] == 1){
                        changes = 1;
                        matr[i][j] = 0;
                    }
                }
            }
            countMin[c]++;
            output[i] = '+';
            count[c]--;
        }
    }

    //wrong characters
    for(i = 0; i < wordLength; i++){
        if(output[i] != '+') {
            c = charToNum(attempt[i]);
            if(count[c] > 0){
                output[i] = '|';
                countMin[c]++;
                count[c]--;
                if(matr[i][c] == 1){
                    changes = 1;
                    matr[i][c] = 0;
                }
            }else{
                output[i] = '/';

                if(countMax[c] == 0){
                    //character forbidden, column of that character to 0
                    for(int j = 0; j < wordLength; j++){
                        if(matr[j][c] == 1){
                            changes = 1;
                            matr[j][c] = 0;
                        }
                    }
                }else{
                    //understood the number of exact occurrences
                    if(nOcc[c] != -countMax[c]){
                        nOcc[c] = -countMax[c];
                        changes = 1;
                    }

                    if(matr[i][c] == 1){
                        changes = 1;
                        matr[i][c] = 0;
                    }
                }
            }
        }
    }

    output[i] = '\0';


    //setting the minimum number of occurrences
    for(i = 0; i < N_ALF; i++){
        if(nOcc[i] >= 0 && nOcc[i] < countMin[i]){
            nOcc[i] = countMin[i];
            changes = 1;
        }
    }

    return changes;
}
int isCorrect(char *output){
    for(int i = 0; i < wordLength; i++)
        if(output[i] != '+')
            return 0;

    return 1;
}
short printable(char *s){
    short count[N_ALF] = {0};
    int car;

    for(int i = 0; i < wordLength; i++){
        car = charToNum(s[i]);
        if(matr[i][car] == 0){
            return 0;
        }
        count[car]++;
    }

    int nOccI, contaI;
    for(int i = 0; i < N_ALF; i++){
        nOccI = nOcc[i];
        contaI = count[i];
        if( (nOccI < 0 && -nOccI != contaI) || (nOccI > 0 && nOccI > contaI) ){
            return 0;
        }
    }

    return 1;
}
void printFiltered(node_t* n){
    if(n == nil)
        return;

    printFiltered(n->left);

    if(n->next != NULL)
        printf("%s\n", n->key);

    printFiltered(n->right);
}
void printAll(node_t* n){
    if(n == nil)
        return;

    printAll(n->left);
    printf("%s\n", n->key);
    printAll(n->right);
}

void allocateNode(){
    indAr = 0;
    arNode = malloc(sizeof(node_t) * L_VET);
}

int updateList() {
    int count;
    node_t *att;

    //searching for the first printable
    while(!printable(firstList->key)) {
        att = firstList;
        firstList = firstList->next;
        att->next = NULL;
    }

    count = 1;

    node_t *tmp = firstList;

    while(tmp->next != lastList && tmp != lastList){
        if(printable(tmp->next->key)){
            tmp = tmp->next;
            count++;
        }else{
            att = tmp->next;
            tmp->next = tmp->next->next;
            att->next = NULL;
        }
    }

    return count;
}
int initList(node_t*n){
    if(n == nil)
        return 0;

    int att, l, r;

    if(printable(n->key)){
        n->next = firstList;
        firstList = n;
        att = 1;
    }else {
        n->next = NULL;
        att = 0;
    }

    r = initList(n->right);
    l = initList(n->left);

    return r+l+att;
}

void initConstraints(){
    for(int i= 0; i < wordLength; i++)
        for(int j = 0; j < N_ALF; j++)
            matr[i][j] = 1;

    for(int i = 0; i < N_ALF; i++){
        nOcc[i] = 0;
    }
}
