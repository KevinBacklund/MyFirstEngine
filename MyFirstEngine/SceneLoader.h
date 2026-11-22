#pragma once
#include <unordered_map>
#include <glm.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

class SceneLoader
{
public:
	struct Object
	{
		std::string meshName;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
	};
	struct Scene
	{
		std::string sceneName;
		std::vector<Object> sceneObjects;
		glm::vec3 goalLocation;
		std::vector<glm::vec3> playerStarts;
	};
	std::unordered_map <std::string, Scene> LoadScenes();

private:
	Scene LoadScene(const std::string& filepath);
};

