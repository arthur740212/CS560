#include "texture.h"

Texture::Texture(const char* image, GLenum textureType, GLuint slot, GLenum format, GLenum pixelType) 
{
	type = textureType;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(textureType, ID);
	unit = slot;

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(textureType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(textureType);

	stbi_image_free(bytes);
	glBindTexture(textureType, 0);
}

void Texture::TextureUnit(Shader& shader, const char* uniform, GLuint unit) 
{
	GLuint textUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(textUni, unit);
}
void Texture::Bind() 
{ 
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}
void Texture::Unbind() 
{ 
	glBindTexture(type, 0); 
}
void Texture::Delete() 
{
	glDeleteTextures(1,&ID);
}