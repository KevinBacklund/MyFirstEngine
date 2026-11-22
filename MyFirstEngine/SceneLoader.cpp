#include "SceneLoader.h"

std::unordered_map <std::string, SceneLoader::Scene> SceneLoader::LoadScenes()
{
	std::unordered_map <std::string, SceneLoader::Scene> result;
    for (const auto& entry : std::filesystem::directory_iterator("../Assets/scenes/"))
    {
        Scene scene = LoadScene(entry.path().string());
        result[scene.sceneName] = scene;
        std::cout << "Loaded Scene: " << scene.sceneName << std::endl;
    }
    return result;
}

SceneLoader::Scene SceneLoader::LoadScene(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string line;
    Scene scene;

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filepath << "\n";
        return scene;
    }

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string prefix;
        if (!(ss >> prefix)) continue;

        if (prefix == "sceneName") {
            file >> scene.sceneName;
        }
        else if (prefix == "sceneObject")
        {
            std::ifstream file(filepath);
            if (!file.is_open()) {
                std::cerr << "Failed to open scene file: " << filepath << "\n";
                return scene;
            }

            std::string line;
            Object* currentObj = nullptr;

            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string keyword;

                if (!(iss >> keyword)) {
                    continue;
                }

                if (keyword == "sceneName") {
                    iss >> scene.sceneName;
                }
                else if (keyword == "sceneObject") {
                    scene.sceneObjects.emplace_back();
                    currentObj = &scene.sceneObjects.back();
                }
                else if (keyword == "model" && currentObj) {
                    iss >> currentObj->meshName;
                }
                else if (keyword == "position" && currentObj) {
                    iss >> currentObj->position.x >> currentObj->position.y >> currentObj->position.z;
                }
                else if (keyword == "scale" && currentObj) {
                    iss >> currentObj->scale.x >> currentObj->scale.y >> currentObj->scale.z;
                }
                else if (keyword == "rotation" && currentObj) {
                    iss >> currentObj->rotation.x >> currentObj->rotation.y >> currentObj->rotation.z;
                }
                else if (keyword == "playerStart") {
                    glm::vec3 start;
                    iss >> start.x >> start.y >> start.z;
                    scene.playerStarts.push_back(start);
                }
                else if (keyword == "goal") {
                    iss >> scene.goalLocation.x >> scene.goalLocation.y >> scene.goalLocation.z;
                }
            }

            return scene;
        }
    }
}