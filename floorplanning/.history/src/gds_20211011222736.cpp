#include "../include/db.h"

void DataService::writeGDS()
{
    string file_path = "output/result.gds";
    std::ofstream gds_file(file_path);
    if (gds_file.is_open()) 
    {
        gds_file << "HEADER 600" << std::endl;
        gds_file << "BGNLIB" << std::endl;
        gds_file << "LIBNAME DensityLib" << std::endl;
        gds_file << "UNITS 0.001 1e-9" << std::endl;
        
        for (size_t i = 0; i < count; i++)
        {
            gds_file << "BGNSTR" << std::endl;
            gds_file << "STRNAME " << i << std::endl;
            gds_file << "BOUNDARY" << std::endl;
            gds_file << "LAYER 1" << std::endl;
            gds_file << "DATATYPE 0" << std::endl;
            gds_file << "XY" << std::endl;
        
            gds_file << 100 << " : " << 100 << std::endl;
            gds_file << 200 << " : " << 100 << std::endl;
            gds_file << 200 << " : " << 200 << std::endl;
            gds_file << 100 << " : " << 200 << std::endl;
            gds_file << 100 << " : " << 100 << std::endl;
            gds_file << "ENDEL" << std::endl;
            gds_file << "ENDSTR" << std::endl;
        }

        gds_file << "BGNSTR" << std::endl;
        gds_file << "STRNAME top" << std::endl;

        for (size_t i = 0; i < count; i++)
        {
            gds_file << "SREF" << std::endl;
            gds_file << "SNAME " << i << std::endl;
            gds_file << "XY 0:0" << std::endl;
            gds_file << "ENDEL" << std::endl;
        }

        gds_file << "ENDSTR" << std::endl;

        gds_file << "ENDLIB" << std::endl;
        gds_file.close();
    }
}