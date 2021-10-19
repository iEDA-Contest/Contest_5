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
    vector<Point*> boundary = _boundary;
    for (size_t i = 0; i < boundary.size() - 1; i++)
    {
        for (size_t j = 0; j < boundary.size() - 1 - i; j++)
        {
            if(boundary[j + 1]->getY() > boundary[j]->getY())
            {
                Point* temp      = boundary[j];
                boundary[j]     = boundary[j + 1];
                boundary[j + 1] = temp;
            }
        }
    }
    for (size_t i = 0; i < boundary.size() - 1; i++)
    {
        for (size_t j = 0; j < boundary.size() - 1 - i; j++)
        {
            if(boundary[j + 1]->getX() < boundary[j]->getX())
            {
                Point* temp      = boundary[j];
                boundary[j]     = boundary[j + 1];
                boundary[j + 1] = temp;
            }
        }
    }
    this->cutIntoRect(boundary);
}

void Module::cutIntoRect(vector<Point*> boundarys)
{
    switch (this->getShape())
    {
    case Shape::LSHAPE :
    {
        float left_height  = boundarys[0]->getY() - boundarys[1]->getY();
        float right_height = boundarys[4]->getY() - boundarys[5]->getY();
        float lx_1, ly_1, hx_1, hy_1;//Coordinate extremum of left rectangle.
        float lx_2, ly_2, hx_2, hy_2;//Coordinate extremum of right rectangle.
       
        //set left rectangle points
        lx_1 = boundarys[0]->getX();
        ly_1 = boundarys[1]->getY();
        hx_1 = boundarys[2]->getX();
        hy_1 = ly_1 + left_height;

        Module* sub_module_left= new Module();

        Point* left_low_left = new Point(lx_1, ly_1);
        sub_module_left->addBoundaryPoint(left_low_left);
        Point* right_low_left = new Point(hx_1, ly_1);
        sub_module_left->addBoundaryPoint(right_low_left);
        Point* right_up_left = new Point(hx_1, hy_1);
        sub_module_left->addBoundaryPoint(right_up_left);
        Point* left_up_left = new Point(lx_1, hy_1);
        sub_module_left->addBoundaryPoint(left_up_left);
            
        this->addSubModule(sub_module_left);

        //set right rectangle points
        lx_2 = boundarys[2]->getX();
        ly_2 = boundarys[5]->getY();
        hx_2 = boundarys[4]->getX();
        hy_2 = ly_2 + right_height;

        Module* sub_module_right= new Module();

        Point* left_low_right = new Point(lx_2, ly_2);
        sub_module_right->addBoundaryPoint(left_low_right);
        Point* right_low_right = new Point(hx_2, ly_2);
        sub_module_right->addBoundaryPoint(right_low_right);
        Point* right_up_right = new Point(hx_2, hy_2);
        sub_module_right->addBoundaryPoint(right_up_right);
        Point* left_up_right = new Point(lx_2, hy_2);
        sub_module_right->addBoundaryPoint(left_up_right);
            
        this->addSubModule(sub_module_right);

        break;
    }
    case Shape::TSHAPE :
    {
        float lx_1, ly_1, hx_1, hy_1;//Coordinate extremum of left rectangle.
        float lx_2, ly_2, hx_2, hy_2;//Coordinate extremum of mid rectangle.
        if(boundarys[2]->getX() == boundarys[4]->getX())//T shape cut into two rectangles
        {
            float left_height  = boundarys[0]->getY() - boundarys[1]->getY();
            float right_height = boundarys[6]->getY() - boundarys[7]->getY();

            lx_1 = boundarys[0]->getX();
            ly_1 = boundarys[1]->getY();
            hx_1 = boundarys[2]->getX();
            hy_1 = ly_1 + left_height;

            //set left rectangle points
            Module* sub_module_left= new Module();

            Point* left_low_left = new Point(lx_1, ly_1);
            sub_module_left->addBoundaryPoint(left_low_left);
            Point* right_low_left = new Point(hx_1, ly_1);
            sub_module_left->addBoundaryPoint(right_low_left);
            Point* right_up_left = new Point(hx_1, hy_1);
            sub_module_left->addBoundaryPoint(right_up_left);
            Point* left_up_left = new Point(lx_1, hy_1);
            sub_module_left->addBoundaryPoint(left_up_left);
                
            this->addSubModule(sub_module_left);

            lx_2 = boundarys[2]->getX();
            ly_2 = boundarys[7]->getY();
            hx_2 = boundarys[6]->getX();
            hy_2 = ly_2 + right_height;

            //set right rectangle points
            Module* sub_module_right= new Module();

            Point* left_low_right = new Point(lx_2, ly_2);
            sub_module_right->addBoundaryPoint(left_low_right);
            Point* right_low_right = new Point(hx_2, ly_2);
            sub_module_right->addBoundaryPoint(right_low_right);
            Point* right_up_right = new Point(hx_2, hy_2);
            sub_module_right->addBoundaryPoint(right_up_right);
            Point* left_up_right = new Point(lx_2, hy_2);
            sub_module_right->addBoundaryPoint(left_up_right);
                
            this->addSubModule(sub_module_right);
        }
        else//T shape cut into three rectangles
        {
            float lx_3, ly_3, hx_3, hy_3;//Coordinate extremum of right rectangle.
            //set all x
            lx_1 = boundarys[1]->getX();
            hx_1 = lx_2 = boundarys[2]->getX();
            hx_2 = lx_3 = boundarys[4]->getX();
            hx_3 = boundarys[6]->getX();

            //set left and right sub module's y
            ly_1 = boundarys[1]->getY();
            hy_1 = boundarys[0]->getY();
            ly_3 = boundarys[7]->getY();
            hy_3 = boundarys[6]->getY();

            //set mid sub module's y
            if(boundarys[2]->getY() == boundarys[4]->getY())
            {
                if(boundarys[3]->getY() == boundarys[5]->getY())
                {
                    ly_2 = boundarys[1]->getY() < boundarys[3]->getY() ? boundarys[1]->getY() : boundarys[3]->getY();
                    hy_2 = hy_1 > boundarys[2]->getY() ? hy_1 : boundarys[2]->getY();
                }
                else
                {
                    ly_2 = ly_1;
                    hy_2 = boundarys[2]->getY();
                }
            }
            else if(boundarys[3]->getY() == boundarys[5]->getY())
            {
                ly_2 = boundarys[3]->getY();
                hy_2 = hy_1;
            }
            else if(boundarys[2]->getY() > boundarys[4]->getY())
            {   
                ly_2 = boundarys[1]->getY();
                hy_2 = boundarys[6]->getY();
            }
            else if(boundarys[2]->getY() < boundarys[4]->getY())
            {
                ly_2 = boundarys[7]->getY();
                hy_2 = boundarys[0]->getY();
            }else
            {
                cout << "cut rect error!!!!!!\n";
            }

            //set left rectangle points
            Module* sub_module_left= new Module();

            Point* left_low_left = new Point(lx_1, ly_1);
            sub_module_left->addBoundaryPoint(left_low_left);
            Point* right_low_left = new Point(hx_1, ly_1);
            sub_module_left->addBoundaryPoint(right_low_left);
            Point* right_up_left = new Point(hx_1, hy_1);
            sub_module_left->addBoundaryPoint(right_up_left);
            Point* left_up_left = new Point(lx_1, hy_1);
            sub_module_left->addBoundaryPoint(left_up_left);
                
            this->addSubModule(sub_module_left);

            //set mid rectangle points
            Module* sub_module_mid= new Module();

            Point* left_low_mid = new Point(lx_2, ly_2);
            sub_module_mid->addBoundaryPoint(left_low_mid);
            Point* right_low_mid = new Point(hx_2, ly_2);
            sub_module_mid->addBoundaryPoint(right_low_mid);
            Point* right_up_mid = new Point(hx_2, hy_2);
            sub_module_mid->addBoundaryPoint(right_up_mid);
            Point* left_up_mid = new Point(lx_2, hy_2);
            sub_module_mid->addBoundaryPoint(left_up_mid);
                
            this->addSubModule(sub_module_mid);

            //set right rectangle points
            Module* sub_module_right= new Module();

            Point* left_low_right = new Point(lx_3, ly_3);
            sub_module_right->addBoundaryPoint(left_low_right);
            Point* right_low_right = new Point(hx_3, ly_3);
            sub_module_right->addBoundaryPoint(right_low_right);
            Point* right_up_right = new Point(hx_3, hy_3);
            sub_module_right->addBoundaryPoint(right_up_right);
            Point* left_up_right = new Point(lx_3, hy_3);
            sub_module_right->addBoundaryPoint(left_up_right);
                
            this->addSubModule(sub_module_right);
        }
       
        
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
bool DataService::readAreaEtcFile(string file_path)
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

        switch (pointsNum)
        {
        case 6:
            module->setShape(Shape::LSHAPE);
            module->arrangePoints();
            break;

        case 8:
            module->setShape(Shape::TSHAPE);
            module->arrangePoints();
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
    for (Module* module : _modules)
    {
        if(module->getName() == name)
        {
            return module;
        }
    }
    
    return nullptr;
}

bool DataService::readLinkFile(string file_path)
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
            
            port->setModuleName(module->getName());
            port->setNet(&net);
            net.push_back(port);
        }
        _nets.push_back(net);
        getline(file, temp);
    }

    return true;
}

