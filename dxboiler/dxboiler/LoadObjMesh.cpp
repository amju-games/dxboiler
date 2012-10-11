#include "LoadObjMesh.h"

#define _XM_NO_INTRINSICS_
#include <xnamath.h>
#include <vector>
#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>


struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
};

struct Face
{
	Vertex m_vertices[3];
};

typedef std::vector<std::string> Strings;

void Trim(std::string* ps)
{
	assert(ps);
	while (!ps->empty() && (ps->at(0) == ' ' || ps->at(0) == '\r' || ps->at(0) == '\n' ) )
	{
		*ps = ps->substr(1);
	}
	while (!ps->empty() && (ps->at(ps->size() - 1) == ' ' || ps->at(ps->size() - 1) == '\r' || ps->at(ps->size() - 1) == '\n') )
	{
		*ps = ps->substr(0, ps->size() - 1);
	}
}

Strings Split(const std::string& cs, char c)
{
  Strings r;
  std::string s(cs);
  while (true) 
  {
	int i = s.find(c);
	if (i == std::string::npos)
	{
	  // No special character found, so push back the entire string and finish.
	  r.push_back(s);
	  break; 
	}    
	else
	{
	  // Found the special character. 
	  // Push back whatever is before the character, then work on the remainder
	  // of the string.
	  r.push_back(s.substr(0, i));
	  s = s.substr(i + 1);
	  Trim(&s);
	  if (s.empty())
	  {
		  break;
	  }
	}
  }
  return r;
}

float ToFloat(const std::string& str)
{
	return atof(str.c_str());
}

int ToInt(const std::string& str)
{
	return atoi(str.c_str());
}
XMFLOAT3 ToVec3(const Strings& strs)
{
	if (strs.size() != 4)
	{
		assert(0);
	}
	return XMFLOAT3(ToFloat(strs[1]), ToFloat(strs[2]), ToFloat(strs[3]));
}
XMFLOAT2 ToVec2(const Strings& strs)
{
	if (strs.size() != 3)
	{
		assert(0);
	}
	return XMFLOAT2(ToFloat(strs[1]), ToFloat(strs[2]));
}

struct FaceIndices
{
	int indices[3];
};

typedef std::vector<XMFLOAT3> TanVec;


inline XMFLOAT3 operator+=(XMFLOAT3& a,XMFLOAT3& b)
{
	return XMFLOAT3(a.x + b.x,
					a.y + b.y,
					a.z + b.z);

}

inline XMFLOAT3 ToFLOAT3(XMVECTOR& v)
{
	return XMFLOAT3(v.x/v.w,
					v.y/v.w,
					v.z/v.w);
}

void CalculateTangent1(Face& f, FaceIndices& fi,TanVec& tan1, TanVec& tan2)
{
	float x1 = f.m_vertices[1].pos.x - f.m_vertices[0].pos.x;
	float x2 = f.m_vertices[2].pos.x - f.m_vertices[0].pos.x;
	float y1 = f.m_vertices[1].pos.y - f.m_vertices[0].pos.y;
	float y2 = f.m_vertices[2].pos.y - f.m_vertices[0].pos.y;
	float z1 = f.m_vertices[1].pos.z - f.m_vertices[0].pos.z;
	float z2 = f.m_vertices[2].pos.z - f.m_vertices[0].pos.z;
		
	float s1 = f.m_vertices[1].uv.x - f.m_vertices[0].uv.x;
	float s2 = f.m_vertices[2].uv.x - f.m_vertices[0].uv.x;
	float t1 = f.m_vertices[1].uv.y - f.m_vertices[0].uv.y;
	float t2 = f.m_vertices[2].uv.y - f.m_vertices[0].uv.y;

	float r = 1.0F / (s1 * t2 - s2 * t1);

	XMFLOAT3 sdir((t2 * x1 - t1 * x2) * r, 
				  (t2 * y1 - t1 * y2) * r,
				  (t2 * z1 - t1 * z2) * r);

	XMFLOAT3 tdir((s1 * x2 - s2 * x1) * r, 
				  (s1 * y2 - s2 * y1) * r,
				  (s1 * z2 - s2 * z1) * r);

	tan1[fi.indices[0]] += sdir;
	tan1[fi.indices[1]] += sdir;
	tan1[fi.indices[2]] += sdir;

	tan2[fi.indices[0]] += tdir; 
	tan2[fi.indices[1]] += tdir; 
	tan2[fi.indices[2]] += tdir; 
	
}

