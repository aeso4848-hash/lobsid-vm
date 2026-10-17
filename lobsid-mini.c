#include<stdio.h>
#include <string.h>

int hex(char *number) {
    int num = 0;
    for (int i = 0;i < 2;i++) {
        num *= 16;
        if(number[i] <= '9' && number[i] >= '0') {
            num += number[i] - '0'; }
        if(number[i] <= 'F' && number[i] >= 'A') {
            num += number[i] - 'A' + 10;
        }
    }
    return num;
}

typedef struct{
    char name[3];
    int value;
} pin;

int main() {
    unsigned char ram[256];
    pin pins[16];
    for(int k = 0; k <16; k++) pins[k].name[0] = '\0';
    int acc = 0;
    int bool_flag = 1;

    FILE *instructions = fopen("instructions","r");

    char lines [1024][6];
    for(int i = 0; i < 1024; i++) {
        for(int j = 0; j < 6; j++) lines[i][j] = '\0';
    }

    int c;
    int line = 0;
    int ch = 0;

    while((c = fgetc(instructions)) != EOF) {
        if (c == '\n') {
            ch = 0;
            line++;
            continue;
        }
        lines[line][ch] = c;
        if(ch < 7){
            ch++;
        } else {
            ch = 0;
            line++;
        }
    }
    fclose(instructions);

    for(int i = 0; i < 1024; i++) {
        char mnemonic[3];
        strncpy(mnemonic, lines[i], 2);
        mnemonic[2] = '\0';

        char noun[3];
        strncpy(noun, lines[i] + 4, 2);
        noun[2] = '\0';

        int value = hex(noun);
        if (strcmp(mnemonic, "pp") == 0) {
            acc = ram[value];
        } else if (strcmp(mnemonic, "ps") == 0) {
            ram[value] = acc;
        } else if (strcmp(mnemonic, "ic") == 0) {
            ram[value]++;
        } else if (strcmp(mnemonic, "dc") == 0) {
            ram[value]--;
        } else if (strcmp(mnemonic, "cp") == 0) {
            bool_flag = acc > ram[value];
        } else if (strcmp(mnemonic, "pn") == 0) {
            for (int item = 0; item < 16; item++) {
                if (pins[item].name[0] == '\0') {
                    strcpy(pins[item].name, noun);
                    pins[item].value = i;
                    break;
                }
            }
        } else if (strcmp(mnemonic, "jp") == 0) {
            if(bool_flag){
                for (int item = 0;item < 16; item++) {
                    if (strcmp(pins[item].name, noun) == 0) {
                        i = pins[item].value;
                    } } }
        } else if (strcmp(mnemonic, "ot") == 0) {
            printf("%c", ram[value]);
        } else if (strcmp(mnemonic, "in") == 0) {
            printf(">");
            ram[value] = getchar();
        } else if (strcmp(mnemonic, "sm") == 0) {
            acc += ram[value];
        }
    }
    return 0;
}
