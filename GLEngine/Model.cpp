#include <stdio.h>
#include "Model.h"
#include "Vertex.h"
#include "FileManager.h"

Model::Model() {}

Model::~Model() 
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Model::init(const char *modelFile)
{
	int numOfVertices = 0;
	FILE *pf = FileManager::getInstance()->openFile(modelFile, "r");
	if ( pf == NULL )
	{
		printf("Can't open file.\n");
	}
	fscanf(pf, "NrVertices: %d\n", &numOfVertices);
	Vertex * verticesData = new Vertex[numOfVertices];
	for ( int i = 0; i < numOfVertices; i++ )
	{
		fscanf(pf,"   %*d. pos:[%f, %f, %f]; norm:[%*f, %*f, %*f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
			&verticesData[i].position.x, &verticesData[i].position.y, &verticesData[i].position.z,
			&verticesData[i].uv.x, &verticesData[i].uv.y);
	}
	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numOfVertices, verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	fscanf(pf, "NrIndices: %d\n", &this->mCountOfIndices);
	int* indicesData = new int[this->mCountOfIndices];
	for (int i = 0; i < this->mCountOfIndices; i += 3)
	{
		fscanf(pf, "   %*d.    %d,    %d,    %d\n", &indicesData[i], &indicesData[i+1], &indicesData[i+2]);
	}
	glGenBuffers(1, &this->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * this->mCountOfIndices, indicesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	FileManager::getInstance()->closeFile(pf);

	delete [] verticesData;
	delete [] indicesData;
}