#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <string>
using namespace std;

struct Vertex
{
	float pos[3];
};

struct Texture
{
	float u, v;
};

struct Normal
{
	float x, y, z;
};

class ObjLoader
{
private:

	int nrOfVertices;
	int nrOfTextureCoords;
	int nrOfNormalCoords;
	int nrOfIndices;	
	int capacity;
	int capacityForIndices;
	Vertex * vertex;
	Texture * texture;
	Normal * normal;
	int * vertexIndices, *textureIndices, *normalIndices;

	void expand();
	void expandIndicesArr();

public:
	ObjLoader(int cap = 2000);
	~ObjLoader();
	bool loadObj(string filename);
	void getVertices(Vertex passedVertexArr[]);
	void getTextures(Texture passedTextureArr[]);
	void getNormals(Normal passedNormalArr[]);
	int getNrOfVertices();
	int getNrOfIndices();
};



#endif
