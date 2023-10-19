#ifndef RENERER_HPP
#define RENERER_HPP

#define DEFAULT_COLOR glm::vec3{1.0f, 1.0f, 1.0f}

#include <vector>
#include <cstddef>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/Logger.hpp"
#include "core/EventSystem.hpp"
#include "math/Transform.hpp"
#include "rendering/Camera.hpp"
#include "rendering/ShaderManager.hpp"
#include "rendering/TextureManager.hpp"
#include "util/General.hpp"

struct Vertex : public IPackagable
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 textureCoords;

	static Vertex Register(const glm::vec3& position, const glm::vec3& color, const glm::vec2& textureCoords)
	{
		Vertex out = {};

		out.position = position;
		out.color = color;
		out.textureCoords = textureCoords;

		return out;
	}
};

struct RenderableData : public IPackagable
{
	std::string name = "";
	ShaderObject shader;
	Transform transform = TRANSFORM_DEFAULT;
	std::map<std::string, Texture> textures = {};

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::map<std::string, unsigned int> buffers =
	{
		{"VAO", 0},
		{"VBO", 0},
		{"EBO", 0}
	};
};

struct RenderableObject
{
	RenderableData data;

	void GenerateRawData()
	{
		data.shader.GenerateShader();

		glGenVertexArrays(1, &data.buffers["VAO"]);
		glGenBuffers(1, &data.buffers["VBO"]);
		glGenBuffers(1, &data.buffers["EBO"]);

		glBindVertexArray(data.buffers["VAO"]);

		glBindBuffer(GL_ARRAY_BUFFER, data.buffers["VBO"]);
		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.buffers["EBO"]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		for (auto& [key, value] : data.textures)
			value.GenerateTexture();

		data.shader.Use();
		data.shader.SetUniform("ourTexture", 0);
	}

	void GenerateTestObject()
	{
		data.name = "testObject";

		std::vector<Vertex> vertices
		{
			Vertex::Register({0.0f, 0.0f, 0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}),
			Vertex::Register({0.0f, 0.5f, 0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}),
			Vertex::Register({0.5f, 0.5f, 0.0f}, DEFAULT_COLOR, {1.0f, 1.0f}),
			Vertex::Register({0.5f, 0.0f, 0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}),

			Vertex::Register({0.0f, 0.0f, 0.5f}, DEFAULT_COLOR, {0.0f, 0.0f}),
			Vertex::Register({0.0f, 0.5f, 0.5f}, DEFAULT_COLOR, {1.0f, 0.0f}),
			Vertex::Register({0.5f, 0.5f, 0.5f}, DEFAULT_COLOR, {1.0f, 1.0f}),
			Vertex::Register({0.5f, 0.0f, 0.5f}, DEFAULT_COLOR, {0.0f, 1.0f}),


			Vertex::Register({0.0f, 0.0f, 0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}),
			Vertex::Register({0.0f, 0.5f, 0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}),
			Vertex::Register({0.0f, 0.5f, 0.5f}, DEFAULT_COLOR, {1.0f, 1.0f}),
			Vertex::Register({0.0f, 0.0f, 0.5f}, DEFAULT_COLOR, {0.0f, 1.0f}),

			Vertex::Register({0.5f, 0.0f, 0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}),
			Vertex::Register({0.5f, 0.5f, 0.0f}, DEFAULT_COLOR, {1.0f, 0.0f}),
			Vertex::Register({0.5f, 0.5f, 0.5f}, DEFAULT_COLOR, {1.0f, 1.0f}),
			Vertex::Register({0.5f, 0.0f, 0.5f}, DEFAULT_COLOR, {0.0f, 1.0f}),


			Vertex::Register({0.0f, 0.5f, 0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}),
			Vertex::Register({0.0f, 0.5f, 0.5f}, DEFAULT_COLOR, {1.0f, 0.0f}),
			Vertex::Register({0.5f, 0.5f, 0.5f}, DEFAULT_COLOR, {1.0f, 1.0f}),
			Vertex::Register({0.5f, 0.5f, 0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}),

			Vertex::Register({0.0f, 0.0f, 0.0f}, DEFAULT_COLOR, {0.0f, 0.0f}),
			Vertex::Register({0.0f, 0.0f, 0.5f}, DEFAULT_COLOR, {1.0f, 0.0f}),
			Vertex::Register({0.5f, 0.0f, 0.5f}, DEFAULT_COLOR, {1.0f, 1.0f}),
			Vertex::Register({0.5f, 0.0f, 0.0f}, DEFAULT_COLOR, {0.0f, 1.0f}),
		};

		std::vector<unsigned int> indices
		{
			0, 1, 3,
			1, 2, 3,

			7, 6, 5,
			7, 5, 4,

			11, 10, 9,
			11, 9, 8,

			12, 13, 15,
			13, 14, 15,

			16, 17, 19,
			17, 18, 19,

			23, 22, 21,
			23, 21, 20
		};
		
		data.transform = TRANSFORM_DEFAULT;
		data.shader = ShaderManager::GetShader(ShaderType::DEFAULT);
		RegisterTexture("default_texture", TextureManager::GetTexture("test_texture"));

		ReRegister(vertices, indices);
		GenerateRawData();
	}

