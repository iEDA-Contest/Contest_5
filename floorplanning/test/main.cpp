#include "../include/floorPlanning.h"

int main(int argc, char const *argv[])
{
    DataService* case1 = new DataService();
    //case1->readAreaEtcFile("./../input/mycase.txt");
    case1->readAreaEtcFile("./../input/Ports_area_etc_input_1.txt");
    case1->readLinkFile("./../input/Ports_link_input.txt_1.txt");
    //// case1->initialSortModules();
    case1->simulatedAnnealing();
    return 0;
}
