#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./proto/test.pb.h" // 刚才生成的文件
using namespace ssp;

int isalpha(char ch) {
    if ('0' <= ch && ch <= '9') {
        return 1;
    }
    if ('a' <= ch && ch <= 'z') {
        return 1;
    }
    if ('A' <= ch && ch <= 'Z') {
        return 1;
    }
    if (ch == '_') {
        return 1;
    }
    return 0;
}
void hex_print(char ch) {
    if (isalpha(ch) == 2) {
        printf("%d %c\n", ch, ch);
    } else {
        int b1;
        int b2;
        int b3;
        int b4;
        int b5;
        int b6;
        int b7;
        int b8;
        b8 = ch & 1;
        b7 = (ch >> 1) & 1;
        b6 = (ch >> 2) & 1;
        b5 = (ch >> 3) & 1;
        b4 = (ch >> 4) & 1;
        b3 = (ch >> 5) & 1;
        b2 = (ch >> 6) & 1;
        b1 = (ch >> 7) & 1;

        printf("%d %c %d%d%d%d %d%d%d%d\n", ch, ch, b1, b2, b3, b4, b5, b6, b7, b8);
    }
}

int main() {
    ssp::UserInfoBase ui;
    ui.set_ver(1);
    ui.set_user_id(10001);
    ui.set_user_name("hank1234");
    ui.set_password("12345678");
    ui.set_max_t(63);
    printf("%d %d %s %s\n", 
        ui.ver(), 
        ui.user_id(), 
        ui.user_name().c_str(), 
        ui.password().c_str());
    char buffer[1024];
    ui.SerializeToArray(buffer, 1024);
    int len = strlen(buffer);
    printf("buffer: %d %s\n", len, buffer);
    int h8 = 10001 / 256;
    int l8 = 10001 % 256;
    hex_print(h8);
    hex_print(l8);
    for (int i = 0; i < len; i++) {
        hex_print(buffer[i]);
    }
    return 0;
}