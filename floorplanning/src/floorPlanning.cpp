#include "../include/floorPlanning.h"

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

Point* Module::findPoint(float x, float y)
{
    for (Point* point : _boundary)
    {
        if(point->getX() == x && point->getY() == y)
        {
            return point;
        }
    }
    return nullptr;
}

void Module::arrangePoints()
{
    vector<Point*> boundary = _boundary;
    for (uint32_t i = 0; i < boundary.size() - 1; i++)
    {
        for (uint32_t j = 0; j < boundary.size() - 1 - i; j++)
        {
            if(boundary[j + 1]->getY() > boundary[j]->getY())
            {
                Point* temp      = boundary[j];
                boundary[j]     = boundary[j + 1];
                boundary[j + 1] = temp;
            }
        }
    }
    for (uint32_t i = 0; i < boundary.size() - 1; i++)
    {
        for (uint32_t j = 0; j < boundary.size() - 1 - i; j++)
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
    this->getSubModule().clear();

    for (Module* sub_module : this->getSubModule())
    {
        sub_module->~Module();
    }

    this->getSubModule().clear();
    
    switch (this->getShape())
    {
    case Shape::LSHAPE :
    {
        float left_height  = boundarys[0]->getY() - boundarys[1]->getY();
        float right_height = boundarys[4]->getY() - boundarys[5]->getY();
        float lx_1, ly_1, hx_1, hy_1;//Coordinate extremum of left rectangle.
        float lx_2, ly_2, hx_2, hy_2;//Coordinate extremum of right rectangle.
       
        
        lx_1 = boundarys[1]->getX();
        ly_1 = boundarys[1]->getY();
        hx_1 = boundarys[2]->getX();
        hy_1 = ly_1 + left_height;

        
        lx_2 = boundarys[2]->getX();
        ly_2 = boundarys[5]->getY();
        hx_2 = boundarys[4]->getX();
        hy_2 = ly_2 + right_height;

        //Arrange points counterclockwise

        this->getBoundary()[0]->setPoint(lx_1, ly_1);
        if(ly_1 != ly_2)
        {
            this->getBoundary()[1]->setPoint(hx_1, ly_1);
            this->getBoundary()[2]->setPoint(lx_2, ly_2);
            this->getBoundary()[3]->setPoint(hx_2, ly_2);
            this->getBoundary()[4]->setPoint(hx_2, hy_2);
        }else
        {
            this->getBoundary()[1]->setPoint(hx_2, ly_2);
            this->getBoundary()[2]->setPoint(hx_2, hy_2);
            this->getBoundary()[3]->setPoint(lx_2, hy_2);
            this->getBoundary()[4]->setPoint(hx_1, hy_1);
        }
        this->getBoundary()[5]->setPoint(lx_1, hy_1);

        //set left rectangle points

        Module* sub_module_left= new Module();

        Point* left_low_left = this->findPoint(lx_1, ly_1);
        if(left_low_left == nullptr)
        {
            left_low_left = new Point(lx_1, ly_1);
        }
        sub_module_left->addBoundaryPoint(left_low_left);

        Point* right_low_left = this->findPoint(hx_1, ly_1);
        if(right_low_left == nullptr)
        {
            right_low_left = new Point(hx_1, ly_1);
        }
        sub_module_left->addBoundaryPoint(right_low_left);

        Point* right_up_left = this->findPoint(hx_1, hy_1);
        if(right_up_left == nullptr)
        {
            right_up_left = new Point(hx_1, hy_1);
        }
        sub_module_left->addBoundaryPoint(right_up_left);

        Point* left_up_left = this->findPoint(lx_1, hy_1);
        if(left_up_left == nullptr)
        {
            left_up_left = new Point(lx_1, hy_1);
        }
        sub_module_left->addBoundaryPoint(left_up_left);

        sub_module_left->setWidth(hx_1 - lx_1);
        sub_module_left->setHeight(hy_1 - ly_1);
            
        this->addSubModule(sub_module_left);

        //set right rectangle points

        Module* sub_module_right= new Module();

        Point* left_low_right = this->findPoint(lx_2, ly_2);
        if(left_low_right == nullptr)
        {
            left_low_right = new Point(lx_2, ly_2);
        }
        sub_module_right->addBoundaryPoint(left_low_right);

        Point* right_low_right = this->findPoint(hx_2, ly_2);
        if(right_low_right == nullptr)
        {
            right_low_right = new Point(hx_2, ly_2);
        }
        sub_module_right->addBoundaryPoint(right_low_right);

        Point* right_up_right = this->findPoint(hx_2, hy_2);
        if(right_up_right == nullptr)
        {
            right_up_right = new Point(hx_2, hy_2);
        }
        sub_module_right->addBoundaryPoint(right_up_right);

        Point* left_up_right = this->findPoint(lx_2, hy_2);
        if(left_up_right == nullptr)
        {
            left_up_right = new Point(lx_2, hy_2);
        }
        sub_module_right->addBoundaryPoint(left_up_right);
        
        sub_module_right->setWidth(hx_2 - lx_2);
        sub_module_right->setHeight(hy_2 - ly_2);

        float y_diff =
        sub_module_right->getBoundary()[0]->getY() - sub_module_left->getBoundary()[1]->getY();
        sub_module_right->setYDiff(y_diff);
            
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

            Point* left_low_left = this->findPoint(lx_1, ly_1);
            if(left_low_left == nullptr)
            {
                left_low_left = new Point(lx_1, ly_1);
            }
            sub_module_left->addBoundaryPoint(left_low_left);

            Point* right_low_left = this->findPoint(hx_1, ly_1);
            if(right_low_left == nullptr)
            {
                right_low_left = new Point(hx_1, ly_1);
            }
            sub_module_left->addBoundaryPoint(right_low_left);

            Point* right_up_left = this->findPoint(hx_1, hy_1);
            if(right_up_left == nullptr)
            {
                right_up_left = new Point(hx_1, hy_1);
            }
            sub_module_left->addBoundaryPoint(right_up_left);

            Point* left_up_left = this->findPoint(lx_1, hy_1);
            if(left_up_left == nullptr)
            {
                left_up_left = new Point(lx_1, hy_1);
            }
            sub_module_left->addBoundaryPoint(left_up_left);

            sub_module_left->setWidth(hx_1 - lx_1);
            sub_module_left->setHeight(hy_1 - ly_1);
                
            this->addSubModule(sub_module_left);

            lx_2 = boundarys[2]->getX();
            ly_2 = boundarys[7]->getY();
            hx_2 = boundarys[6]->getX();
            hy_2 = ly_2 + right_height;

            //set right rectangle points
            Module* sub_module_right= new Module();

            Point* left_low_right = this->findPoint(lx_2, ly_2);
            if(left_low_right == nullptr)
            {
                left_low_right = new Point(lx_2, ly_2);
            }
            sub_module_right->addBoundaryPoint(left_low_right);

            Point* right_low_right = this->findPoint(hx_2, ly_2);
            if(right_low_right == nullptr)
            {
                right_low_right = new Point(hx_2, ly_2);
            }
            sub_module_right->addBoundaryPoint(right_low_right);

            Point* right_up_right = this->findPoint(hx_2, hy_2);
            if(right_up_right == nullptr)
            {
                right_up_right = new Point(hx_2, hy_2);
            }
            sub_module_right->addBoundaryPoint(right_up_right);

            Point* left_up_right = this->findPoint(lx_2, hy_2);
            if(left_up_right == nullptr)
            {
                left_up_right = new Point(lx_2, hy_2);
            }
            sub_module_right->addBoundaryPoint(left_up_right);

            sub_module_right->setWidth(hx_2 - lx_2);
            sub_module_right->setHeight(hy_2 - ly_2);
            
            float y_diff =
            sub_module_right->getBoundary()[0]->getY() - sub_module_left->getBoundary()[1]->getY();
            sub_module_right->setYDiff(y_diff);

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

            Point* left_low_left = this->findPoint(lx_1, ly_1);
            if(left_low_left == nullptr)
            {
                left_low_left = new Point(lx_1, ly_1);
            }
            sub_module_left->addBoundaryPoint(left_low_left);

            Point* right_low_left = this->findPoint(hx_1, ly_1);
            if(right_low_left == nullptr)
            {
                right_low_left = new Point(hx_1, ly_1);
            }
            sub_module_left->addBoundaryPoint(right_low_left);

            Point* right_up_left = this->findPoint(hx_1, hy_1);
            if(right_up_left == nullptr)
            {
                right_up_left = new Point(hx_1, hy_1);
            }
            sub_module_left->addBoundaryPoint(right_up_left);

            Point* left_up_left = this->findPoint(lx_1, hy_1);
            if(left_up_left == nullptr)
            {
                left_up_left = new Point(lx_1, hy_1);
            }
            sub_module_left->addBoundaryPoint(left_up_left);

            sub_module_left->setWidth(hx_1 - lx_1);
            sub_module_left->setHeight(hy_1 - ly_1);
                
            this->addSubModule(sub_module_left);

            //set mid rectangle points
            Module* sub_module_mid= new Module();

            Point* left_low_mid = this->findPoint(lx_2, ly_2);
            if(left_low_mid == nullptr)
            {
                left_low_mid = new Point(lx_2, ly_2);
            }
            sub_module_mid->addBoundaryPoint(left_low_mid);

            Point* right_low_mid = this->findPoint(hx_2, ly_2);
            if(right_low_mid == nullptr)
            {
                right_low_mid = new Point(hx_2, ly_2);
            }
            sub_module_mid->addBoundaryPoint(right_low_mid);

            Point* right_up_mid = this->findPoint(hx_2, hy_2);
            if(right_up_mid == nullptr)
            {
                right_up_mid = new Point(hx_2, hy_2);
            }
            sub_module_mid->addBoundaryPoint(right_up_mid);

            Point* left_up_mid = this->findPoint(lx_2, hy_2);
            if(left_up_mid == nullptr)
            {
                left_up_mid = new Point(lx_2, hy_2);
            }
            sub_module_mid->addBoundaryPoint(left_up_mid);

            sub_module_mid->setWidth(hx_2 - lx_2);
            sub_module_mid->setHeight(hy_2 - ly_2);

            float _y_diff_1 =
            sub_module_mid->getBoundary()[0]->getY() - sub_module_left->getBoundary()[1]->getY();
            sub_module_mid->setYDiff(_y_diff_1);
                
            this->addSubModule(sub_module_mid);

            //set right rectangle points
            Module* sub_module_right= new Module();

            Point* left_low_right = this->findPoint(lx_3, ly_3);
            if(left_low_right == nullptr)
            {
                left_low_right = new Point(lx_3, ly_3);
            }
            sub_module_right->addBoundaryPoint(left_low_right);

            Point* right_low_right = this->findPoint(hx_3, ly_3);
            if(right_low_right == nullptr)
            {
                right_low_right = new Point(hx_3, ly_3);
            }
            sub_module_right->addBoundaryPoint(right_low_right);

            Point* right_up_right = this->findPoint(hx_3, hy_3);
            if(right_up_right == nullptr)
            {
                right_up_right = new Point(hx_3, hy_3);
            }
            sub_module_right->addBoundaryPoint(right_up_right);

            Point* left_up_right = this->findPoint(lx_3, hy_3);
            if(left_up_right == nullptr)
            {
                left_up_right = new Point(lx_3, hy_3);
            }
            sub_module_right->addBoundaryPoint(left_up_right);

            sub_module_right->setWidth(hx_3 - lx_3);
            sub_module_right->setHeight(hy_3 - ly_3);

            float _y_diff_2 = 
            sub_module_right->getBoundary()[0]->getY() - sub_module_mid->getBoundary()[1]->getY();
            sub_module_right->setYDiff(_y_diff_2);
                
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
        uint32_t pos1 = str.find("(");
        str.erase(0, pos1 + 1);
        pos1 = 0;

        uint32_t pos2 = str.find(",");
        uint32_t pos3 = str.find(")");

        uint32_t len = str.copy(buffer, pos2 - pos1, pos1);
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
        uint32_t pos1 = str.find(":");
        str.erase(0, pos1 + 1);
        module->setName(str);

        //get boundary
        getline(file, str);
        uint32_t pointsNum = 0;
        while (str.find("(") != str.npos)
        {
            uint32_t pos1 = str.find("(");
            str.erase(0, pos1 + 1);
            pos1 = 0;

            uint32_t pos2 = str.find(",");
            uint32_t pos3 = str.find(")");

            uint32_t len = str.copy(buffer, pos2 - pos1, pos1);
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
            float lx, hx, ly, hy;
            lx = hx = module->getBoundary()[0]->getX();
            ly = hy = module->getBoundary()[0]->getY();
            for (Point* point : module->getBoundary())
            {
                if(point->getX() < lx)
                {
                    lx = point->getX();
                }
                if(point->getX() > hx)
                {
                    hx = point->getX();
                }
                if(point->getY() < ly)
                {
                    ly = point->getY();
                }
                if(point->getY() > hy)
                {
                    hy = point->getY();
                }
            }
            
            module->setWidth(hx - lx);
            module->setHeight(hy - ly);

            module->getBoundary()[0]->setPoint(lx, ly);
            module->getBoundary()[1]->setPoint(hx, ly);
            module->getBoundary()[2]->setPoint(hx, hy);
            module->getBoundary()[3]->setPoint(lx, hy);
            break;
        }

        //get Port
        getline(file, str);
        while (str.find("P") != str.npos)
        {
            Port* port = new Port();
            vector<Point*> boundary;

            while (str.find("(") != str.npos)
            {
                uint32_t pos1 = str.find("(");
                str.erase(0, pos1 + 1);
                pos1 = 0;

                uint32_t pos2 = str.find(",");
                uint32_t pos3 = str.find(")");

                uint32_t len = str.copy(buffer, pos2 - pos1, pos1);
                buffer[len] = '\0';
                x = atof(buffer);
                len = str.copy(buffer, pos3 - pos2 - 2, pos2 + 2);
                buffer[len] = '\0';
                y = atof(buffer);

                Point* point = new Point(x, y);
                
                boundary.push_back(point);

                str.erase(0, pos3 + 1);
            }

            Point* center_point = calculationCenterPoint(boundary);
            center_point->setPoint(center_point->getX(), center_point->getY());
            port->setCenterPoint(center_point);

            module->addPort(port);
            getline(file, str);
        }

        this->addModule(module);
    }

    return true;
}

Point* DataService::calculationCenterPoint(vector<Point*> boundary)
{
    float x_add = 0;
    float y_add = 0;

    uint8_t points_num = boundary.size();

    for (Point* point : boundary)
    {
        x_add += point->getX();
        y_add += point->getY();
    }
    
    float x_average = x_add / points_num;
    float y_average = y_add / points_num;

    Point* center_point = new Point();
    center_point->setPoint(x_average, y_average);

    return center_point;
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

void DataService::findNodeById(Node* root)
{
    if(root == nullptr)
    {
        return;
    }

    if(root->getId() == _node_id)
    {
        _node_finding = root;
        return;
    }

    findNodeById(root->getLChildNode());

    findNodeById(root->getRChildNode());
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
            uint32_t pos1 = moduleName.find(" ");
            string name = moduleName.substr(0, pos1);
            moduleName.erase(0, pos1 + 1);
            uint32_t pos2 = portId.find(" ");
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
    root->setParentNode(nullptr);
    root->setModule(_modules[0]);
    recursiveConstructionNode(root, 0);
    _b_star_tree = root;
}

bool DataService::initPlaceModules()
{
    _outline_points.clear();

    _place_modules_success = true;

    this->setOriginPoint(_area[0]);

    placeModule(_b_star_tree);

    return _place_modules_success;
}

void DataService::placeModule(Node* root)
{
    if(_place_modules_success == false || root == nullptr)
    {
        return;
    }

    Module* module = root->getModule();

    if(module->getShape() == Shape::RECT)
    {
        placeRectShape(module);
    }
    else if(module->getShape() == Shape::LSHAPE)
    {
        placeLShape(module);
    }
    else
    {
        placeTShape(module);
    }
    writeGDS();
    updateOutLine(module);

    placeModule(root->getLChildNode());
    
    setOriginPoint(module->getBoundary()[module->getBoundary().size() - 1]);

    placeModule(root->getRChildNode());
}

void DataService::placeRectShape(Module* module)
{
    //save origin x and y
    float origin_x = module->getBoundary()[0]->getX();
    float origin_y = module->getBoundary()[0]->getY();

    //determine x coordinate
    float low_y  = _origin_point_for_placeing->getY();
    float high_y = low_y + module->getHeight();

    float left_x_on_area_limit    = getLeftXOnAreaLimit(low_y, high_y);
    float left_x_on_outline_limit = -FLT_MAX;//getLeftXOnOutlineLimit(low_y, high_y);

    float left_x = _origin_point_for_placeing->getX() >
    (left_x_on_area_limit > left_x_on_outline_limit ? left_x_on_area_limit : left_x_on_outline_limit) ?
    _origin_point_for_placeing->getX() :
    (left_x_on_area_limit > left_x_on_outline_limit ? left_x_on_area_limit : left_x_on_outline_limit);

    //determine y coordinate
    float low_x  = _origin_point_for_placeing->getX();
    float high_x = low_x + module->getWidth();

    float bottom_y_on_area_limit    = getBottomYOnAreaLimit(low_x, high_x);
    float bottom_y_on_outline_limit = getBottomYOnOutlineLimit(low_x, high_x);

    float bottom_y = bottom_y_on_area_limit > bottom_y_on_outline_limit ? bottom_y_on_area_limit : bottom_y_on_outline_limit;

    //Judge whether out of bounds
    if( left_x + module->getWidth()    > getRightXOnAreaLimit(low_y, high_y)
     || bottom_y + module->getHeight() > getTopYOnAreaLimit(low_x, high_x) 
      )
    {
        _place_modules_success = false;
        return;
    }

    //change points
    module->getBoundary()[0]->setPoint(left_x, bottom_y);
    module->getBoundary()[1]->setPoint(left_x + module->getWidth(), bottom_y);
    module->getBoundary()[2]->setPoint(left_x + module->getWidth(), bottom_y + module->getHeight());
    module->getBoundary()[3]->setPoint(left_x, bottom_y + module->getHeight());

    //change ports' center point 
    for (Port* port : module->getPorts())
    {
        port->getCenter_Point()->setX(port->getCenter_Point()->getX() + left_x - origin_x);
        port->getCenter_Point()->setY(port->getCenter_Point()->getY() + bottom_y - origin_y);
    }  

    setOriginPoint(module->getBoundary()[1]);
}

void DataService::placeLShape(Module* module)
{
    //save origin x and y
    float origin_x = module->getBoundary()[0]->getX();
    float origin_y = module->getBoundary()[0]->getY();

    Module* major_rect  = module->getSubModule()[0];
    Module* sub_rect    = module->getSubModule()[1];

    placeRectShape(major_rect);

    //The major module needs to increase the value of Y coordinate because the sub module cannot be placed
    float sub_rect_bottom_y   = placeSubRect(sub_rect);
    float major_add_diff      = major_rect->getBoundary()[0]->getY() + sub_rect->getYDiff();

    if(sub_rect_bottom_y > major_add_diff)
    {
        float y_increment = sub_rect_bottom_y - major_add_diff;

        for (Point* point : major_rect->getBoundary())
        {
            point->setY(point->getY() + y_increment);
        }

        //Judge whether out of bounds
        float low_x  = major_rect->getBoundary()[0]->getX();
        float high_x = major_rect->getBoundary()[1]->getX();

        float top_y_limit = getTopYOnAreaLimit(low_x, high_x);
        if(major_rect->getBoundary()[2]->getY() > top_y_limit)
        {
            _place_modules_success = false;
            return;
        }
    }

    //set sub_rect's coordinate
    float lx = major_rect->getBoundary()[1]->getX();
    float ly = major_rect->getBoundary()[0]->getY() + sub_rect->getYDiff();
    float hx = lx + sub_rect->getWidth();
    float hy = ly + sub_rect->getHeight();

    sub_rect->getBoundary()[0]->setPoint(lx, ly);
    sub_rect->getBoundary()[1]->setPoint(hx, ly);
    sub_rect->getBoundary()[2]->setPoint(hx, hy);
    sub_rect->getBoundary()[3]->setPoint(lx, hy);

    //Judge whether out of bounds
    float low_x  = sub_rect->getBoundary()[0]->getX();
    float high_x = sub_rect->getBoundary()[1]->getX();

    float top_y_limit = getTopYOnAreaLimit(low_x, high_x);
    if(sub_rect->getBoundary()[2]->getY() > top_y_limit)
    {
        _place_modules_success = false;
        return;
    }

    //change ports' center point 
    for (Port* port : module->getPorts())
    {
        port->getCenter_Point()->setX(port->getCenter_Point()->getX() + module->getBoundary()[0]->getX() - origin_x);
        port->getCenter_Point()->setY(port->getCenter_Point()->getY() + module->getBoundary()[0]->getY() - origin_y);
    }  

    float origin_point_y = sub_rect_bottom_y < major_add_diff ? sub_rect_bottom_y : major_add_diff;
    Point* new_origin_point = new Point(hx, origin_point_y);
    setOriginPoint(new_origin_point);
}

void DataService::placeTShape(Module* module)
{
    if(module->getSubModule().size() == 2)
    {
        placeLShape(module);
    }
    else
    {
        //save origin x and y
        float origin_x = module->getBoundary()[0]->getX();
        float origin_y = module->getBoundary()[0]->getY();

        Module* major_rect     = module->getSubModule()[0];
        Module* sub_rect_left  = module->getSubModule()[1];
        Module* sub_rect_right = module->getSubModule()[2];

        placeRectShape(major_rect);

        //The major module needs to increase the value of Y coordinate because the left sub module cannot be placed.
        float left_sub_rect_bottom_y   = placeSubRect(sub_rect_left);
        float major_add_diff      = major_rect->getBoundary()[0]->getY() + sub_rect_left->getYDiff();

        if(left_sub_rect_bottom_y > major_add_diff)
        {
            float y_increment = left_sub_rect_bottom_y - major_add_diff;

            for (Point* point : major_rect->getBoundary())
            {
                point->setY(point->getY() + y_increment);
            }

            //Judge whether out of bounds
            float low_x  = major_rect->getBoundary()[0]->getX();
            float high_x = major_rect->getBoundary()[1]->getX();

            float top_y_limit = getTopYOnAreaLimit(low_x, high_x);
            if(major_rect->getBoundary()[2]->getY() > top_y_limit)
            {
                _place_modules_success = false;
                return;
            }
        }


        //set sub_rect's coordinate
        float lx = major_rect->getBoundary()[1]->getX();
        float ly = major_rect->getBoundary()[0]->getY() + sub_rect_left->getYDiff();
        float hx = lx + sub_rect_left->getWidth();
        float hy = ly + sub_rect_left->getHeight();

        sub_rect_left->getBoundary()[0]->setPoint(lx, ly);
        sub_rect_left->getBoundary()[1]->setPoint(hx, ly);
        sub_rect_left->getBoundary()[2]->setPoint(hx, hy);
        sub_rect_left->getBoundary()[3]->setPoint(lx, hy);

        setOriginPoint(sub_rect_left->getBoundary()[1]);

        //The major module and left sub module needs to increase the value of Y coordinate,
        //because the right sub module cannot be placed.
        float right_sub_rect_bottom_y   = placeSubRect(sub_rect_right);
        float left_add_diff             = sub_rect_left->getBoundary()[0]->getY() + sub_rect_right->getYDiff();

        if(right_sub_rect_bottom_y > left_add_diff)
        {
            float y_increment = right_sub_rect_bottom_y - left_add_diff;

            for (Point* point : major_rect->getBoundary())
            {
                point->setY(point->getY() + y_increment);
            }

            for (Point* point : sub_rect_left->getBoundary())
            {
                point->setY(point->getY() + y_increment);
            }
            
            //Judge whether out of bounds
            float low_x  = major_rect->getBoundary()[0]->getX();
            float high_x = major_rect->getBoundary()[1]->getX();

            float left_top_y_limit = getTopYOnAreaLimit(low_x, high_x);
            if(major_rect->getBoundary()[2]->getY() > left_top_y_limit)
            {
                _place_modules_success = false;
                return;
            }

            low_x  = sub_rect_left->getBoundary()[0]->getX();
            high_x = sub_rect_left->getBoundary()[1]->getX();

            float right_top_y_limit = getTopYOnAreaLimit(low_x, high_x);
            if(sub_rect_left->getBoundary()[2]->getY() > right_top_y_limit)
            {
                _place_modules_success = false;
                return;
            }
        }

        //set right_sub_rect's coordinate
        lx = sub_rect_left->getBoundary()[1]->getX();
        ly = sub_rect_left->getBoundary()[0]->getY() + sub_rect_right->getYDiff();
        hx = lx + sub_rect_right->getWidth();
        hy = ly + sub_rect_right->getHeight();

        sub_rect_right->getBoundary()[0]->setPoint(lx, ly);
        sub_rect_right->getBoundary()[1]->setPoint(hx, ly);
        sub_rect_right->getBoundary()[2]->setPoint(hx, hy);
        sub_rect_right->getBoundary()[3]->setPoint(lx, hy);

        //Judge whether out of bounds
        float low_x  = sub_rect_right->getBoundary()[0]->getX();
        float high_x = sub_rect_right->getBoundary()[1]->getX();

        float top_y_limit = getTopYOnAreaLimit(low_x, high_x);
        if(sub_rect_right->getBoundary()[2]->getY() > top_y_limit)
        {
            _place_modules_success = false;
            return;
        }

        //change ports' center point 
        for (Port* port : module->getPorts())
        {
            port->getCenter_Point()->setX(port->getCenter_Point()->getX() + module->getBoundary()[0]->getX() - origin_x);
            port->getCenter_Point()->setY(port->getCenter_Point()->getY() + module->getBoundary()[0]->getY() - origin_y);
        }  

        float origin_point_y = right_sub_rect_bottom_y < major_add_diff ? right_sub_rect_bottom_y : major_add_diff;
        Point* new_origin_point = new Point(hx, origin_point_y);
        setOriginPoint(new_origin_point);
    }
}

float DataService::placeSubRect(Module* sub_rect)
{
    //determine y coordinate
    float low_x  = _origin_point_for_placeing->getX();
    float high_x = low_x + sub_rect->getWidth();

    float bottom_y_on_area_limit    = getBottomYOnAreaLimit(low_x, high_x);
    float bottom_y_on_outline_limit = -FLT_MAX;//getBottomYOnOutlineLimit(low_x, high_x);

    float bottom_y = _origin_point_for_placeing->getY() >
    (bottom_y_on_area_limit > bottom_y_on_outline_limit ? bottom_y_on_area_limit : bottom_y_on_outline_limit) ?
    _origin_point_for_placeing->getY() :
    (bottom_y_on_area_limit > bottom_y_on_outline_limit ? bottom_y_on_area_limit : bottom_y_on_outline_limit);

    //Judge whether out of bounds
    if(bottom_y + sub_rect->getHeight() > getTopYOnAreaLimit(low_x, high_x))
    {
        _place_modules_success = false;
        return -FLT_MAX;
    }

    return bottom_y;
}

void DataService::updateOutLine(Module* module)
{
    if(_place_modules_success == false)
    {
        return;
    }

    if(_outline_points.empty())
    {
        _outline_points = module->getBoundary();
        return;
    }

    vector<Point*> new_outline;
    uint32_t outline_size  = _outline_points.size();
    uint32_t boundary_size = module->getBoundary().size();

    uint32_t boundary_point_index_1, boundary_point_index_2;
    Point* first_point_to_delete = nullptr;
    Point* last_point_to_delete = nullptr;

    for (size_t i = 0; i < outline_size - 2; i++)
    {
        if(_outline_points[i]->getX() == module->getBoundary()[0]->getX()
        && _outline_points[i + 1]->getX() == module->getBoundary()[boundary_size - 1]->getX())
        {
            float y1 = _outline_points[i + 1]->getY();
            float y2 = _outline_points[i]->getY();
            float y3 = module->getBoundary()[boundary_size - 1]->getY();
            float y4 = module->getBoundary()[0]->getY();

            if(((y1 >= y3 && y4 < y1)
            ||  (y1 >= y4 && y2 < y4)
            ||  (y1 <= y3 && y2 >= y4)
            ||  (y1 >= y3 && y2 <= y4))
            &&  y1 > y2)
            {
                new_outline.clear();

                boundary_point_index_1 = i;
                boundary_point_index_2 = i + 1;

                first_point_to_delete = _outline_points[i + 1];
   
                for (size_t j = 0; j <= boundary_point_index_1; j++)
                {
                    new_outline.push_back(_outline_points[j]);
                }
                
                if(_outline_points[boundary_point_index_1]->getY() != module->getBoundary()[0]->getY())
                {
                    new_outline.push_back(module->getBoundary()[0]);
                }
                
                for (size_t j = 1; j < boundary_size - 1; j++)
                {
                    new_outline.push_back(module->getBoundary()[j]);
                }
                
                if(_outline_points[boundary_point_index_2]->getY() !=
                module->getBoundary()[boundary_size - 1]->getY())
                {
                    new_outline.push_back(module->getBoundary()[boundary_size - 1]);
                }

                for (size_t j = boundary_point_index_2; j < outline_size; j++)
                {
                    new_outline.push_back(_outline_points[j]);
                }

                break;
            } 
        }

        if(_outline_points[i]->getY() == module->getBoundary()[1]->getY()
        && _outline_points[i + 1]->getY() == module->getBoundary()[0]->getY())
        {
            float x1 = _outline_points[i + 1]->getX();
            float x2 = _outline_points[i]->getX();
            float x3 = module->getBoundary()[0]->getX();
            float x4 = module->getBoundary()[1]->getX();

            if(((x1 <= x3 && x4 > x1)
            ||  (x1 <= x4 && x2 > x4)
            ||  (x1 >= x3 && x2 <= x4)
            ||  (x1 <= x3 && x2 >= x4))
            &&  x2 > x1)
            {
                new_outline.clear();

                boundary_point_index_1 = i;
                boundary_point_index_2 = i + 1;

                first_point_to_delete = _outline_points[i + 1];

                for (size_t j = 0; j <= boundary_point_index_1; j++)
                {
                    new_outline.push_back(_outline_points[j]);
                }
                
                if(_outline_points[boundary_point_index_1]->getX() !=
                   module->getBoundary()[1]->getX())
                {
                    new_outline.push_back(module->getBoundary()[1]);
                }
                
                for (size_t j = 2; j < boundary_size; j++)
                {
                    new_outline.push_back(module->getBoundary()[j]);
                }
                
                if(_outline_points[boundary_point_index_2]->getX() !=
                   module->getBoundary()[0]->getX())
                {
                    new_outline.push_back(module->getBoundary()[0]);
                }

                for (size_t j = boundary_point_index_2; j < outline_size; j++)
                {
                    new_outline.push_back(_outline_points[j]);
                }

                break;
            }
        }
    }

    for (size_t i = 0; i < outline_size - 2; i++)
    {
        if(_outline_points[i]->getX() == module->getBoundary()[0]->getX()
        && _outline_points[i + 1]->getX() == module->getBoundary()[boundary_size - 1]->getX())
        {
            float y1 = _outline_points[i + 1]->getY();
            float y2 = _outline_points[i]->getY();
            float y3 = module->getBoundary()[boundary_size - 1]->getY();
            float y4 = module->getBoundary()[0]->getY();

            if(((y1 >= y3 && y4 < y1)
            ||  (y1 >= y4 && y2 < y4)
            ||  (y1 <= y3 && y2 >= y4)
            ||  (y1 >= y3 && y2 <= y4))
            &&  y1 > y2)
            {
                last_point_to_delete = _outline_points[i + 1];
            }
        }

        if(_outline_points[i]->getY() == module->getBoundary()[1]->getY()
        && _outline_points[i + 1]->getY() == module->getBoundary()[0]->getY())
        {
            float x1 = _outline_points[i + 1]->getX();
            float x2 = _outline_points[i]->getX();
            float x3 = module->getBoundary()[0]->getX();
            float x4 = module->getBoundary()[1]->getX();

            if(((x1 <= x3 && x4 > x1)
            ||  (x1 <= x4 && x2 > x4)
            ||  (x1 >= x3 && x2 <= x4)
            ||  (x1 <= x3 && x2 >= x4))
            &&  x2 > x1)
            {
                last_point_to_delete = _outline_points[i + 1];
            }
        }
    }

    if(last_point_to_delete != nullptr)
    {
        auto it = new_outline.begin();
        while (*it != first_point_to_delete)
        {
            it++;
        }
        
        while (*it != last_point_to_delete)
        {
            new_outline.erase(it);
        }
    }
    
    _outline_points.clear();
    _outline_points.push_back(new_outline[0]);
    for (size_t i = 1; i < new_outline.size() - 1; i++)
    {
        if(new_outline[i - 1]->getX() == new_outline[i]->getX() &&
           new_outline[i]->getX() == new_outline[i + 1]->getX())
        {
            continue;
        }
        if(new_outline[i - 1]->getY() == new_outline[i]->getY() &&
           new_outline[i]->getY() == new_outline[i + 1]->getY())
        {
            continue;
        }
        _outline_points.push_back(new_outline[i]);
    } 
    _outline_points.push_back(new_outline[new_outline.size() - 1]);
}

float DataService::getLeftXOnAreaLimit(float low_y, float high_y)
{
    Point* last_point_of_range;
    Point* next_point_of_range;
    vector<Point*> points_in_range;
    uint32_t size = _area.size();

    last_point_of_range = _area[0];
    next_point_of_range = _area[size - 1];

    for (uint32_t i = size; i > 0; i--)
    {
        if(_area[i % size]->getY() <= low_y && _area[i - 1]->getY() >= high_y)
        {
            float x1 = _area[i % size]->getX();
            float x2 = _area[i - 1]->getX();
            float y1 = _area[i % size]->getY();
            float y2 = _area[i - 1]->getY();

            float x_diff = x1 - x2;
            float y_diff = y1 - y2;
            float x_y_ratio = x_diff / y_diff;

            float left_x_1 = x1 + (low_y - y1) * x_y_ratio;
            float left_x_2 = x2 + (high_y - y2) * x_y_ratio;

            return left_x_1 >= left_x_2 ? left_x_1 : left_x_2;
        }
        else
        {
            if(_area[i % size]->getY() < high_y && _area[i % size]->getY() > low_y)
            {
                points_in_range.push_back(_area[i % size]);
                next_point_of_range = _area[i - 1];
            }
            else
            {
                if(    _area[(i + 1) % size]->getY() <= high_y
                    && _area[(i + 1) % size]->getY() >= low_y
                    && _area[(i + 1) % size]->getY() != _area[i % size]->getY()
                  )
                {
                    break;
                }
                last_point_of_range = _area[i % size];
            }
        }
    }
    
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

float DataService::getRightXOnAreaLimit(float low_y, float high_y)
{
    Point* last_point_of_range;
    Point* next_point_of_range;
    vector<Point*> points_in_range;
    uint32_t size = _area.size();

    last_point_of_range = _area[0];
    next_point_of_range = _area[1];

    for (uint32_t i = 1 ; i < size - 1; i++)
    {
        if(_area[i]->getY() <= low_y && _area[(i + 1) % size]->getY() >= high_y)
        {
            float x1 = _area[(i + 1) % size]->getX();
            float x2 = _area[i]->getX();
            float y1 = _area[(i + 1) % size]->getY();
            float y2 = _area[i]->getY();

            float x_diff = x1 - x2;
            float y_diff = y1 - y2;
            float x_y_ratio = x_diff / y_diff;

            float left_x_1 = x1 + (low_y - y1) * x_y_ratio;
            float left_x_2 = x2 + (high_y - y2) * x_y_ratio;

            return left_x_1 >= left_x_2 ? left_x_1 : left_x_2;
        }
        else
        {
            if(_area[i]->getY() < high_y && _area[i]->getY() > low_y)
            {
                points_in_range.push_back(_area[i]);
                next_point_of_range = _area[i + 1];
            }
            else
            {
                if( (_area[(i - 1) % size]->getY() <= high_y)
                    && (_area[(i - 1) % size]->getY() >= low_y)
                    && (_area[(i - 1) % size]->getY() != _area[i]->getY())
                    )
                {
                    break;
                }
                last_point_of_range = _area[i];
            }
        } 
    }
    
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

float DataService::getTopYOnAreaLimit(float low_x, float high_x)
{
    Point* last_point_of_range;
    Point* next_point_of_range;
    vector<Point*> points_in_range;
    uint32_t size = _area.size();

    last_point_of_range = _area[0];
    next_point_of_range = _area[size - 1];

    for (uint32_t i = size - 1 ; i > 0; i--)
    {
        if(_area[i]->getX() <= low_x && _area[i - 1]->getX() >= high_x)
        {
            float x1 = _area[i]->getX();
            float x2 = _area[i - 1]->getX();
            float y1 = _area[i]->getY();
            float y2 = _area[i - 1]->getY();

            float x_diff = x1 - x2;
            float y_diff = y1 - y2;
            float y_x_ratio = y_diff / x_diff;

            float top_y_1 = y1 + (low_x - x1) * y_x_ratio;
            float top_y_2 = y2 + (high_x - x2) * y_x_ratio;

            return top_y_1 <= top_y_2 ? top_y_1 : top_y_2;
        }
        else
        {
            if(_area[i]->getX() < high_x && _area[i]->getX() > low_x)
            {
                points_in_range.push_back(_area[i]);
                next_point_of_range = _area[i - 1];
            }
            else
            {
                if(   _area[(i + 1) % size]->getX() <= high_x
                   && _area[(i + 1) % size]->getX() >= low_x
                   && _area[(i + 1) % size]->getX() != _area[i]->getX()
                   )
                {
                    break;
                }
                last_point_of_range = _area[i];
            }
        } 
    }
    
    size = points_in_range.size();

    //Calculate the X coordinate of the first candidate
    float top_y_1 = points_in_range[0]->getY();
    for (Point* point : points_in_range)
    {
        if(point->getX() < top_y_1)
        {
            top_y_1 = point->getY();
        }
    }
    
    //Calculate the X coordinate of the second candidate
    float x1 = last_point_of_range->getX();
    float x2 = points_in_range[0]->getX();

    float y1 = last_point_of_range->getY();
    float y2 = points_in_range[0]->getY();

    float x_diff = x1 - x2;
    float y_diff = y1 - y2;
    float y_x_ratio = y_diff / x_diff;

    float top_y_2 = y1 + (low_x - x1) * y_x_ratio;

    //Calculate the X coordinate of the second candidate
    x1 = points_in_range[size - 1]->getX();
    x2 = next_point_of_range->getX();

    y1 = points_in_range[size - 1]->getY();
    y2 = next_point_of_range->getY();

    x_diff = x1 - x2;
    y_diff = y1 - y2;
    y_x_ratio = y_diff / x_diff;

    float top_y_3 = y1 + (high_x - x1) * y_x_ratio;

    return top_y_1 <= (top_y_2 <= top_y_3 ? top_y_2 : top_y_3) ? top_y_1 : (top_y_2 <= top_y_3 ? top_y_2 : top_y_3);
}

float DataService::getBottomYOnAreaLimit(float low_x, float high_x)
{
    Point* last_point_of_range;
    Point* next_point_of_range;
    vector<Point*> points_in_range;
    uint32_t size = _area.size();

    last_point_of_range = _area[0];
    next_point_of_range = _area[1];

    for (uint32_t i = 1 ; i < size - 1; i++)
    {
        if(_area[i - 1]->getX() <= low_x && _area[i]->getX() >= high_x)
        {
            float x1 = _area[i - 1]->getX();
            float x2 = _area[i]->getX();
            float y1 = _area[i - 1]->getY();
            float y2 = _area[i]->getY();

            float x_diff = x1 - x2;
            float y_diff = y1 - y2;
            float y_x_ratio = y_diff / x_diff;

            float bottom_y_1 = y1 + (low_x - x1) * y_x_ratio;
            float bottom_y_2 = y2 + (high_x - x2) * y_x_ratio;

            return bottom_y_1 <= bottom_y_2 ? bottom_y_1 : bottom_y_2;
        }
        else
        {
            if(_area[i]->getX() < high_x && _area[i]->getX() > low_x)
            {
                points_in_range.push_back(_area[i]);
                next_point_of_range = _area[i + 1];
            }
            else
            {
                if(   _area[i - 1]->getX() <= high_x
                   && _area[i - 1]->getX() >= low_x
                   && _area[i - 1]->getX() != _area[i]->getY()
                   )
                {
                    break;
                }
                last_point_of_range = _area[i];
            }
        } 
    }
    
    size = points_in_range.size();

    //Calculate the X coordinate of the first candidate
    float bottom_y_1 = points_in_range[0]->getY();
    for (Point* point : points_in_range)
    {
        if(point->getX() > bottom_y_1)
        {
            bottom_y_1 = point->getY();
        }
    }
    
    //Calculate the X coordinate of the second candidate
    float x1 = last_point_of_range->getX();
    float x2 = points_in_range[0]->getX();

    float y1 = last_point_of_range->getY();
    float y2 = points_in_range[0]->getY();

    float x_diff = x1 - x2;
    float y_diff = y1 - y2;
    float y_x_ratio = y_diff / x_diff;

    float bottom_y_2 = y1 + (low_x - x1) * y_x_ratio;

    //Calculate the X coordinate of the second candidate
    x1 = points_in_range[size - 1]->getX();
    x2 = next_point_of_range->getX();

    y1 = points_in_range[size - 1]->getY();
    y2 = next_point_of_range->getY();

    x_diff = x1 - x2;
    y_diff = y1 - y2;
    y_x_ratio = y_diff / x_diff;

    float bottom_y_3 = y1 + (high_x - x1) * y_x_ratio;

    return bottom_y_1 <= (bottom_y_2 <= bottom_y_3 ? bottom_y_2 : bottom_y_3) ?
           bottom_y_1 : (bottom_y_2 <= bottom_y_3 ? bottom_y_2 : bottom_y_3);
}

float DataService::getLeftXOnOutlineLimit(float low_y, float high_y)
{
    if(_outline_points.empty())
    {
        return -FLT_MAX;
    }

    float left_x = _outline_points[0]->getX();
    vector<Point*> points_in_range;
    uint32_t size = _outline_points.size();

    for (uint32_t i = 1; i < size - 1; i++)
    {
        if(_outline_points[i]->getY() <= low_y
        && _outline_points[i + 1]->getY() >= high_y
        && _outline_points[i]->getX() == _origin_point_for_placeing->getX())
        {
            return _outline_points[i]->getX();
        }
        if(_outline_points[i]->getY() > low_y && _outline_points[i]->getY() < high_y)
        {
            points_in_range.push_back(_outline_points[i]);
        }
    }
    
    if(points_in_range.empty())
    {
        return -FLT_MAX;
    }
    else
    {
        for (Point* point : points_in_range)
        {
            if(point->getX() > left_x)
            {
                left_x = point->getX();
            }
        }
        return left_x;
    }
}

float DataService::getBottomYOnOutlineLimit(float low_x, float high_x)
{
    if(_outline_points.empty())
    {
        return -FLT_MAX;
    }

    float bottom_y = _outline_points[0]->getY();
    vector<Point*> points_in_range;
    uint32_t size = _outline_points.size();

    for (uint32_t i = size - 1; i > 0; i--)
    {
        if(_outline_points[i]->getX() <= low_x
        && _outline_points[i - 1]->getX() >= high_x
        && _outline_points[i]->getY() >= _origin_point_for_placeing->getY())
        {
            return _outline_points[i]->getY();
        }

        if(_outline_points[i]->getX() > low_x && _outline_points[i]->getX() < high_x)
        {
            points_in_range.push_back(_outline_points[i]);
        }
    }
    

    if(points_in_range.empty())
    {
        return -FLT_MAX;
    }
    else
    {
        for (Point* point : points_in_range)
        {
            if(point->getY() > bottom_y)
            {
                bottom_y = point->getY();
            }
        }
        return bottom_y;
    }
}

void DataService::randomDisturb()
{
    uint32_t modules_num = this->getModules().size();

    srand(time(NULL));
    int rand_num = rand() % 4;

    switch (rand_num)
    {
    //rotate module
    case 0:
        {
            srand(time(NULL));
            int node_id = rand() % modules_num;
            setIdOfNode(node_id);
            findNodeById(_b_star_tree);
            Node* node = _node_finding;

            rotateModule(node->getModule());

            node->getModule()->arrangePoints();
        }
        break;
    
    //flip module
    case 1:
        {
            srand(time(NULL));
            int node_id = rand() % modules_num;
            setIdOfNode(node_id);
            findNodeById(_b_star_tree);
            Node* node = _node_finding;

            flipModule(node->getModule());

            node->getModule()->arrangePoints();
        }
        break;

    //move a node
    case 2:
        {
            srand(time(NULL));
            int node_id_1 = rand() % modules_num;
            setIdOfNode(node_id_1);
            findNodeById(_b_star_tree);
            Node* node_1 = _node_finding;

            int node_id_2 = node_id_1;
            while (node_id_2 == node_id_1)
            {
                srand(time(NULL));
                node_id_2 = rand() % modules_num;
            }
            
            setIdOfNode(node_id_2);
            findNodeById(_b_star_tree);
            Node* node_2 = _node_finding;

            moveNode(node_1, node_2);
        }
        break;

    //exchange two nodes
    case 3:
        {
            srand(time(NULL));
            int node_id_1 = rand() % modules_num;
            setIdOfNode(node_id_1);
            findNodeById(_b_star_tree);
            Node* node_1 = _node_finding;

            int node_id_2 = node_id_1;
            while (node_id_2 == node_id_1)
            {
                srand(time(NULL));
                node_id_2 = rand() % modules_num;
            }
            
            setIdOfNode(node_id_2);
            findNodeById(_b_star_tree);
            Node* node_2 = _node_finding;

            Module* temp = node_1->getModule();
            node_1->setModule(node_2->getModule());
            node_2->setModule(temp);
        }   
        break;

    default:
        break;
    }
}

Orient DataService::changeOrient(Orient old_orient, Orient new_orient)
{
    int high_orient_int = old_orient >= new_orient ? old_orient : new_orient;
    int low_orient_int  = old_orient < new_orient ? old_orient : new_orient;

    if(high_orient_int < 4 && low_orient_int < 4)
    {
        Orient orient = (Orient) ( (high_orient_int + low_orient_int) % 4);
        return orient;
    }

    if(high_orient_int >= 4 && low_orient_int >= 4)
    {
        Orient orient = (Orient) ( (high_orient_int + low_orient_int - 8) % 4);
        return orient;
    }

    Orient orient = (Orient) ( (high_orient_int + low_orient_int - 4) % 4 + 4);
    return orient;
}

void DataService::changePointsOrder(vector<Point*> boundary, Point* origin_point, bool is_flip)
{
    uint8_t points_num = boundary.size();
    uint8_t move_back_times = 0;

    if(is_flip)
    {
        for (size_t i = 0; i < points_num / 2; i++)
        {
            Point* temp = boundary[i];
            boundary[i] = boundary[points_num - i - 1];
            boundary[points_num - i - 1] = temp;
        }
    }

    while (boundary[move_back_times] != origin_point)
    {
        ++move_back_times;
    }
        
    for (size_t i = 0; i < points_num; i++)
    {
        Point* temp = boundary[(i + move_back_times) % points_num];
        boundary[(i + move_back_times) % points_num] = boundary[i];
        boundary[i] = temp;
    }
}

void DataService::rotateModule(Module* module)
{
    srand(time(NULL));
    int rand_num = rand() % 3;

    Orient orient;

    switch (rand_num)
    {
        
    case 0:
        {
            orient = Orient::R90;
        }
        break;
        
    case 1:
        {
            orient = Orient::R180;
        }
        break;
        
    case 2:
        {
            orient = Orient::R270;
        }
        break;

    default:
        break;
    }

    switch (module->getShape())
        {
        case Shape::RECT:
            {
                rotateRect(module, orient);
            }
            break;
        
        case Shape::LSHAPE:
            {
                rotateLShape(module, orient);
            }
            break;
        
        case Shape::TSHAPE:
            {
                rotateTShape(module, orient);
            }
            break;

        default:
            break;
        }

    module->setOrient(changeOrient(module->getOrient(), orient));
}

void DataService::rotatePoint(Point* rotate_point, Point* origin_point, Orient orient)
{
    float x = rotate_point->getX();
    float y = rotate_point->getY();
    float origin_x = origin_point->getX();
    float origin_y = origin_point->getY();
    float x_diff = x - origin_x;
    float y_diff = y - origin_y;

    switch (orient)
    {
    case Orient::R90:
        {
            rotate_point->setPoint(origin_x - y_diff, origin_y + x_diff);
        }
        break;
    
    case Orient::R180:
        {
            rotate_point->setPoint(origin_x - x_diff, origin_y - y_diff);
        }
        break;

    case Orient::R270:
        {
            rotate_point->setPoint(origin_x + y_diff, origin_y - x_diff);
        }
        break;

    default:
        break;
    }
}

void DataService::rotateRect(Module* module, Orient orient)
{
    if(orient == Orient::R180)
    {
        return;
    }

    vector<Point*> boundary = module->getBoundary();

    rotatePoint(boundary[1], boundary[0], orient);
    rotatePoint(boundary[2], boundary[0], orient);
    rotatePoint(boundary[3], boundary[0], orient);

    for (Port* port : module->getPorts())
    {
        rotatePoint(port->getCenter_Point(), boundary[0], orient);
    }

    changePointsOrder(boundary, boundary[0], false);            
}

void DataService::rotateLShape(Module* module, Orient orient)
{
    vector<Point*> boundary = module->getBoundary();

    rotatePoint(boundary[1], boundary[0], orient);
    rotatePoint(boundary[2], boundary[0], orient);
    rotatePoint(boundary[3], boundary[0], orient);
    rotatePoint(boundary[4], boundary[0], orient);
    rotatePoint(boundary[5], boundary[0], orient);

    for (Port* port : module->getPorts())
    {
        rotatePoint(port->getCenter_Point(), boundary[0], orient);
    }

    changePointsOrder(boundary, boundary[0], false); 
}

void DataService::rotateTShape(Module* module, Orient orient)
{
    vector<Point*> boundary = module->getBoundary();

    rotatePoint(boundary[1], boundary[0], orient);
    rotatePoint(boundary[2], boundary[0], orient);
    rotatePoint(boundary[3], boundary[0], orient);
    rotatePoint(boundary[4], boundary[0], orient);
    rotatePoint(boundary[5], boundary[0], orient);
    rotatePoint(boundary[6], boundary[0], orient);
    rotatePoint(boundary[7], boundary[0], orient);

    for (Port* port : module->getPorts())
    {
        rotatePoint(port->getCenter_Point(), boundary[0], orient);
    }

    changePointsOrder(boundary, boundary[0], false); 
}

void DataService::flipModule(Module* module)
{
    srand(time(NULL));
    int rand_num = rand() % 4;

    Orient orient;

    switch (rand_num)
    {
        
    case 0:
        {
            orient = Orient::MX;
        }
        break;
        
    case 1:
        {
            orient = Orient::MY;
        }
        break;
        
    case 2:
        {
            orient = Orient::MXR90;
        }
        break;

    case 3:
        {
            orient = Orient::MYR90;
        }

    default:
        break;
    }

    switch (module->getShape())
        {
        case Shape::RECT:
            {
                flipRect(module, orient);
            }
            break;
        
        case Shape::LSHAPE:
            {
                flipLShape(module, orient);
            }
            break;
        
        case Shape::TSHAPE:
            {
                flipTShape(module, orient);
            }
            break;

        default:
            break;
        }

    module->setOrient(changeOrient(module->getOrient(), orient));
}

void DataService::flipPoint(Point* flip_point, Point* origin_point, Orient orient)
{
    switch (orient)
    {
    case Orient::MX:
        {
            flip_point->setPoint(flip_point->getX(), origin_point->getY() * 2 - flip_point->getY());
        }
        break;
    
    case Orient::MY:
        {
            flip_point->setPoint(origin_point->getX() * 2 - flip_point->getX(), flip_point->getY());
        }
        break;

    case Orient::MXR90:
        {
            flip_point->setPoint(flip_point->getY(), flip_point->getX());
        }
        break;

    case Orient::MYR90:
        {
            flip_point->setPoint(origin_point->getX() * 2 - flip_point->getY(), origin_point->getY() * 2 - flip_point->getX());
        }
        break;

    default:
        break;
    }
}

void DataService::flipRect(Module* module, Orient orient)
{
    if(orient == Orient::MX || orient == Orient::MY)
    {
        return;
    }

    vector<Point*> boundary = module->getBoundary();

    flipPoint(boundary[1], boundary[0], orient);
    flipPoint(boundary[2], boundary[0], orient);
    flipPoint(boundary[3], boundary[0], orient);

    for (Port* port : module->getPorts())
    {
        flipPoint(port->getCenter_Point(), boundary[0], orient);
    }

    changePointsOrder(boundary, boundary[0], true);                
}

void DataService::flipLShape(Module* module, Orient orient)
{
    vector<Point*> boundary = module->getBoundary();

    flipPoint(boundary[1], boundary[0], orient);
    flipPoint(boundary[2], boundary[0], orient);
    flipPoint(boundary[3], boundary[0], orient);
    flipPoint(boundary[4], boundary[0], orient);
    flipPoint(boundary[5], boundary[0], orient);

    for (Port* port : module->getPorts())
    {
        flipPoint(port->getCenter_Point(), boundary[0], orient);
    }

    changePointsOrder(boundary, boundary[0], true); 
}

void DataService::flipTShape(Module* module, Orient orient)
{
    vector<Point*> boundary = module->getBoundary();

    flipPoint(boundary[1], boundary[0], orient);
    flipPoint(boundary[2], boundary[0], orient);
    flipPoint(boundary[3], boundary[0], orient);
    flipPoint(boundary[4], boundary[0], orient);
    flipPoint(boundary[5], boundary[0], orient);
    flipPoint(boundary[6], boundary[0], orient);
    flipPoint(boundary[7], boundary[0], orient);
    
    for (Port* port : module->getPorts())
    {
        flipPoint(port->getCenter_Point(), boundary[0], orient);
    }

    changePointsOrder(boundary, boundary[0], true); 
}

void DataService::moveNode(Node* node_1, Node* node_2)
{
    //find a leaf node of node_1 replace node_1
    Node* leaf_node = node_1;

    while (leaf_node->getLChildNode() != nullptr || leaf_node->getRChildNode() != nullptr)
    {
        if(leaf_node->getLChildNode() == nullptr)
        {
            leaf_node = leaf_node->getRChildNode();
        }
        else  if(leaf_node->getRChildNode() == nullptr)
        {
            leaf_node = leaf_node->getLChildNode();
        }
        else
        {
            srand(time(NULL));
            int left_or_right = rand() % 2;

            if(left_or_right == 0)
            {
                leaf_node = leaf_node->getLChildNode();
            }
            else
            {
                leaf_node = leaf_node->getRChildNode();
            }
        }      
    }
    
    //leaf node replace with node_1
    Module* temp = leaf_node->getModule();
    leaf_node->setModule(node_1->getModule());
    node_1->setModule(temp);

    //move leaf_node to node_2' parent node
    if(leaf_node->getParentNode()->getLChildNode() == leaf_node)
    {
        leaf_node->getParentNode()->setLChildNode(nullptr);
    }
    else
    {
        leaf_node->getParentNode()->setRChildNode(nullptr);
    }

    leaf_node->setParentNode(nullptr);
    leaf_node->setLChildNode(nullptr);
    leaf_node->setRChildNode(nullptr);

    if(node_2->getParentNode() == nullptr)
    {
        
    }
    else if(node_2->getParentNode()->getLChildNode() == node_2)
    {
        node_2->getParentNode()->setLChildNode(leaf_node);
        leaf_node->setParentNode(node_2->getParentNode());
    }
    else
    {
        node_2->getParentNode()->setRChildNode(leaf_node);
        leaf_node->setParentNode(node_2->getParentNode());
    }

    srand(time(NULL));
    int left_or_right = rand() % 2;

    if(left_or_right == 0)
    {
        leaf_node->setLChildNode(node_2);
        node_2->setParentNode(leaf_node);
    }
    else
    {
        leaf_node->setRChildNode(node_2);
        node_2->setParentNode(leaf_node);
    }
}

float DataService::computeNetsLength()
{
    float total_wire_length = 0;

    for (vector<Port*> net : _nets)
    {
        float lx, ly, hx, hy;
        
        lx = hx = net[0]->getCenter_Point()->getX();
        ly = hy = net[0]->getCenter_Point()->getY();

        for (Port* port : net)
        {
            if(port->getCenter_Point()->getX() < lx)
            {
                lx = port->getCenter_Point()->getX();
            }
            if(port->getCenter_Point()->getX() > hx)
            {
                hx = port->getCenter_Point()->getX();
            }
            if(port->getCenter_Point()->getY() < ly)
            {
                ly = port->getCenter_Point()->getY();
            }
            if(port->getCenter_Point()->getY() > hy)
            {
                hy = port->getCenter_Point()->getY();
            }
        }
        
        total_wire_length += (hx - lx) + (hy - ly);
    }

    return total_wire_length;
}

void DataService::computeOffset()
{
    for (Module* module : _modules)
    {
        float x_add = 0;
        float y_add = 0;

        uint8_t points_num = module->getBoundary().size();

        for (Point* point : module->getBoundary())
        {
            x_add += point->getX();
            y_add += point->getY();
        }
        
        float x_average = x_add / points_num;
        float y_average = y_add / points_num;

        Point* offset = new Point();
        offset->setPoint(x_average, y_average);

        module->setOffset(offset);
    } 
}

void DataService::simulatedAnnealing()
{
    float pre_cost, cur_cost, best_cost, diff_cost;

    int init_disturb_times = 100 * _modules.size();
    float average_uphill_cost = computeAverageUphillCost(init_disturb_times);

    best_cost = computeNetsLength();

    float P = 0.9;//Empirical value, only used for formula calculation
    int k = 7;//The parameter K in the three segment function formula given in this paper
    float reject_rate = 0;//Rejection rate
    float conv_rate = 0.99;//When the convergence rate is greater than the rejection rate,
                           //the cooling cycle is introduced

    int count = 0;//Count the cooling times
    int worse_count = 0;//Count the number of times the result is worse than the last time
    int better_count = 0;//Count the number of times random results are better than the last one
    int reject_times = 0;

    float T = average_uphill_cost / log(1 / P);

    do
    {
        ++count;

        int count_disturb = 0;//disturb times
        int uphill = 0;//uphill times
        int cycle_index = 0;//Number of cycles
        float diff_cost_average = 0;


        for (; uphill < init_disturb_times && cycle_index < 2 * init_disturb_times; cycle_index++)
        {
            randomDisturb();
            initPlaceModules();
            cur_cost = computeNetsLength();
            diff_cost = cur_cost - pre_cost;

            float p = 1 / (exp(diff_cost / T));//Probability of accepting worse solution
            p = p > 1 ? 1 : p;

            ++count_disturb;

            diff_cost_average += fabs(diff_cost);

            //Randomly decide whether to accept a worse solution
            srand(time(NULL));   
            int random = float(rand() % 10000) / 10000;

            if(diff_cost <= 0 || random < p)
            {
                pre_cost = cur_cost;

                if(diff_cost > 0)
                {
                    ++uphill;
                    ++worse_count;
                }
                else if(diff_cost < 0)
                {
                    ++better_count;
                }

                if(pre_cost < best_cost)
                {   
                    computeOffset();
                    outputResult();
                    ++better_count;
                }
            }
            else
            {
                ++reject_times;
            }
        }

        if(better_count == 0)
        {
            ++worse_count;
        }
        else
        {
            worse_count = 0;
        }

        diff_cost_average /= count_disturb;
        reject_rate = float(reject_times) / cycle_index;

        if(count < k)
        {
            T = (T / 100) / ((float)count / diff_cost_average);
        }
        else
        {
            T = T / ((double)count / diff_cost_average);
        } 
    } while ((reject_rate < conv_rate && T>1e-4) || worse_count <= 3);
}

float DataService::computeAverageUphillCost(uint32_t disturb_times)
{
    int uphill_times = 0;
    float total_uphill_cost = 0;
    float pre_cost, cur_cost, best_cost;

    //init first placement
    initBStarTree();
    initPlaceModules();
    pre_cost = computeNetsLength();
    computeOffset();
    outputResult();

    for (size_t i = 0; i < disturb_times; i++)
    {
        randomDisturb();
        initPlaceModules();

        cur_cost = computeNetsLength();
        if (cur_cost > pre_cost)
        {
            ++uphill_times;
            total_uphill_cost += cur_cost - pre_cost;
            pre_cost = cur_cost;
        }
        if (cur_cost < best_cost)
        {
            best_cost = cur_cost;
        }
    }
    
    return total_uphill_cost / uphill_times;
}

void DataService::outputResult()
{
    string file_path = "./../output/result.txt";
    std::ofstream result_file(file_path);
    if (result_file.is_open()) 
    {
        for (Module* module : _modules)
        {
            result_file << "Module: " << module->getName() << std::endl;
            result_file << "Orient: " << module->getOrient() << std::endl;
            result_file << "Offset :(" << module->getOffset()->getX() << ", "
                        << module->getOffset()->getY() << ")" << std::endl;
        }

        result_file.close();
    }
}
