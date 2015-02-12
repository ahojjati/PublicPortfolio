#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct User {
    int (*func)(const char*);
    int (*count)(const char*);
    unsigned int id;
    unsigned long ssn;
    char name[73];
    char notes[198];
} User;

User* users;
char *names[] = {"Blake Tucker", "Maria Grant", "Victor Coleman", "Brian Wright", "Theresa Scott"};
long ssns[] = {453247574, 300274885, 538740265, 372613257, 372851715};
int num_users = 5;

int print_id(const char* name) {
    for(int i=0;i<num_users;i++) {
        if(strcmp(users[i].name,name) == 0) {
            printf("%s: %d\n",name,users[i].id);
            return i;
        }
    }
    return -1;
}

int print_ssn(const char* name) {
    for(int i=0;i<num_users;i++) {
        if(strcmp(users[i].name,name) == 0) {
            printf("%s: %lu\n",name,users[i].ssn);
            return i;
        }
    }
   return -1;
}

int count(const char* arg) {
    return strlen(arg);
}

void init_users() {
    for(int i=0;i<num_users;i++) {
        users[i].func = print_id;
        users[i].count = count;
        users[i].id = i;
        users[i].ssn = ssns[i];
        strcpy(users[i].name, names[i]);
    }
}

void set_notes(char* arg) {
    for(int i=0;i<num_users;i++)
        strcpy(users[i].notes, arg);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("\nNot enough arguments.\n\n");
        exit(0);
    }

    users = (User*) malloc(num_users*sizeof(User));
    init_users();
    set_notes(argv[1]);
    
    int z = 0;
    for(int i=0;i<num_users;i++) {
        users[i].func(users[i].name);
        z += users[i].count(users[i].notes);
    }
    return 0;
}
