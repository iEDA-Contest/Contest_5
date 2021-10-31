#include <float.h>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Orient{
    R0 = 0,
    R90,
    R180,
    R270,
    MX,
    MXR90,
    MY,
    MYR90
};

enum Shape{
    RECT = 0,
    LSHAPE,
    TSHAPE
};

class Point
{
public:
    Point() = default;
    Point(float x, float y);
    ~Point() = default;
//getter
    const float getX() const { return _x; }
    const float getY() const { return _y; }
//setter
    void setX(float x) { _x = x; }
    void setY(float y) { _y = y; }
    void setPoint(float x, float y) { _x = x; _y = y; }

private:
    float _x;
    float _y;
};

class Port
{
public:
    Port() = default;
    ~Port() = default;
//getter
    string getModuleName() { return _module_name; }
    vector<Port*>* getNet() { return _nets; }
    Point* getCenter_Point() { return _center_point; }
//setter
    void setModuleName(string name) { _module_name = name; }
    void setNet(vector<Port*>* nets) { _nets = nets; }
    void setCenterPoint(Point* center_point) { _center_point = center_point; }

private:
    string _module_name;
    vector<Port*>* _nets;
    Point* _center_point;
};

class Module
{
public:
    Module() = default;
    Module(string name);
    ~Module() = default;
//getter
    const string getName() const { return _name; }
    const float getWidth() const { return _width; }
    const float getHeight() const { return _height; }
    vector<Point*>& getBoundary() { return _boundary; }
    Point* findPoint(float x, float y);
    vector<Port*>& getPorts() { return _ports; }
    const Shape getShape() const { return _shape; }
    const Orient getOrient() const { return _orient; }
    const Point* getOffset() const { return _offset; }
    vector<Module*>& getSubModule() { return _sub_modules; }
    const float getYDiff() const { return _y_diff; }
    
//setter
    void setName(string name) { _name = name; }
    void setWidth(float width) { _width = width; }
    void setHeight(float height) { _height = height; }
    void addBoundaryPoint(Point* point) { _boundary.push_back(point); }
    void addPort(Port* port) { _ports.push_back(port); }
    void setShape(Shape shape) { _shape = shape; }
    void setOrient(Orient orient) { _orient = orient; }
    void setOffset(Point* offset) { _offset = offset; }
    void addSubModule(Module* module) { _sub_modules.push_back(module); }
    void setYDiff(float y_diff) { _y_diff = y_diff; }
//operator
    void cutIntoRect(vector<Point*> boundarys);//Cut the L-shaped and T-shaped modules vertically into 2 or 3 rectangular sub modules.
    void arrangePoints();//Arrange the boundary points from small to large according to the Y coordinate, 
                         //and then from small to large according to the X coordinate.

private:
    string _name;
    vector<Point*> _boundary;
    float _width;
    float _height;
    vector<Port*> _ports;
    Shape _shape;

    Orient _orient;
    Point* _offset;

    vector<Module*> _sub_modules;
    float _y_diff;//Y coordinate of sub sub module minus y coordinate of main sub module
};

class Node
{
public:
    Node() = default;
    Node(uint32_t id);
    ~Node() = default;
//getter
    const uint32_t getId() const { return _id; }
    Module* getModule() const { return _module; }
    Node* getParentNode() const { return _parent; }
    Node* getLChildNode() const { return _lchild; }
    Node* getRChildNode() const { return _rchild; }
//setter
    void setId(uint32_t id) { _id = id; }
    void setModule(Module* module) { _module = module; }
    void setParentNode(Node* parent) { _parent = parent; }
    void setLChildNode(Node* lchild) { _lchild = lchild; }
    void setRChildNode(Node* rchild) { _rchild = rchild; }

private:
    uint32_t _id;
    Module* _module;
    Node* _parent;
    Node* _lchild;
    Node* _rchild;
};

class DataService
{
public:
    DataService() = default;
    ~DataService() = default;
//getter
    vector<Point*>& getArea() { return _area; }
    vector<Module*>& getModules() { return _modules; }
    vector<vector<Port*>>& getNets() { return _nets; }
    Node* getRootNode() { return _b_star_tree; }
    const Point* getOriginPoint() const { return _origin_point_for_placeing; }
    int getIdOfNode() { return _node_id; }

//setter
    void addAreaPoint(Point* point) { _area.push_back(point); }
    void addModule(Module* module) { _modules.push_back(module); }
    void setBStarTreeRootNode(Node* node) { _b_star_tree = node; }
    void setOriginPoint(Point* origin_point_for_placeing) { _origin_point_for_placeing = origin_point_for_placeing; }
    void setIdOfNode(int id) { _node_id = id; }

//operator

    //find data
    Module* findModule(string name);
    void findNodeById(Node* root);

    //read initial data
    bool readAreaEtcFile(string file_path);
    bool readLinkFile(string file_path);
    Point* calculationCenterPoint(vector<Point*> boundary);

    // void initialSortModules();//make modules that have connections together

    //initialize a b * Tree
    void recursiveConstructionNode(Node* node, uint32_t index);
    void initBStarTree();
    
    //place the layout according to the B * tree
    bool initPlaceModules();
    void placeModule(Node* root);

    void placeRectShape(Module* module);
    void placeLShape(Module* module);
    void placeTShape(Module* module);
    
    float placeSubRect(Module* sub_rect);

    void updateOutLine(Module* module);

    float getLeftXOnAreaLimit(float low_y, float high_y);
    float getRightXOnAreaLimit(float low_y, float high_y);
    float getTopYOnAreaLimit(float low_x, float high_x);
    float getBottomYOnAreaLimit(float low_x, float high_x);

    float getLeftXOnOutlineLimit(float low_y, float high_y);
    float getBottomYOnOutlineLimit(float low_x, float high_x);

    //random disturbance of B * Tree
    void randomDisturb();

    Orient changeOrient(Orient old_orient, Orient new_orient);
    void changePointsOrder(vector<Point*> boundary, Point* origin_point, bool is_flip);
    
    void rotateModule(Module* module);
    void rotatePoint(Point* rotate_point, Point* origin_point, Orient orient);
    void rotateRect(Module* module, Orient orient);
    void rotateLShape(Module* module, Orient orient);
    void rotateTShape(Module* module, Orient orient);

    void flipModule(Module* module);
    void flipPoint(Point* rotate_point, Point* origin_point, Orient orient);
    void flipRect(Module* module, Orient orient);
    void flipLShape(Module* module, Orient orient);
    void flipTShape(Module* module, Orient orient);

    void moveNode(Node* node_1, Node* node_2);

    //compute nets length
    float computeNetsLength();

    //compute modules' offset
    void computeOffset();

    //simulated annealing
    void simulatedAnnealing();
    float computeAverageUphillCost(uint32_t disturb_times);

    //write .gds file
    void writeGDS();

    //output result
    void outputResult();

private:

    vector<Point*> _area;
    vector<Module*> _modules;
    vector<vector<Port*>> _nets;
    Node* _b_star_tree;
    vector<Point*> _outline_points;
    bool _place_modules_success;
    Point* _origin_point_for_placeing;
    uint32_t _node_id;
    Node* _node_finding;
};
