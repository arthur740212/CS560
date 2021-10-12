CS560 Project1 Arthur Chang 60001620
2021/10/10

Viewer Instruction:

WASD to move, SHIFT to speed up
Change View Angle by left click drag
Toggles described in viewer



Structure:

Bone:

Contains a name, an ID, a VQS representing the bone in local space.
A list of key frames of positions, rotations, and scales.


//Updates the local space VQS of the bone;
 void Update(float animationTime);

//Interpolates position between keyframes (linear interpolation)
GetInterpolatedPos(float animationTime);

//Interpolates quaternion between keyframes with Slerp (unoptimized version, still using three trig-functions)
GetInterpolatedQuat(float animationTime);

//Interpolates scale between keyframes (linear interpolation)
GetInterpolatedPos(float animationTime);

//Selects which key frame to use according to current time.
int Get(Position/Rotation/Scale)Index(float animationTime);



-------------------------------------------------------------------------------------------------------
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



-------------------------------------------------------------------------------------------------------------
Animation:

Stores node data with AssimpNodeData Hiearchically, with a name indicating the bone associated with the node


//Add the bones to a map following the hierarchical node structure
void ReadBones(const aiAnimation* animation, Model& model);

//Create the node hierarchy
void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

//Gets the vertices of the bones for point display
void GetSkeletonBones();

//Gets the links between bone vertices for line display
void GetSkeletonBoneHiearchy(const AssimpNodeData* node, int parentBoneIndex);


----------------------------------------------------------------------------------------------------------------
Animator:

Grabs a reference to an animation and runs it


//Updates the time and the Bones VQS according to time
void UpdateAnimation(float dt);

//Calculate the GLOBAL SPACE VQS for each bone following through the hierarchy
//convert and store them into a list of MATRICES, which is later passed into the shader
//for vertex multiplication
void CalculateBoneVQS(const AssimpNodeData* node, VQS parentVQS);



-------------------------------------------------------------------------------------------------------------
Main reference is https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation

Reading/Parsing and Structural code is borrowed from the tutorial with small changes to help understanding.
With self implemented Interpolation/Quaternion/VQS class and functions integrated and replacing the tutorials code.
The tutorial was all matrix based, submitted project is VQS based, matrix conversion only used at model loading and shader computing.