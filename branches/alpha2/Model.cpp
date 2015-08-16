#include <stdio.h>
#include "Model.h"
#include "Vertex.h"
#include "FileManager.h"
#include "Lib.h"
#include "LogHeaderAndroid.h"
#include <string.h>

Model::Model(const char *modelFile)
{
	this->mModelFile = new char[strlen(modelFile) + 1];
	strcpy(mModelFile, modelFile);
	mFirstArr = NULL;
	mCountArr = NULL;
	mAmbientArr = NULL;
	mDiffuseArr = NULL;
	mSpecularArr = NULL;
}

Model::~Model() 
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	SAFE_FREE(mFirstArr);
	SAFE_FREE(mCountArr);
	SAFE_FREE(mModelFile);
	SAFE_FREE(mAmbientArr);
	SAFE_FREE(mDiffuseArr);
	SAFE_FREE(mSpecularArr);
}

void Model::init()
{
	this->mCountOfMaterials = 0;
	this->mCountOfIndices   = 0;
	this->mTransparent      = 0;
	int numOfVertices       = 0;
	FILE *pf = FileManager::getInstance()->openFile(mModelFile, "r");
	if ( pf == NULL )
	{
		printf("Can't open file: %s\n", mModelFile);
	}

	if ( strstr ( mModelFile, "got" ) != NULL )
	{
		printf("Load model type .got\n");
		
		//Read model got

		//Vertices : Pos, Uv, Normal
		fscanf(pf, "NumVerts = %d\n", &numOfVertices);
		Vertex * verticesData = new Vertex[numOfVertices];
		for ( int i = 0; i < numOfVertices; i++ )
		{
			fscanf(pf, "Pos: [%f, %f, %f], Uv: [%f, %f], Normals: [%f, %f, %f]\n",
				&verticesData[i].position.x, &verticesData[i].position.y, &verticesData[i].position.z,
				&verticesData[i].uv.x, &verticesData[i].uv.y,
				&verticesData[i].normal.x, &verticesData[i].normal.y, &verticesData[i].normal.z);
		}

		//Materials
		fscanf(pf, "NumMaterials: %d\n", &this->mCountOfMaterials);

		//First number of vexter to start draw
		this->mFirstArr = new int [this->mCountOfMaterials];
		fscanf(pf, "First\n");
		for ( int i = 0; i < this->mCountOfMaterials; i++ )
		{
			fscanf(pf, "%d\n", &this->mFirstArr[i]);
		}

		//Count number of vertices to draw
		this->mCountArr = new int [this->mCountOfMaterials];
		fscanf(pf, "Count\n");
		for ( int i = 0; i < this->mCountOfMaterials; i++ )
		{
			fscanf(pf, "%d\n", &this->mCountArr[i]);
		}
		
		//Ambient
		this->mAmbientArr = new Vector3 [this->mCountOfMaterials];
		fscanf(pf, "Ambient\n");
		for ( int i = 0; i < this->mCountOfMaterials; i++ )
		{
			fscanf(pf, "[%f, %f, %f]\n",
				&this->mAmbientArr[i].x,
				&this->mAmbientArr[i].y,
				&this->mAmbientArr[i].z);
		}
		
		//Diffuse
		this->mDiffuseArr = new Vector3 [this->mCountOfMaterials];
		fscanf(pf, "Diffuse\n");
		for ( int i = 0; i < this->mCountOfMaterials; i++ )
		{
			fscanf(pf, "[%f, %f, %f]\n",
				&this->mDiffuseArr[i].x,
				&this->mDiffuseArr[i].y,
				&this->mDiffuseArr[i].z);
		}
		
		//Specular
		this->mSpecularArr = new Vector3 [this->mCountOfMaterials];
		fscanf(pf, "Specular\n");
		for ( int i = 0; i < this->mCountOfMaterials; i++ )
		{
			fscanf(pf, "[%f, %f, %f]\n",
				&this->mSpecularArr[i].x,
				&this->mSpecularArr[i].y,
				&this->mSpecularArr[i].z);
		}
		fscanf(pf, "Transparent: %f\n", &this->mTransparent);

		//Gen buffer
		glGenBuffers(1, &this->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numOfVertices, verticesData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		FREE_1D_ARRAY( verticesData );
	}

	else 
	{
		printf("Load model type .nfg\n");

		//Read model nfg

		//Vertices data
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

		//Indices data
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
		
		FREE_1D_ARRAY ( verticesData );
		FREE_1D_ARRAY ( indicesData );
	}
	delete mModelFile;
	mModelFile = NULL;
	FileManager::getInstance()->closeFile(pf);
}