	static RenderableObject Register(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const ShaderObject& shader = ShaderManager::GetShader(ShaderType::DEFAULT))
	{
		RenderableObject out = {};

		out.data.name = name;
		out.data.shader = shader;
		out.data.vertices = vertices;
		out.data.indices = indices;

		return out;
	}

	void RegisterTexture(const std::string& name, const Texture& texture)
	{
		data.textures.insert(std::pair<std::string, Texture>(name, texture));
	}

	void ReRegister(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		data.vertices = vertices;
		data.indices = indices;
	}

	void CleanUp()
	{
		glDeleteVertexArrays(1, &data.buffers["VAO"]);
		glDeleteBuffers(1, &data.buffers["VBO"]);
		glDeleteBuffers(1, &data.buffers["EBO"]);

		data.vertices.clear();
		data.indices.clear();
		data.buffers.clear();

		data.shader.CleanUp();
	}
};

namespace Renderer
{
	extern std::vector<RenderableObject> renderableObjects;

	void RegisterRenderableObject(RenderableObject& object)
	{
		int count = 0;

		for (RenderableObject iterationObject : renderableObjects)
		{
			if (iterationObject.data.name == object.data.name)
				renderableObjects[count] = object;

			++count;
		}

		renderableObjects.push_back(object);
	}

	void RenderObjects(Camera camera)
	{
		for (RenderableObject& object : renderableObjects)
		{
			int count = 0;

			for (auto const& [key, value] : object.data.textures)
			{
				glActiveTexture(GL_TEXTURE0 + count);
				glBindTexture(GL_TEXTURE_2D, value.textureID);

				++count;
			}

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, object.data.transform.position);
			//model = glm::rotate(model, object.data.transform.rotation.w, qtov(object.data.transform.rotation));

			object.data.shader.Use();
			object.data.shader.SetUniform("projection", camera.data.matrices.projection);
			object.data.shader.SetUniform("view", camera.data.matrices.view);
			object.data.shader.SetUniform("model", model);
			
			glBindVertexArray(object.data.buffers["VAO"]);
			glDrawElements(GL_TRIANGLES, object.data.indices.size(), GL_UNSIGNED_INT, 0);

			int error = glGetError();

			if (error != GL_NO_ERROR)
				Logger_ThrowError(std::to_string(error), fmt::format("OpenGL error: {}", error), false);
		}
	}

	RenderableObject& GetRenderableObject(const std::string& name)
	{
		for (RenderableObject& object : renderableObjects)
		{
			if (object.data.name == name)
				return object;
		}
	}

	void CleanUpObjects()
	{
		for (RenderableObject& object : renderableObjects)
			object.CleanUp();

		renderableObjects.clear();

		EventSystem::DispatchEvent(EventType::RENDERER_CLEANUP_EVENT, NULL);
	}
}

std::vector<RenderableObject> Renderer::renderableObjects;

#endif // !RENERER_HPP