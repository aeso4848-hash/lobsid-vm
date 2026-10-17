#include <stdio.h>
#include <string.h>

int readhex(char *number) {
    int num = 0;
    for (int i = 0; i < 4; i++) {
        num *= 16;
        if(number[i] <= '9' && number[i] >= '0'){
            num += number[i] - '0';
        } else if (number[i] <= 'F' && number[i] >= 'A') {
            num += number[i] - 'A' + 10;
        }
    }
    return num;
}

typedef struct{
    char name[4];
    int value;
} pin;

int main() {
    unsigned char ram[4096];
    pin pins[64];
    for(int k = 0; k < 64; k++) pins[k].name[0] = '\0';
    int acc = 0;
    int bool_flag = 0;

    FILE *instructions = fopen("instructions","r");

    char lines[512][8];
    for(int i = 0; i < 512; i++) {
        for(int j = 0; j < 8; j++) lines[i][j] = '\0';
    }

    int c;
    int line = 0;
    int ch = 0;

    while ((c = fgetc(instructions)) != EOF) {
        if (c == '\n') {
            ch = 0;
            line++;
            continue;
        }
        lines[line][ch] = c;
        if (ch < 7) {
            ch++;
        } else {
            ch = 0;
            line++;
        }
    }
    fclose(instructions);

    for (int i = 0; i < 512; i++) {
        char mnemonic[4];
        strncpy(mnemonic, lines[i], 3);
        mnemonic[3] = '\0';

        char value[4];
        strncpy(value, lines[i] + 4, 3);
        value[3] = '\0';

        if (strcmp(mnemonic, "pop") == 0) {
            acc = ram[readhex(value)];
        } else if (strcmp(mnemonic, "psh") == 0) {
            ram[readhex(value)] = acc;
        } else if (strcmp(mnemonic, "add") == 0) {
            acc += ram[readhex(value)];
        } else if (strcmp(mnemonic, "sub") == 0) {
            acc -= ram[readhex(value)];
        } else if (strcmp(mnemonic, "mlt") == 0) {
            acc *= ram[readhex(value)];
        } else if (strcmp(mnemonic, "div") == 0) {
            acc /= ram[readhex(value)];
        } else if (strcmp(mnemonic, "inc") == 0) {
            ram[readhex(value)]++;
        } else if (strcmp(mnemonic, "dec") == 0) {
            ram[readhex(value)]--;
        } else if (strcmp(mnemonic, "cmp") == 0) {
            bool_flag = acc > ram[readhex(value)];
        } else if (strcmp(mnemonic, "pin") == 0) {
            for (int item = 0; item < 64; item++) {
                if(pins[item].name[0] == '\0') {
                    strcpy(pins[item].name, value);
                    pins[item].value = i;
                    break;
                }
            }
        } else if (strcmp(mnemonic, "jmp") == 0) {
            if(bool_flag) {
                for (int item = 0; item < 64; item++) {
                    if(strcmp(pins[item].name, value) == 0) {
                        i = pins[item].value;
                    } } }
        } else if (strcmp(mnemonic, "out") == 0) {
            printf("%c", ram[readhex(value)]);
        } else if (strcmp(mnemonic, "ask") == 0) {
            printf(">");
            ram[readhex(value)] = getchar();
        } else if (strcmp(mnemonic, "dbl") == 0) {
            acc *= 2;
        } else if (strcmp(mnemonic, "hlf") == 0) {
            acc /= 2;
        } else if (strcmp(mnemonic, "clr") == 0) {
            for (int item = 0;item < 4096;item++) {
                ram[item] = 0;
            }
        }
    }
    return 0;
}
