#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
char* Mystrstr(const char* str1, const char* str2) {
    char* cur =(char*) str1;
    while (*cur) {
        char* s1 = cur;
        char* s2 = str2;
        while (*s1 == *s2 && *s1 != '\0' && *s2 != '\0') {
            s1++;
            s2++;
        }
        if (*s2 == '\0') {
            return cur;
        }
        cur++;
    }
    return NULL;
}
int main(){
    char* s1 = "hell  oea";
    char* s2 = "o";
    printf("%s\n",Mystrstr(s1, s2));
    
    return 0;
}