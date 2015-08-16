#include "Shader.h"
#include "FileManager.h"
#include "LogHeaderAndroid.h"
#include "Lib.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	mStates.clear();

	this->mVSpath = new char[ strlen(vertexShaderPath) + 1 ];
	this->mFSpath = new char[ strlen(fragmentShaderPath) + 1 ];
	strcpy( this->mVSpath, vertexShaderPath );
	strcpy( this->mFSpath, fragmentShaderPath );
}
int Shader::init()
{
	mVertexShader = loadShader(GL_VERTEX_SHADER, this->mVSpath);
	if ( mVertexShader == 0 )
	{
		printf("\nLoading %s fail", this->mVSpath);
		return -1;
	}
	mFragmentShader = loadShader(GL_FRAGMENT_SHADER, this->mFSpath);

	if ( mFragmentShader == 0 )
	{
		printf("\nLoading %s fail", this->mFSpath);
		glDeleteShader( mVertexShader );
		return -2;
	}
	mProgram = loadProgram(mVertexShader, mFragmentShader);

	this->mAttrPosition   = glGetAttribLocation  ( this->mProgram, "a_position" );
	this->mAttrUV         = glGetAttribLocation  ( this->mProgram, "a_uv" );
	this->mAttrNormal     = glGetAttribLocation  ( this->mProgram, "a_normal" );
	this->mUniTime        = glGetUniformLocation ( this->mProgram, "u_time" );
	this->mUniColor       = glGetUniformLocation ( this->mProgram, "u_color" );
	this->mUniAmbient     = glGetUniformLocation ( this->mProgram, "u_ambient" );
	this->mUniDiffuse     = glGetUniformLocation ( this->mProgram, "u_diffuse" );
	this->mUniSpecular    = glGetUniformLocation ( this->mProgram, "u_specular" );
	this->mUniTransparent = glGetUniformLocation ( this->mProgram, "u_transparent" );
	this->mUniCameraPos   = glGetUniformLocation ( this->mProgram, "u_cameraPos" );
	this->mUniWorldMatrix = glGetUniformLocation ( this->mProgram, "u_W" );
	this->mUniFinalMatrix = glGetUniformLocation ( this->mProgram, "u_WVP" );
	this->mUniTextures[0] = glGetUniformLocation ( this->mProgram, "u_texture1" );
	this->mUniTextures[1] = glGetUniformLocation ( this->mProgram, "u_texture2" );
	this->mUniTextures[2] = glGetUniformLocation ( this->mProgram, "u_texture3" );
	this->mUniTextures[3] = glGetUniformLocation ( this->mProgram, "u_texture4" );
	return true;
}
void Shader::addState(GLenum state)
{
	mStates.push_back( state );
}
void Shader::enableState()
{
	for( GLuint i = 0; i < mStates.size(); i++ )
	{
		glEnable( mStates[i] );
		if ( mStates[i] == GL_BLEND )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
	}
}
void Shader::disableState()
{
	for( GLuint i = 0; i < mStates.size(); i++ )
	{
		glDisable( mStates[i] );
	}
}
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param Shaderrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint Shader::loadShader ( GLenum type, const char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
		return 0;
	FileManager* fileManager = FileManager::getInstance();
	FILE * pf = fileManager->openFile(filename, "rb");
	if (pf == 0)
	{
		return 0;
	}
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * buffer = new char[size + 1];
	fread(buffer, sizeof(char), size, pf);
	buffer[size] = 0;
	fclose(pf);

	glShaderSource ( shader, 1, (const char **)&buffer, NULL );
	delete [] buffer;
	buffer = NULL;
	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char  [infoLen];


			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			printf ( "\nError compiling shader:\n%s\n", infoLog );            

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}
	return shader;
}


GLuint Shader::loadProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char[sizeof(char) * infoLen];

			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			printf ( "\nError linking program:\n%s\n", infoLog );            

			delete infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}
Shader::~Shader()
{
	if ( mProgram )
	{
		glDeleteProgram(mProgram);
	}

	SAFE_FREE(mVSpath);
	SAFE_FREE(mFSpath);
}

GLenum Shader::getStateByString(const char *stateStr)
{
	if (strstr("GL_DEPTH_TEST", stateStr))
	{
		return GL_DEPTH_TEST;
	}
	if (strstr("GL_CULL_FACE", stateStr))
	{
		return GL_CULL_FACE;
	}
	if (strstr("GL_SAMPLE_ALPHA_TO_COVERAGE", stateStr))
	{
		return GL_SAMPLE_ALPHA_TO_COVERAGE;
	}
}
