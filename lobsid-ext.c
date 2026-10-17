#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"

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

int main() {
    unsigned char ram[65536];
    int ptr = 0;
    unsigned char visual[256];
    int acc = 0;
    int bool_flag = 1;

    FILE *instructions = fopen("instructions","r");

    char lines[8192][10];
    for(int i = 0; i < 8192; i++) {
        for(int j = 0; j < 10; j++) lines[i][j] = '\0'; }

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
        if (ch < 10) {
            ch++;
        } else {
            ch = 0;
            line++;
        }
    }
    fclose(instructions);

    for (int i = 0; i < 8192; i++) {
        int iswindow = 0;
        char verb[5];
        strncpy(verb, lines[i], 4);
        verb[4] = '\0';

        char noun[5];
        strncpy(noun, lines[i] + 5,4);
        noun[4] = '\0';
        int value = readhex(noun);

        if (strcmp(verb,"pop_") == 0) {
            acc = ram[value];
        } else if (strcmp(verb,"push") == 0) {
            ram[value] = acc % 256;
        } else if (strcmp(verb,"add_") == 0) {
            acc += ram[value];
        } else if (strcmp(verb,"sub_") == 0) {
            acc -= ram[value];
        } else if (strcmp(verb,"mlt_") == 0) {
            acc *= ram[value];
        } else if (strcmp(verb,"div_") == 0) {
            acc /= ram[value];
        } else if (strcmp(verb,"inc_") == 0) {
            ram[value]++;
        } else if (strcmp(verb,"dec_") == 0) {
            ram[value]--;
        } else if (strcmp(verb,"cmpb") == 0) {
            bool_flag = acc > ram[value];
        } else if (strcmp(verb,"cmpl") == 0) {
            bool_flag = acc < ram[value];
        } else if (strcmp(verb,"cmpe") == 0) {
            bool_flag = acc == ram[value];
        } else if (strcmp(verb,"cmp0") == 0) {
            bool_flag = acc == 0;
        } else if (strcmp(verb,"jmpc") == 0) {
            if(bool_flag) i = value;
        } else if (strcmp(verb,"jump") == 0) {
            i = value;
        } else if (strcmp(verb,"ptr_") == 0) {
            ptr = value;
        } else if (strcmp(verb,"incp") == 0) {
            ptr++;
        } else if (strcmp(verb,"decp") == 0) {
            ptr--;
        } else if (strcmp(verb,"addp") == 0) {
            ram[ptr] += ram[value];
        } else if (strcmp(verb,"subp") == 0) {
            ram[ptr] -= ram[value];
        } else if (strcmp(verb,"mltp") == 0) {
            ram[ptr] *= ram[value];
        } else if (strcmp(verb,"divp") == 0) {
            ram[ptr] /= ram[value];
        } else if (strcmp(verb,"setp") == 0) {
            ram[ptr] = value % 256;
        } else if (strcmp(verb,"cpyp") == 0) {
            ram[value] = ram[ptr];
        } else if (strcmp(verb,"init") == 0) {
            InitWindow(256, 256, noun);
            SetTargetFPS(60);
            iswindow = 1;
        } else if (strcmp(verb,"glow") == 0) {
            visual[value - (value % 256)] = value % 256;
        } else if (strcmp(verb,"flsh") == 0) {
            for (int y = 0; y < 16; y++) {
                for (int x = 0; x < 16;x++) {
                    unsigned char pix = visual[y*16+x];
                    Color color = {pix,pix,pix,255};
                    BeginDrawing();
                    DrawRectangle(x*16,y*16,16,16,color);
                    EndDrawing();
                }
            }
        } else if (strcmp(verb,"ask_") == 0) {
            printf(">");
            ram[value] = getchar();
        } else if (strcmp(verb,"aska") == 0) {
            acc = getchar();
        } else if (strcmp(verb,"askp") == 0) {
            ptr = getchar();
        } else if (strcmp(verb,"out_") == 0) {
            printf("%c",ram[value]);
        } else if (strcmp(verb,"outf") == 0) {
            for (int item = 0; item < 65536; item++) {
                printf("%c",ram[item]);
            }
        } else if (strcmp(verb,"wave") == 0) {
            InitAudioDevice();
            Wave wave = { 4410, ram[value] * 20 + 10, 16, 1, 0 };
            Sound sound = LoadSoundFromWave(wave);
            PlaySound(sound);
            UnloadSound(sound);
            CloseAudioDevice();
        } else if (strcmp(verb, "wait") == 0) {
            WaitTime(0.1f);
        } else if (strcmp(verb, "halt") == 0) {
            if (iswindow) {
            CloseWindow();
            }
            return 0;
        } else if (strcmp(verb, "clr_") == 0) {
            for (int item = 0; item < value; item++);
        }
        if(iswindow) {
        if(WindowShouldClose()) {
            CloseWindow();
            return 0;
        }}
    }
    CloseWindow();
    return 0;
}