// void DataService::initialSortModules()
// {
//     vector<Module*> _backups = _modules;
//     _modules.clear();

//     while (!_backups.empty())
//     {
//         for(vector<Port*> net : _nets)
//         {
//             for (Port* port : net)
//             {
//                 string module_name = port->getModuleName();

//                 for (auto it = _backups.begin(); it != _backups.end(); it++)
//                 {
//                     if((*it)->getName() == module_name)
//                     {
//                         _modules.push_back(*it);
//                         it = _backups.erase(it);
//                         if(it == _backups.end())
//                         {
//                             break;
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

void DataService::recursiveConstructionNode(Node* node, uint32_t index)
{
    node->setLChildNode(nullptr);
    node->setRChildNode(nullptr);
    if(index * 2 + 1 < _modules.size())
    {
        Node* left_child = new Node(index * 2 + 1);
        left_child->setModule(_modules[index * 2 + 1]);
        node->setLChildNode(left_child);
        left_child->setParentNode(node);
        recursiveConstructionNode(left_child, index * 2 + 1);
    }
    if(index * 2 + 2 < _modules.size())
    {
        Node* right_child = new Node(index * 2 + 2);
        right_child->setModule(_modules[index * 2 + 2]);
        node->setRChildNode(right_child);
        right_child->setParentNode(node);
        recursiveConstructionNode(right_child, index * 2 + 2);
    }
}

