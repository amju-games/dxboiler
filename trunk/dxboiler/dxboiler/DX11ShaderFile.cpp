/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "DX11ShaderFile.h"

bool DX11ShaderFile::OpenRead(const std::string& filename)
{
    m_if.open(filename.c_str());
    return m_if.good();
}

bool DX11ShaderFile::GetString(std::string* result)
{
  if (m_if.eof())
  {
    return false;
  }

  getline(m_if, *result);
  return true;
}


