#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "math/AABBCollision.hpp"
#include "math/Raycast.hpp"
#include "rendering/Camera.hpp"

struct PlayerData
{
	Transform transform;
	Camera camera;
	BoundingBox collider;
};

struct Player
{
	PlayerData data;

	void InitalizePlayer(const glm::vec3& position)
	{
		data.transform = TRANSFORM_POSITION(position.x, position.y, position.z);
		data.collider = BoundingBox::Register(position, {0.75f, 1.85, 0.75f});
		data.camera.InitalizeCamera(position);
	}

	void Update()
	{
		data.camera.data.transform.position = data.transform.position;
		data.camera.Update();

		UpdateMouseMovement();
		UpdateBlockEditing();
		UpdateMovement();
	}

	void UpdateBlockEditing()
	{
		if (Input::GetMouseButtonDown(0))
		{
			Raycast ray = Raycast::Register(data.camera);

			glm::ivec3 result = ray.CheckBlock(Settings::ConvertRandom<Chunk>("mainChunk"), 5.0f, 0.5f);

			if (result.x != -1)
				Settings::ConvertRandom<Chunk>("mainChunk").SetBlock(result, "block_air");
		}
	}

	void UpdateMovement()
	{
		float velocity = data.camera.data.movementSpeed * Settings::mainWindow.data.deltaTime;

		if (Input::GetKeyDown(GLFW_KEY_W))
			data.transform.position += data.camera.data.transform.rotation * velocity;

		if (Input::GetKeyDown(GLFW_KEY_A))
			data.transform.position -= data.camera.data.right * velocity;

		if (Input::GetKeyDown(GLFW_KEY_S))
			data.transform.position -= data.camera.data.transform.rotation * velocity;

		if (Input::GetKeyDown(GLFW_KEY_D))
			data.transform.position += data.camera.data.right * velocity;

		data.camera.UpdateCameraVectors();
	}

	void UpdateMouseMovement()
	{
		float xOffset = Input::mousePosition.x - data.camera.data.lastX;
		float yOffset = data.camera.data.lastY - Input::mousePosition.y;
		data.camera.data.lastX = Input::mousePosition.x;
		data.camera.data.lastY = Input::mousePosition.y;

		xOffset *= data.camera.data.mouseSensitivity;
		yOffset *= data.camera.data.mouseSensitivity;

		data.camera.data.yaw += xOffset;
		data.camera.data.pitch += yOffset;

		if (data.camera.data.pitch > 89.0f)
			data.camera.data.pitch = 89.0f;
		if (data.camera.data.pitch < -89.0f)
			data.camera.data.pitch = -89.0f;

		data.camera.UpdateCameraVectors();
	}
};

#endif // !PLAYER_HPP