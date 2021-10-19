#include "../include/floorPlanning.h"

int main(int argc, char const *argv[])
{
    DataService* case1 = new DataService();
    case1->readAreaEtcFile("./../input/mycase.txt");
    //case1->readLinkFile("./../input/Ports_link_input.txt_1.txt");
    //// case1->initialSortModules();
    //case1->initBStarTree();
    case1->getLeftXOnAreaLimit(0, 120);
    case1->getRightXOnAreaLimit(0, 100);
    case1->getTopYOnAreaLimit(60, 200);
    case1->getBottomYOnAreaLimit(0, 400);
    case1->writeGDS();
    return 0;
}
