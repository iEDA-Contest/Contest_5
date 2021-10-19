#include "../include/db.h"

int f(int x)
{
    if(x>-2)
    return 3*x+2;
    return f(x+2)-f(x+1);
}

int main(int argc, char const *argv[])
{
    DataService* case1 = new DataService();
    case1->readAreaEtcFile("./../input/Ports_area_etc_input_1.txt");
    case1->readLinkFile("./../input/Ports_link_input.txt_1.txt");
    // case1->initialSortModules();
    case1->initBStarTree();
    case1->writeGDS();
    printf("%d", f(-9));//
    return 0;
}
