#include "Engine.hpp"

namespace ecs {

Engine::Engine() :
lastEntity_(_invalidEntity),
entities_(),
componentStores_(),
systems_() {
}

Engine::~Engine() {
}

// Dodaj System.
void Engine::addSystem(const System::Ptr& aSystemPtr) {
    systems_.push_back(aSystemPtr);
}

// Zarejestruj Obiekt do wszystkich Systemów.
size_t Engine::registerEntity(const Entity aEntity) {
    size_t affectedSystemsCount = 0;

    auto entity = entities_.find(aEntity);
    if (entities_.end() == entity) {
        throw std::runtime_error("The Entity does not exist");
    }
    auto entityComponents = (*entity).second;

    // Sprawdź, które Systemy będą zainteresowane tym Obiektem
    for (auto system  = systems_.begin();
              system != systems_.end();
            ++system) {
        auto systemRequiredComponents = (*system)->getRequiredComponents();
        for (std::size_t family = 0;
                family < systemRequiredComponents.size();
                family++)
        {
            // Sprawdź czy Obiekt zawiera wszystkie wymagane Komponenty
            if (std::includes(entityComponents.begin(), entityComponents.end(),
                            systemRequiredComponents[family].begin(), systemRequiredComponents[family].end())) {
                // Zarejestruj Obiekt
                (*system)->registerEntity(aEntity, family);
                ++affectedSystemsCount;
            }
        }            
        
    }

    return affectedSystemsCount;
}

// Wyrejestruj Obiekt ze wszystkich Systemów.
void Engine::unregisterEntity(const Entity aEntity) {
    auto entity = entities_.find(aEntity);
    if (entities_.end() == entity) {
        throw std::runtime_error("The Entity does not exist");
    }

    for (auto system  = systems_.begin();
              system != systems_.end();
            ++system) {
        (*system)->unregisterEntity(aEntity);
    }

    return;
}

// Wykonaj obliczenia dla wszystkich Obiektów wszystkich Systemów.
void Engine::update(unsigned int frameTime) {

    auto frameStart = std::chrono::system_clock::now();

    for (auto system  = systems_.begin();
              system != systems_.end();
            ++system) {
        (*system)->update(frameTime);
    }

    std::this_thread::sleep_until(frameStart + std::chrono::milliseconds(frameTime));

    return;
}

std::set<Entity> Engine::getEntitySet() {
    std::set<Entity> ret;
    for (auto &&entPair : entities_)
    {
        ret.insert(entPair.first);
    }
    return ret;
}

}