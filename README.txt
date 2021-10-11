Viewer Instruction:

WASD to move, SHIFT to speed up
Change View Angle by left click drag
Toggles described in viewer



Structure:

Model:
Mainly contains data related to vertices
Stores which bones influences it for each vertice

Loads the mesh/meshes through Assimp


//Sets the influencing bones list to nothing
void SetVertexBoneDataToDefault(Vertex& vertex);

//Adds and influencing bone data to the list
void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

//Get all the vertices influenced by each bone, then add that bone to the vertices affected
void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);


//Stores all the vertices in it, also calls ExtractBoneWeightForVertices(...)
Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, int layer);


Animation:

//Gets the vertices of the bones for point display
void GetSkeletonBones();

//Gets the links between bone vertices for line display
void GetSkeletonBoneHiearchy(const AssimpNodeData* node, int parentBoneIndex);
