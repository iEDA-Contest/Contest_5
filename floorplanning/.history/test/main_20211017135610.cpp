#include "../include/db.h"

int main(int argc, char const *argv[])
{
    DataService* case1 = new DataService();
    case1->readAreaEtcFile("./../input/mycase.txt");//(-100, 150)(10, 120)(-50, 100)(50, 50)(-30, 10)
    //case1->readLinkFile("./../input/Ports_link_input.txt_1.txt");
    //// case1->initialSortModules();
    //case1->initBStarTree();
    case1->getLeftXOnAreaLimit(190, 10);
    case1->writeGDS();
    return 0;
}