void CalculateTangent2(Face& f, FaceIndices& fi,TanVec& tan1, TanVec& tan2)
{
	for(int i = 0; i < 3; ++i)
	{   
		XMVECTOR n = XMLoadFloat3(&f.m_vertices[i].normal);
		XMVECTOR t = XMLoadFloat3(&tan1[fi.indices[i]]);
		XMVECTOR tangent;

		tangent = XMVectorScale(XMVectorSubtract(t,n), XMVector3Dot(n,t).x);
		tangent = XMVector3Normalize(tangent);
		
		tangent.w = XMVector3Dot(XMVector3Cross(n,t),XMLoadFloat3(&tan2[fi.indices[i]])).x < 0.0 ? -1.0f : 1.0f;

		f.m_vertices[i].tangent = ToFLOAT3(tangent);
	}
}

class File
{
public:
	bool OpenRead(const std::string& filename)
	{
		m_f.open(filename.c_str());

		if (!m_f.good())
		{
			return false;
		}

		return true;
	}
	bool GetInteger(int* pIntResult)
	{
		std::string s;
		GetString(&s);
		*pIntResult = atoi(s.c_str());
		return true;
	}	

	bool GetString(std::string* pStringResult)
	{
		if (!m_f.good())
		{
			return false;
		}

		const int MAX_BUF_SIZE = 2000;
		char buf[MAX_BUF_SIZE];

		m_f.getline(buf,MAX_BUF_SIZE);

		*pStringResult = buf;

		return true;
	}
private:
	std::ifstream m_f;
};

bool LoadObjMesh(DXMesh** MeshInOut, ID3D11DeviceContext* d3dcontext,std::string& filename,bool indexed)
{
	File objFile;
	objFile.OpenRead(filename);

	std::vector<XMFLOAT3> vertices;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;

	Strings tempStrings;
	std::string line;

	std::vector<Face> Faces;
	std::vector<FaceIndices> FIVec;

	while (objFile.GetString(&line))
	{
		if (!line.find("v "))
		{
			tempStrings = Split(line,' ');

			vertices.push_back(ToVec3(tempStrings));
		}
		if (!line.find("vt "))
		{

			tempStrings = Split(line,' ');

			texcoords.push_back(ToVec2(tempStrings));
		}
		if (!line.find("vn "))
		{
			tempStrings = Split(line,' ');

			normals.push_back(ToVec3(tempStrings));
		}
		if (!line.find("f "))
		{
			Face tempFace;
			FaceIndices tempFI;

			tempStrings = Split(line,' ');

			Strings temp =  Split(tempStrings[1],'/');

			int posIndex = ToInt(temp[0]) - 1;
			tempFace.m_vertices[0].pos = vertices[posIndex];
			tempFace.m_vertices[0].uv = texcoords[ToInt(temp[1]) - 1];	
			tempFace.m_vertices[0].normal = normals[ToInt(temp[2]) - 1];
			tempFI.indices[0] = posIndex;

			temp =  Split(tempStrings[2],'/');

			posIndex = ToInt(temp[0]) - 1;
			tempFace.m_vertices[1].pos = vertices[posIndex];
			tempFace.m_vertices[1].uv = texcoords[ToInt(temp[1]) - 1];	
			tempFace.m_vertices[1].normal = normals[ToInt(temp[2]) - 1];
			tempFI.indices[1] = posIndex;

			temp =  Split(tempStrings[3],'/');

			posIndex = ToInt(temp[0]) - 1;
			tempFace.m_vertices[2].pos = vertices[posIndex];
			tempFace.m_vertices[2].uv = texcoords[ToInt(temp[1]) - 1];	
			tempFace.m_vertices[2].normal = normals[ToInt(temp[2]) - 1];
			tempFI.indices[2] = posIndex;

			Faces.push_back(tempFace);
			FIVec.push_back(tempFI);
		}

	}
	
	TanVec tan1,tan2;
	tan1.resize(vertices.size());
	tan2.resize(vertices.size());

	for(int i = 0; i < Faces.size(); ++i)
	{
		CalculateTangent1(Faces[i],FIVec[i],tan1,tan2);
	}
	
	for(int i = 0; i < Faces.size(); ++i)
	{
		CalculateTangent2(Faces[i],FIVec[i],tan1,tan2);
	}

	return true;
}