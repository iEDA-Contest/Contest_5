#include "../include/db.h"

void DataService::writeGDS()
{
    string file_path = "/home/yangzonglin/floorplanning/output/result.gds";
    std::ofstream gds_file(file_path);
    if (gds_file.is_open()) 
    {
        gds_file << "HEADER 600" << std::endl;
        gds_file << "BGNLIB" << std::endl;
        gds_file << "LIBNAME DensityLib" << std::endl;
        gds_file << "UNITS 0.001 1e-9" << std::endl;

        //area
        gds_file << "BGNSTR" << std::endl;
        gds_file << "STRNAME area" << std::endl;
        gds_file << "BOUNDARY" << std::endl;
        gds_file << "LAYER 1" << std::endl;
        gds_file << "DATATYPE 0" << std::endl;
        gds_file << "XY" << std::endl;
        
        gds_file << this->getArea()[0]->getX() << " : " << this->getArea()[0]->getY() << std::endl;
        gds_file << this->getArea()[1]->getX() << " : " << this->getArea()[1]->getY() << std::endl;
        gds_file << this->getArea()[2]->getX() << " : " << this->getArea()[2]->getY() << std::endl;
        gds_file << this->getArea()[3]->getX() << " : " << this->getArea()[3]->getY() << std::endl;
        gds_file << this->getArea()[0]->getX() << " : " << this->getArea()[0]->getY() << std::endl;
        gds_file << "ENDEL" << std::endl;
        gds_file << "ENDSTR" << std::endl;
        
        //modules boundary
        uint32_t index = 0;
        for (Module* module : this->getModules())
        {
            gds_file << "BGNSTR" << std::endl;
            gds_file << "STRNAME " << ++index << std::endl;
            gds_file << "BOUNDARY" << std::endl;
            gds_file << "LAYER 1" << std::endl;
            gds_file << "DATATYPE 0" << std::endl;
            gds_file << "XY" << std::endl;
        
            for (Point* point : module->getBoundary())
            {
                gds_file << point->getX() << " : " << point->getY() << std::endl;
            }
            gds_file << module->getBoundary()[0]->getX() << " : " << module->getBoundary()[0]->getY() << std::endl;

            gds_file << "ENDEL" << std::endl;
            gds_file << "ENDSTR" << std::endl;
        }

        gds_file << "BGNSTR" << std::endl;
        gds_file << "STRNAME top" << std::endl;

        //add area
        gds_file << "SREF" << std::endl;
        gds_file << "SNAME area" << std::endl;
        gds_file << "XY 0:0" << std::endl;
        gds_file << "ENDEL" << std::endl;

        //add modules boundary
        index = 0;
        for (Module* module : this->getModules())
        {
            gds_file << "SREF" << std::endl;
            gds_file << "SNAME " << ++index << std::endl;
            gds_file << "XY 0:0" << std::endl;
            gds_file << "ENDEL" << std::endl;
        }

        gds_file << "ENDSTR" << std::endl;

        gds_file << "ENDLIB" << std::endl;
        gds_file.close();
    }
}