#include <stdio.h>

int main()
{
    char str[1000];
    while (fgets(str, 999, stdin) != NULL) {
        char s[100];
        if(sscanf(str, "mem_stacks_B=%s", s) == 1)
            puts(s);
    }
}
