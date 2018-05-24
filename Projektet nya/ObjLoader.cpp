#include "ObjLoader.h"
#include<fstream>
using namespace std;

void ObjLoader::expand()
{
	this->capacity += 2000;	
	Vertex* vertexTemp = new Vertex[this->capacity];
	Texture* textureTemp = new Texture[this->capacity];
	Normal* normalTemp = new Normal[this->capacity];

	for (int i = 0; i < this->nrOfVertices; i++)
	{
		vertexTemp[i] = this->vertex[i];		
	}
	for (int i = 0; i < this->nrOfTextureCoords; i++)
	{		
		textureTemp[i] = this->texture[i];		
	}
	for (int i = 0; i < this->nrOfNormalCoords; i++)
	{		
		normalTemp[i] = this->normal[i];
	}
	delete[] this->vertex;
	delete[] this->texture; 
	delete[] this->normal;
	this->vertex = vertexTemp;
	this->texture = textureTemp;
	this->normal = normalTemp;
}

void ObjLoader::expandIndicesArr()
{
	this->capacityForIndices += 1000;
	int* vertexTemp = new int[this->capacityForIndices];
	int* textureTemp = new int[this->capacityForIndices];
	int* normalTemp = new int[this->capacityForIndices];

	for (int i = 0; i < this->nrOfIndices; i++)
	{
		vertexTemp[i] = this->vertexIndices[i];
		textureTemp[i] = this->textureIndices[i];
		normalTemp[i] = this->normalIndices[i];
	}
	delete[] this->vertexIndices;
	delete[] this->textureIndices;
	delete[] this->normalIndices;
	this->vertexIndices = vertexTemp;
	this->textureIndices = textureTemp;
	this->normalIndices = normalTemp;
}

ObjLoader::ObjLoader(int cap)
{
	this->nrOfVertices = 0;
	this->nrOfTextureCoords = 0;
	this->nrOfNormalCoords = 0;
	this->nrOfIndices = 0;	
	this->capacity = cap;
	this->capacityForIndices = 1000;
	this->vertex = new Vertex[this->capacity];
	this->texture = new Texture[this->capacity];
	this->normal = new Normal[this->capacity];
	this->vertexIndices = new int[this->capacityForIndices];
	this->textureIndices = new int[this->capacityForIndices];
	this->normalIndices = new int[this->capacityForIndices];
}

ObjLoader::~ObjLoader()
{
	delete[] this->vertex;
	delete[] this->texture;
	delete[] this->normal;
	delete[] this->vertexIndices;
	delete[] this->textureIndices;
	delete[] this->normalIndices;
}

bool ObjLoader::loadObj(string filename)
{
	bool success = false;
	ifstream myInfileStream;

	myInfileStream.open(filename);	

	char firstCharInLine = (char)0;
	char secondCharInLine = (char)0;
	float vertexPosFloat[3];
	float texU;
	float texV;
	float x, y, z;		
	string waste = "";
	
	while (myInfileStream.is_open())
	{
		myInfileStream.get(firstCharInLine);
		myInfileStream.get(secondCharInLine);

		if (firstCharInLine == 'v' && secondCharInLine != 't' && secondCharInLine != 'n')
		{			
			myInfileStream >> vertexPosFloat[0];			
			myInfileStream >> vertexPosFloat[1];		
			myInfileStream >> vertexPosFloat[2];	
			
			Vertex temp;
			temp.pos[0] = vertexPosFloat[0];
			temp.pos[1] = vertexPosFloat[1];
			temp.pos[2] = vertexPosFloat[2];
			this->vertex[this->nrOfVertices] = temp;			
			this->nrOfVertices++;
			if (this->nrOfVertices == this->capacity)
			{
				this->expand();
			}
		}
		else if (firstCharInLine == 'v' && secondCharInLine == 't')
		{
			myInfileStream >> texU;
			myInfileStream >> texV;				
			
			Texture temp;
			temp.u = texU;
			temp.v = texV;			
			this->texture[this->nrOfTextureCoords] = temp;
			this->nrOfTextureCoords++;
			if (this->nrOfTextureCoords == this->capacity)
			{
				this->expand();
			}
		}
		else if (firstCharInLine == 'v' && secondCharInLine == 'n')
		{			
			myInfileStream >> x;			
			myInfileStream >> y;			
			myInfileStream >> z;

			Normal temp;
			temp.x = x;
			temp.y = y;
			temp.z = z;
			this->normal[this->nrOfNormalCoords] = temp;
			this->nrOfNormalCoords++;
			if (this->nrOfNormalCoords == this->capacity)
			{
				this->expand();
			}
		}
		else if (firstCharInLine == 'f')
		{
			int vertexIndex[3], textureIndex[3], normalIndex[3];
			char nextChar;
			nextChar = myInfileStream.peek();	
			int nrOf = 0;

			for (int i = 0; i < 3; i++)
			{
				myInfileStream >> vertexIndex[i];				
				myInfileStream.get();
				myInfileStream >> textureIndex[i];
				myInfileStream.get();
				myInfileStream >> normalIndex[i];				

				nextChar = myInfileStream.peek();
				if (nextChar != '\n')
				{					
					myInfileStream.get();
				}					
			}
			
			for (int i = 0; i < 3; i++)
			{
				
				this->vertexIndices[this->nrOfIndices] = vertexIndex[i];
				this->textureIndices[this->nrOfIndices] = textureIndex[i];
				this->normalIndices[this->nrOfIndices] = normalIndex[i];
				this->nrOfIndices++;
				if (this->nrOfIndices == this->capacityForIndices)
				{
					this->expandIndicesArr();
				}
			}			
		}
		
		getline(myInfileStream, waste);		

		if (myInfileStream.eof())
		{			
			myInfileStream.close();
			success = true;			
		}		
	}

	return success;
}

void ObjLoader::getVertices(Vertex passedVertexArr[])
{
	
	for (int i = 0; i < this->nrOfIndices; i++)
	{		
		passedVertexArr[i] = this->vertex[this->vertexIndices[i] - 1];
	}
}

void ObjLoader::getTextures(Texture passedTextureArr[])
{
	for (int i = 0; i < this->nrOfIndices; i++)
	{
		passedTextureArr[i] = this->texture[this->textureIndices[i] - 1];
	}
}

void ObjLoader::getNormals(Normal passedNormalArr[])
{
	for (int i = 0; i < this->nrOfIndices; i++)
	{
		passedNormalArr[i] = this->normal[this->normalIndices[i] - 1];
	}
}

int ObjLoader::getNrOfVertices()
{
	return this->nrOfVertices;
}

int ObjLoader::getNrOfIndices()
{
	return this->nrOfIndices;
}
