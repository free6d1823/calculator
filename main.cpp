#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

using namespace std;
static int bytes_per_row = 10;
void usage(char* name)
{
    printf("Usage: %s <operand1>[<op><oprand2>]\n", name);
}
#define char2i(d) (d>= '0' && d <= '9')?(d-'0'): \
                     ( (d >= 'a' && d<= 'f')?(d-'a'+10): \
                       (d >= 'A' && d<= 'F')?(d-'A'+10):0 )

off_t hex2long(char* hex)
{
    off_t val = 0;
    char* p;
    int start = 0;
    for(p=hex; *p != 0; p++) {
        if (!start) {
            if (*p == 'x' || *p == 'X')
                start=1;
            continue;
        }
        val <<= 4;
        val += char2i(*p);
    }
    if(!start) { //not hex, try decimal
        val = atoi(hex);
    }
    return val;

}
#define IS_OP(c) (c=='+'||c=='-'||c=='*'||c=='/')
int ParserString(char* text, long* v1, long* v2, int* type)
{
    int op = 0;
	char value[2][32] = {"0", "0"};
    int index = 0;
    long va, vb;
    char*p1, *p2;
    p1 = text;
    for(p2=text; *p2 != 0 && *p2!= ' '; p2++){
        if (IS_OP(*p2)){
            op = *p2;
            if(p1 != p2)
                memcpy(value[index], p1, p2-p1);
            value[index][p2-p1] = 0;
            p2++;
            p1=p2;
            index ++;        
        }
        if (index == 2)
            break;
    }
    if (index <2 && p1 != p2) {
            memcpy(value[index], p1, p2-p1);
            value[index][p2-p1] = 0;
    }
    va = hex2long(value[0]);
    vb = hex2long(value[1]);
    *v1 = va;
    *v2 = vb;
    *type = 0;
    return op;
}
int main(int argc, char *argv[])
{
    int op = 0;
    int type = 0;
    long a,b,c;
    if (argc<2) {
        usage(argv[0]);
        exit(-1);
    }
    op = ParserString(argv[1], &a, &b, &type);
    switch(op)
    {
    case '+':
        c = a+b;
        break;
    case '-':
        c = a-b;
        break;
    case '*':
        c = a*b;
        break;
    case '/':
        c = a/b;
        break;
    default:
        c = a;
        break;
    }
    printf("0x%08lX %ld\n", c,c);
    return 0;
}

