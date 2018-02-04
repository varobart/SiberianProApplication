#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLFW/glfw3.h>


class Texture{

public:
	Texture(const char* texturePath);
	void passToShader(unsigned int shaderID, const char* samplerName, int target);
	void load();
	inline GLuint getTextureID(){ return textureID;}


private:
	const char* texturePath;
	GLuint textureID;
	int width;
	int height;
	void init();



};


#endif