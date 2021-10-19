#include "../include/db.h"

int main(int argc, char const *argv[])
{
    DataService* case1 = new DataService();
    case1->readAreaEtcFile("./../input/mycase");
    //case1->readLinkFile("./../input/Ports_link_input.txt_1.txt");
    // case1->initialSortModules();
    case1->initBStarTree();
    case1->getLeftXOnAreaLimit(342.5, 411.6);
    case1->writeGDS();
    return 0;
}
