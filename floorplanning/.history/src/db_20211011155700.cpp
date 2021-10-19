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

void Module::arrangePoints()
{
    for (size_t i = 0; i < _boundary.size() - 1; i++)
    {
        for (size_t j = 0; j < _boundary.size() - 1 - i; j++)
        {
            if(_boundary[j + 1]->getY() > _boundary[j]->getY())
            {
                Point* temp      = _boundary[j];
                _boundary[j]     = _boundary[j + 1];
                _boundary[j + 1] = temp;
            }
        }
    }
    for (size_t i = 0; i < _boundary.size() - 1; i++)
    {
        for (size_t j = 0; j < _boundary.size() - 1 - i; j++)
        {
            if(_boundary[j + 1]->getX() < _boundary[j]->getX())
            {
                Point* temp      = _boundary[j];
                _boundary[j]     = _boundary[j + 1];
                _boundary[j + 1] = temp;
            }
        }
    }
}

void Module::cutIntoRect()
{
    switch (this->getShape())
    {
    case Shape::LSHAPE :
    {
        float left_reduce  = _boundary[0]->getY() - _boundary[1]->getY();
        float right_reduce = _boundary[4]->getY() - _boundary[5]->getY();
        float lx_1, ly_1, hx_1, hy_1;//Coordinate extremum of left rectangle.
        float lx_2, ly_2, hx_2, hy_2;//Coordinate extremum of right rectangle.
       
        //set left rectangle points
        lx_1 = _boundary[0]->getX();
        ly_1 = _boundary[1]->getY();
        hx_1 = _boundary[2]->getX();
        hy_1 = ly_1 + left_reduce;

        Module* sub_module_left= new Module();

        Point* left_up_left = new Point(lx_1, hy_1);
        sub_module_left->addBoundaryPoint(left_up_left);
        Point* left_low_left = new Point(lx_1, ly_1);
        sub_module_left->addBoundaryPoint(left_low_left);
        Point* right_low_left = new Point(hx_1, ly_1);
        sub_module_left->addBoundaryPoint(right_low_left);
        Point* right_up_left = new Point(hx_1, hy_1);
        sub_module_left->addBoundaryPoint(right_up_left);
            
        this->addSubModule(sub_module_left);

        //set right rectangle points
        lx_2 = _boundary[2]->getX();
        ly_2 = _boundary[5]->getY();
        hx_2 = _boundary[4]->getX();
        hy_2 = ly_2 + right_reduce;

        Module* sub_module_right= new Module();

        Point* left_up_right = new Point(lx_2, hy_2);
        sub_module_right->addBoundaryPoint(left_up_right);
        Point* left_low_right = new Point(lx_2, ly_2);
        sub_module_right->addBoundaryPoint(left_low_right);
        Point* right_low_right = new Point(hx_2, ly_2);
        sub_module_right->addBoundaryPoint(right_low_right);
        Point* right_up_right = new Point(hx_2, hy_2);
        sub_module_right->addBoundaryPoint(right_up_right);
            
        this->addSubModule(sub_module_right);

        break;
    }
    case Shape::TSHAPE :
    {

        break;
    }
    default:
        break;
    }
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
    getline(file, str);
    while(!file.eof())
    {
        Module* module = new Module();

        //get name
        size_t pos1 = str.find(":");
        str.erase(0, pos1 + 1);
        module->setName(str);

        //get boundary
        getline(file, str);
        uint32_t pointsNum = 0;
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
            ++pointsNum;
        }

        module->arrangePoints();

        switch (pointsNum)
        {
        case 6:
            module->setShape(Shape::LSHAPE);
            module->cutIntoRect();
            break;
        case 8:
            module->setShape(Shape::TSHAPE);
            module->cutIntoRect();
            break;

        default:
            break;
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
            module->addPort(port);
            getline(file, str);
        }

        this->addModule(module);
    }

    return true;
}

Module* DataService::findModule(string name)
{
    for (Module* module : this->getModules())
    {
        if(module->getName() == name)
        {
            return module;
        }
    }
    
    return nullptr;
}

bool DataService::ReadLinkFile(string file_path)
{
    ifstream file;
    file.open(file_path);

    if(!file.is_open())
    {
        return false;
    }

    //delete Link name
    string temp;
    getline(file, temp);
    while (!file.eof())
    {
        vector<Port*> net;
        string moduleName, portId;
        getline(file, moduleName);
        moduleName.append(" ");
        getline(file, portId);
        portId.append(" ");

        while (!moduleName.empty())
        {
            size_t pos1 = moduleName.find(" ");
            string name = moduleName.substr(0, pos1);
            moduleName.erase(0, pos1 + 1);
            size_t pos2 = portId.find(" ");
            uint32_t id = atoi(portId.substr(0, pos2).c_str());
            portId.erase(0, pos2 + 1);

            Module* module = this->findModule(name);
            Port* port = module->getPorts()[id - 1];
            
            port->setNet(&net);
            net.push_back(port);
        }
        _nets.push_back(net);
        getline(file, temp);
    }

    return true;
}
