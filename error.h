
#pragma once

__attribute__((__noreturn__)) void panic(char *file,int line,char *format,...);

#define panic(...) panic(__FILE__,__LINE__,__VA_ARGS__)
#define assert(arg) {if(!(arg)){panic("failed assertion: %s",#arg);}}


