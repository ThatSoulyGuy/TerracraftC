#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <STBI/stb_image.h>
#include "core/Logger.hpp"
#include "core/Settings.hpp"

#define DEFAULT_TEXTURE_PROPERTIES TextureProperties::Register(GL_REPEAT, GL_NEAREST, true)

struct TextureProperties
{
    int wrapping, precision;
    bool flip;

    static TextureProperties Register(int wrapping, int precision, bool flip)
    {
        TextureProperties out = {};

        out.wrapping = wrapping;
        out.precision = precision;
        out.flip = flip;

        return out;
    }
};

struct Texture
{
	std::string name = "";
	std::string path = "";
    glm::ivec2 size = {0.0f, 0.0f};
	unsigned int textureID = 0;
    unsigned char* data = NULL;

    void GenerateTexture(const TextureProperties& properties = DEFAULT_TEXTURE_PROPERTIES)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, properties.wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, properties.wrapping);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, properties.precision);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, properties.precision);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        int nrComponents;
        stbi_set_flip_vertically_on_load(properties.flip);

        data = stbi_load(path.c_str(), &size.x, &size.y, &nrComponents, 0);

        if (data)
        {
            GLenum format = 0;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            Logger_ThrowError("null", fmt::format("Failed to load texture from path: {}", path), false);

        stbi_image_free(data);
    }

	static Texture Register(const std::string& localPath, const std::string& name, const std::string& domain = Settings::defaultDomain)
	{
		Texture out = {};

		out.name = name;
		out.path = "assets/" + domain + "/" + localPath;

		return out;
	}
};

namespace TextureManager
{
    std::vector<Texture> registeredTextures;

    void RegisterTexture(const Texture& texture)
    {
        registeredTextures.push_back(texture);
    }

    Texture GetTexture(const std::string& name)
    {
        for (Texture texture : registeredTextures)
        {
            if (texture.name == name)
                return texture;
        }
    }
}

#endif // !TEXTURE_HPP