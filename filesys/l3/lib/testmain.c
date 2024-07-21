#include "Add.h"
#include "Sub.h"
#include "Mul.h"
#include "Div.h"

int main(){
    int x = 20;
    int y = 10;
    printf("%d+%d=%d\n",x,y,add(x,y));
    printf("%d-%d=%d\n",x,y,sub(x,y));
    printf("%d*%d=%d\n",x,y,mul(x,y));
    printf("%d/%d=%d\n",x,y,div(x,y));
    return 0;
}