void DataService::initBStarTree()
{
    Node* root = new Node(0);
    root->setModule(_modules[0]);
    recursiveConstructionNode(root, 0);
    _b_star_tree = root;
}

bool DataService::placeModules()
{
    
    return true;
}

float DataService::getLeftXOnAreaLimit(float low_y, float high_y)
{
    Point* last_point_of_range;
    Point* next_point_of_range;
    vector<Point*> points_in_range;
    size_t size = _area.size();

    last_point_of_range = _area[0];
    next_point_of_range = _area[size - 1];

    for (uint32_t i = size - 1 ; i > 0; i--)
    {
        if(_area[i]->getY() <= high_y && _area[i]->getY() >= low_y)
        {
            points_in_range.push_back(_area[i]);
            next_point_of_range = _area[i - 1];
        }else
        {
            if(_area[(i + 1) % size]->getY() <= high_y && _area[(i + 1) % size]->getY() >= low_y)
            {
                break;
            }
            last_point_of_range = _area[i];
        }
    }
    
    if(points_in_range.empty())
    {
        float x1 = last_point_of_range->getX();
        float x2 = next_point_of_range->getX();

        if(x1 == x2)
        {
            return x1;
        }
        else
        {
            float y1 = last_point_of_range->getY();
            float y2 = next_point_of_range->getY();

            float x_diff = x1 - x2;
            float y_diff = y1 - y2;
            float x_y_ratio = x_diff / y_diff;

            float left_x_1 = x1 + (low_y - y1) * x_y_ratio;
            float left_x_2 = x2 + (high_y - y2) * x_y_ratio;

            return left_x_1 >= left_x_2 ? left_x_1 : left_x_2;
        }
    }
    else
    {
        size = points_in_range.size();

        //Calculate the X coordinate of the first candidate
        float left_x_1 = points_in_range[0]->getX();
        for (Point* point : points_in_range)
        {
            if(point->getX() > left_x_1)
            {
                left_x_1 = point->getX();
            }
        }
        
        //Calculate the X coordinate of the second candidate
        float x1 = last_point_of_range->getX();
        float x2 = points_in_range[0]->getX();

        float y1 = last_point_of_range->getY();
        float y2 = points_in_range[0]->getY();

        float x_diff = x1 - x2;
        float y_diff = y1 - y2;
        float x_y_ratio = x_diff / y_diff;

        float left_x_2 = x1 + (low_y - y1) * x_y_ratio;

        //Calculate the X coordinate of the second candidate
        x1 = points_in_range[size - 1]->getX();
        x2 = next_point_of_range->getX();

        y1 = points_in_range[size - 1]->getY();
        y2 = next_point_of_range->getY();

        x_diff = x1 - x2;
        y_diff = y1 - y2;
        x_y_ratio = x_diff / y_diff;

        float left_x_3 = x1 + (high_y - y1) * x_y_ratio;

        return left_x_1 >= (left_x_2 >= left_x_3 ? left_x_2 : left_x_3) ? left_x_1 : (left_x_2 >= left_x_3 ? left_x_2 : left_x_3);
    }
}

