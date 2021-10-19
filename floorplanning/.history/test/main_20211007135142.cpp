#include "../include/db.h"

int main(int argc, char const *argv[])
{
    DataService* case1 = new DataService();
    case1->ReadAreaEtcFile("/home/yangzonglin/case/Ports_area_etc_input_1.txt");
    case1->ReadLinkFile("/home/yangzonglin/case/Ports_link_input.txt_1.txt");
    return 0;
}
