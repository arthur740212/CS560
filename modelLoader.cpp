#include "modelLoader.h"

ModelLoader::ModelLoader()
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile("BaseMesh_Anim.fbx", ASSIMP_LOAD_FLAGS);

	if (pScene)
	{
		std::cout << "Import success" << std::endl;
	}
	else
	{
		std::cout << "Import fail" << importer.GetErrorString() << std::endl;
	}
}

ModelLoader::~ModelLoader()
{
}