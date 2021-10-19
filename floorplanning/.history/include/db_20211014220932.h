#include <fstream>
#include <stdio.h>
#include <stdlib.h>
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
    MY,
    MXR90,
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
    vector<Point*>& getPoints() { return _points; }
//setter
    void setModuleName(string name) { _module_name = name; }
    void setNet(vector<Port*>* nets) { _nets = nets; }
    void addPoint(Point* point) { _points.push_back(point); }

private:
    string _module_name;
    vector<Port*>* _nets;
    vector<Point*> _points;
};

class Module
{
public:
    Module() = default;
    Module(string name);
    ~Module() = default;
//getter
    const string getName() const { return _name; }
    vector<Point*>& getBoundary() { return _boundary; }
    vector<Port*>& getPorts() { return _ports; }
    const Shape getShape() const { return _shape; }
    const Orient getOrient() const { return _orient; }
    const Point getOffset() const { return _offset; }
    vector<Module*>& getSubModule() { return _sub_modules; }
//setter
    void setName(string name) { _name = name; }
    void addBoundaryPoint(Point* point) { _boundary.push_back(point); }
    void addPort(Port* port) { _ports.push_back(port); }
    void setShape(Shape shape) { _shape = shape; }
    void setOrient(Orient orient) { _orient = orient; }
    void setOffset(Point offset) { _offset = offset; }
    void addSubModule(Module* module) { _sub_modules.push_back(module); }
//operator
    void cutIntoRect(vector<Point*> boundarys);//Cut the L-shaped and T-shaped modules vertically into 2 or 3 rectangular sub modules.
    void arrangePoints();//Arrange the boundary points from small to large according to the Y coordinate, 
                         //and then from small to large according to the X coordinate.

private:
    string _name;
    vector<Point*> _boundary;
    vector<Port*> _ports;
    Shape _shape;

    Orient _orient;
    Point _offset;

    vector<Module*> _sub_modules;
};

class Node
{
public:
    Node() = default;
    Node(uint32_t id);
    ~Node() = default;
//getter
    const uint32_t getId() const { return _id; }
    const Module* getParentNode() const { return _parent; }
    const Module* getRootNode() const { return _root; }
    const Module* getLChildNode() const { return _lchild; }
    const Module* getRchildNode() const { return _rchild; }
//setter
    void setParentNode(Module* parent) { _parent = parent; }
    void setRootNode(Module* root) { _root = root; }
    void setLChildNode(Module* lchild) { _lchild = lchild; }
    void setRChildNode(Module* rchild) { _rchild = rchild; }

private:
    uint32_t _id;
    Module* _parent;
    Module* _root;
    Module* _lchild;
    Module* _rchild;
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
//setter
    void addAreaPoint(Point* point) { _area.push_back(point); }
    void addModule(Module* module) { _modules.push_back(module); }
//operator
    Module* findModule(string name);
    void initialSortModules();//make modules that have connections together
    bool ReadAreaEtcFile(string file_path);
    bool ReadLinkFile(string file_path);
    void writeGDS();

private:
    vector<Point*> _area;
    vector<Module*> _modules;
    vector<vector<Port*>> _nets;
};
