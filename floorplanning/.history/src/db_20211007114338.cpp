#include "../include/db.h"

//Point
Point::Point(float x, float y)
{
    _x = x;
    _y = y;
}

//Module
Module::Module(string name)
{
    _name = name;
}

//Node
Node::Node(uint32_t id)
{
    _id = id;
}

//DataService
bool DataService::ReadAreaEtcFile(string file_path)
{
    ifstream file;
    file.open(file_path);

    if(!file.is_open())
    {
        return false;
    }

    string str;//save one line message
    char buffer[10];//save temporary num
    float x, y;
    
    //get area massage
    getline(file, str);
    while (str.find("(") != str.npos)
    {
        size_t pos1 = str.find("(");
        str.erase(0, pos1 + 1);
        pos1 = 0;

        size_t pos2 = str.find(",");
        size_t pos3 = str.find(")");

        size_t len = str.copy(buffer, pos2 - pos1, pos1);
        buffer[len] = '\0';
        x = atof(buffer);
        len = str.copy(buffer, pos3 - pos2 - 2, pos2 + 2);
        buffer[len] = '\0';
        y = atof(buffer);

        Point* area_point = new Point(x, y);
        this->addAreaPoint(area_point);
        str.erase(0, pos3 + 1);
    }
    
    //get rule message,but don't deal with it.
    getline(file, str);

    //get Module message
    while(!file.eof())
    {
        Module* module = new Module();

        //get name
        getline(file, str);
        size_t pos1 = str.find(":");
        str.erase(0, pos1 + 1);
        module->setName(str);

        //get boundary
        getline(file, str);
        while (str.find("(") != str.npos)
        {
            size_t pos1 = str.find("(");
            str.erase(0, pos1 + 1);
            pos1 = 0;

            size_t pos2 = str.find(",");
            size_t pos3 = str.find(")");

            size_t len = str.copy(buffer, pos2 - pos1, pos1);
            buffer[len] = '\0';
            x = atof(buffer);
            len = str.copy(buffer, pos3 - pos2 - 2, pos2 + 2);
            buffer[len] = '\0';
            y = atof(buffer);

            Point* Boundary_point = new Point(x, y);
            module->addBoundaryPoint(Boundary_point);
            str.erase(0, pos3 + 1);
        }

        //get Port
        getline(file, str);
        while (str.find("P") != str.npos)
        {
            Port* port = new Port();
            while (str.find("(") != str.npos)
            {
                size_t pos1 = str.find("(");
                str.erase(0, pos1 + 1);
                pos1 = 0;

                size_t pos2 = str.find(",");
                size_t pos3 = str.find(")");

                size_t len = str.copy(buffer, pos2 - pos1, pos1);
                buffer[len] = '\0';
                x = atof(buffer);
                len = str.copy(buffer, pos3 - pos2 - 2, pos2 + 2);
                buffer[len] = '\0';
                y = atof(buffer);

                Point* point = new Point(x, y);
                port->addPoint(point);
                str.erase(0, pos3 + 1);
            }
        }

        this->addModule(module);
    }

    return true;
}
