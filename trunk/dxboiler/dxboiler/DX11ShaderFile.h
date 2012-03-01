/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef DX11_SHADER_FILE_H_INCLUDED

#include <fstream>
#include <string>

class DX11ShaderFile
{
public:
    bool OpenRead(const std::string& filename);
    bool GetString(std::string* result);

private:
    std::ifstream m_if;
};

#endif
