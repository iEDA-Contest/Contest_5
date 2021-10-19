#include <stdio.h>
int f(int x)
{
    if(x>-2)
    return 3*x+2;
    return f(x+2)-f(x+1);
}

int main()
{
    printf("%d", f(-9));
    return 0;
}
