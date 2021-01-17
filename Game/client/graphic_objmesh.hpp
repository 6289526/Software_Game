/**
 * @file graphic_objmesh.hpp
 * @brief メッシュファイルの管理クラス
 * 
 */
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include "client_common.h"

using namespace std;

#define OBJ_BUFFER_LENGTH 1024
#define OBJ_NAME_LENGTH 256
#define OBJ_MAGNIFICATION 1

struct OBJVEC2{
	float x;
	float y;
	OBJVEC2( float nx=0.0f, float ny=0.0f );
	operator float* ();
	operator const float* () const;
};
struct OBJVEC3{
	float x;
	float y;
	float z;
	OBJVEC3( float nx=0.0f, float ny=0.0f, float nz=0.0f );
	operator float* ();
	operator const float* () const;
};
struct OBJVERTEX{
	OBJVEC2 texcoord;
	OBJVEC3 normal;
	OBJVEC3 position;
	OBJVERTEX(){}
};

class OBJMESH
{
private:
	OBJVERTEX* m_Vertices;
	unsigned int* m_Indices;
	unsigned int m_NumVertices;
	unsigned int m_NumIndices;
	char m_directoryPath[OBJ_NAME_LENGTH];

	bool LoadMTLFile( const char* filename );
	bool LoadOBJFile( const char* filename );

public:
	OBJMESH();
	~OBJMESH();

	bool LoadFile( const char* filename );
	void Release();
	void Draw(const FloatCube *cube, float dir);
	unsigned int GetNumVertices();
	unsigned int GetNumSubsets();
	unsigned int GetNumMaterials();
	unsigned int GetNumIndices();
	unsigned int  GetIndexData( unsigned int index );
	unsigned int* GetIndices();
	OBJVERTEX  GetVertex( unsigned int index );
	OBJVERTEX* GetVertices();
};
