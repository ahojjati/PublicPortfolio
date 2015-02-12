#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MAX_MEM_POOL = 409;
char* memoryPool = NULL;
char* currentStartAddr = 0;

char* getBuffer(int size){
    if(memoryPool == NULL){
        memoryPool = (char*)malloc(sizeof(char)*MAX_MEM_POOL);
        currentStartAddr = memoryPool;
    }
    if(currentStartAddr >= memoryPool+MAX_MEM_POOL){
        printf("\nError: Local memory pool overflow.");
    }
    char* retAddr = currentStartAddr;
    currentStartAddr = currentStartAddr + size;
    return retAddr;
}

void declare_variable(char **inputName, char **destName, char **titleString, char **destTitleString, char *username){
    *inputName = getBuffer(10);
    strcpy(*inputName, username);
    *destName = getBuffer(10);
    *titleString = getBuffer(16);
    *destTitleString = getBuffer(16);
}

void name_relocate(char *inputName, char *destName){
    strcpy(destName, inputName); 
}

void title_relocate(char *titleString, char *destTitleString){
    titleString = "Your Name =";
    strcpy(destTitleString, titleString);
}

void data_relocate(char *inputName, char *destName, char *titleString, char *destTitleString){
    name_relocate(inputName, destName);
    title_relocate(titleString,destTitleString);
}

void print_data(char *destName, char *destTitleString){
    printf("\n\n%s ",destTitleString);
    printf("%s",destName);
    printf("\n\n");
}
void error_message(){
    printf("\nError: Incorrect input argument.\n\nUsage: <this executable> <printable ASCII string>\n\n");
}

int main(int argc, char **argv) {
    int i;
    int j; //not used for now
    int q = 0; //also not used
    if (argc < 2) {
        error_message();
        exit(0);
    }
    char *inputName;
    char *destName;
    char *titleString;
    char *destTitleString;

    if(strlen(argv[1]) > MAX_MEM_POOL)
    {
        error_message();
        exit(0);
    }
    char nu[409];
    for(i = 0; i < 409; i++)
        nu[i] = '\0';
    strcpy(nu,argv[1]);
    for (i = 0; i < strlen(nu); ++i)
    {
        if (nu[i] < 0x20 || nu[i] > 127)
        {
            error_message();
            exit(0);
        }
    }
    declare_variable(&inputName, &destName, &titleString, &destTitleString, nu);
    data_relocate(inputName, destName, titleString, destTitleString);    
    print_data(destName, destTitleString);

    return 0;
}
