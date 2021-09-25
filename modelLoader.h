#pragma once
#ifndef MODELLOADER_CLASS_H
#define MODELLOADER_CLASS_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate|aiProcess_GenSmoothNormals|aiProcess_FlipUVs|aiProcess_JoinIdenticalVertices)

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

private:

};




#endif