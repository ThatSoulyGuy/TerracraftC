#include "core/Logger.hpp"
#include "core/EventSystem.hpp"
#include "core/Settings.hpp"
#include "core/Window.hpp"
#include "gameplay/Player.hpp"
#include "rendering/Renderer.hpp"
#include "rendering/TextureManager.hpp"
#include "world/BlockType.hpp"
#include "world/Chunk.hpp"
#include "world/TextureAtlasManager.hpp"

Player player;
Window window;
Chunk chunk;

int main()
{
	Logger_Init();

	Settings::InitGLFW();
	ShaderManager::RegisterShader(ShaderObject::Register("shaders/default", ShaderType::DEFAULT));
	TextureManager::RegisterTexture(Texture::Register("textures/test_image.png", "test_texture"));
	TextureManager::RegisterTexture(Texture::Register("textures/terrain.png", "terrain_atlas"));

	BlockType::Initalize();
	TextureAtlasManager::Initalize();

	window.GenerateWindow("Terracraft* 0.1.4", glm::ivec2{ 750, 450 }, glm::vec3{ 0.0f, 0.34f, 0.51f });
	Settings::mainWindow = window;
	EventSystem::DispatchEvent(EventType::TC_PRE_INIT_EVENT, NULL);
	
	Input::InitInput(window.data.window);

	chunk.GenerateChunk();

	player.InitalizePlayer(glm::vec3{ 6.0f, 24.0f, 6.0f });

	Settings::randomData.insert({ "mainChunk", &chunk });
	EventSystem::DispatchEvent(EventType::TC_INIT_EVENT, NULL);

	Logger_WriteConsole("Hello, World!", LogLevel::INFO);

	while (!window.ShouldClose())
	{
		window.UpdateColors();

		player.Update();
		Renderer::RenderObjects(player.data.camera);

		window.UpdateBuffers();

		Settings::mainWindow = window;
	}

	window.CleanUp();
	Renderer::CleanUpObjects();

	EventSystem::DispatchEvent(EventType::TC_CLEANUP_EVENT, NULL);

	Logger_CleanUp();

	return 0;
}