float DataService::getRightXOnAreaLimit(float low_y, float high_y)
{
    Point* last_point_of_range;
    Point* next_point_of_range;
    vector<Point*> points_in_range;
    size_t size = _area.size();

    last_point_of_range = _area[0];
    next_point_of_range = _area[1];

    for (uint32_t i = 0 ; i < size; i++)
    {
        if(_area[i]->getY() <= high_y && _area[i]->getY() >= low_y)
        {
            points_in_range.push_back(_area[i]);
            next_point_of_range = _area[i + 1];
        }else
        {
            if(_area[(i - 1) % size]->getY() <= high_y && _area[(i - 1) % size]->getY() >= low_y)
            {
                break;
            }
            last_point_of_range = _area[i];
        }
    }
    
    if(points_in_range.empty())
    {
        float x1 = last_point_of_range->getX();
        float x2 = next_point_of_range->getX();

        if(x1 == x2)
        {
            return x1;
        }
        else
        {
            float y1 = last_point_of_range->getY();
            float y2 = next_point_of_range->getY();

            float x_diff = x1 - x2;
            float y_diff = y1 - y2;
            float x_y_ratio = x_diff / y_diff;

            float left_x_1 = x1 + (low_y - y1) * x_y_ratio;
            float left_x_2 = x2 + (high_y - y2) * x_y_ratio;

            return left_x_1 <= left_x_2 ? left_x_1 : left_x_2;
        }
    }
    else
    {
        size = points_in_range.size();

        //Calculate the X coordinate of the first candidate
        float left_x_1 = points_in_range[0]->getX();
        for (Point* point : points_in_range)
        {
            if(point->getX() < left_x_1)
            {
                left_x_1 = point->getX();
            }
        }
        
        //Calculate the X coordinate of the second candidate
        float x1 = last_point_of_range->getX();
        float x2 = points_in_range[0]->getX();

        float y1 = last_point_of_range->getY();
        float y2 = points_in_range[0]->getY();

        float x_diff = x1 - x2;
        float y_diff = y1 - y2;
        float x_y_ratio = x_diff / y_diff;

        float left_x_2 = x1 + (low_y - y1) * x_y_ratio;

        //Calculate the X coordinate of the second candidate
        x1 = points_in_range[size - 1]->getX();
        x2 = next_point_of_range->getX();

        y1 = points_in_range[size - 1]->getY();
        y2 = next_point_of_range->getY();

        x_diff = x1 - x2;
        y_diff = y1 - y2;
        x_y_ratio = x_diff / y_diff;

        float left_x_3 = x1 + (high_y - y1) * x_y_ratio;

        return left_x_1 <= (left_x_2 <= left_x_3 ? left_x_2 : left_x_3) ? left_x_1 : (left_x_2 <= left_x_3 ? left_x_2 : left_x_3);
    }
}
