#include <fstream>
#include <stdio.h>
#include <stdlib.h>
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
    vector<Port*>* getNet() { return _net; }
    vector<Point*>& getPoints() { return _points; }
//setter
    void setNet(vector<Port*>* net) { _net = net; }
    void addPoint(Point* point) { _points.push_back(point); }

private:
    vector<Port*>* _net;
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
//setter
    void setName(string name) { _name = name; }
    void addBoundaryPoint(Point* point) { _boundary.push_back(point); }
    void addPort(Port* port) { _ports.push_back(port); }
    void setShape(Shape shape) { _shape = shape; }
    void setOrient(Orient orient) { _orient = orient; }
    void setOffset(Point offset) { _offset = offset; }

private:
    string _name;
    vector<Point*> _boundary;
    vector<Port*> _ports;
    Shape _shape;

    Orient _orient;
    Point _offset;
};

class Node
{
public:
    Node() = default;
    Node(uint32_t id);
    ~Node() = default;
//getter
    const uint32_t getId() const { return _id; }
    const Module* getRootNode() const { return _root; }
    const Module* getLChildNode() const { return _lchild; }
    const Module* getRchildNode() const { return _rchild; }
//setter
    void setRootNode(Module* root) { _root = root; }
    void setLChildNode(Module* lchild) { _lchild = lchild; }
    void setRChildNode(Module* rchild) { _rchild = rchild; }

private:
    uint32_t _id;
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
    bool ReadAreaEtcFile(string file_path);
    bool ReadLinkFile(string file_path);
    Module* findModule(string name);

private:
    vector<Point*> _area;
    vector<Module*> _modules;
    vector<vector<Port*>> _nets;
};
