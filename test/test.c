#include <stdio.h>
#include <inttypes.h> // Sử dụng thư viện inttypes.h để định nghĩa uint8_t

int main() {
    // uint8_t array[2] = {0xA9, 128}; // Notation using hexadecimal values
    // if(array[0] & 0x80){
    //     printf("The most significant bit of array[0] is set (1)\n");
    // }

    // if (array[1] & 0x80) { // Use 0x80 to represent binary 10000000
    //     printf("The most significant bit of array[1] is set (1)\n");
    // }
    int a = 6;
    int b =6;
    if(a==b){
        printf("ok\n");
    }
    if( a & b){
        printf("hi");
    }
}
