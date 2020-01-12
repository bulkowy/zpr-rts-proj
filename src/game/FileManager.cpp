#include "FileManager.hpp"

FileManager::FileManager() {
}

void FileManager::init(const char* filename){
	std::ifstream ifs(filename);

	Json::Reader reader;
	Json::Value obj;

	reader.parse(ifs, obj);

	for (auto &&i : obj.getMemberNames())
	{
		this->readSpritesFromJson(obj[i]);
	}

}

void FileManager::readSpritesFromJson(Json::Value& object) {
	sf::Texture* tex;
	for (Json::Value::ArrayIndex i=0; i<object.size(); i++){
		tex = new sf::Texture();
		tex->loadFromFile(object[i]["path"].asString());
		sprites.insert(std::pair<std::string, std::unique_ptr<sf::Texture>>(object[i]["name"].asString(), std::unique_ptr<sf::Texture>(tex)));
	}
}

sf::Sprite* FileManager::getSprite(std::string name) {
	auto texture = sprites.find(name);
	if (texture == sprites.end())
	{
		std::cout << "No sprite of given name: '" << name << "'. Ignoring." << std::endl;
		return nullptr;
	}
	
	return new sf::Sprite(*(texture)->second);
}
