#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>

#include <jsoncpp/json/json.h>
#include <fstream>
#include <iostream>

/**
 * Klasa zarządzająca odczytem danych (głównie obrazków) z plików.
 */
class FileManager {
	
    /**
     * Mapa sprite'ów - stąd pobierane będą wszystkie obrazki. Wypełniana z plików na początku działania programu.
     * Łączy nazwę obrazka z obiektem SFMLowym reprezentującym teksturę.
     */
	std::map<std::string, std::unique_ptr<sf::Texture>> sprites;
	
public:
	FileManager();
	void init(const char* filename);
	void readSpritesFromJson(Json::Value& object);
	sf::Sprite* getSprite(std::string name);
};

#endif
