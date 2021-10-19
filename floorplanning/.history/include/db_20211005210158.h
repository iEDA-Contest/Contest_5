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

struct Point
{
    float x;
    float y;
};

class Port
{
public:
    Port() = default;
    ~Port() = default;
//getter
    vector<Point>& getPoints(){ return _points; }

private:
    vector<Point> _points;
};

class Module
{
public:
    Module(string name);
    ~Module();
//getter
    string getName(){ return _name; }
    vector<Point>& getBoundary(){ return _boundary; }
    vector<Port*>& get_ports() { return _ports; }
    Shape getShape(){ return _shape; }
    Orient getOrient(){ return _orient; }
    Point

private:
    string _name;
    vector<Point> _boundary;
    vector<Port*> _ports;
    Shape _shape;

    Orient _orient;
    Point _offset;
};

class Node
{
public:
    Node(uint32_t id);
    ~Node();

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
private:
    vector<Module> _modules;
    vector<vector<Port*>> _nets;
};
