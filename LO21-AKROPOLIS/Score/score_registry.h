#pragma once

#include <map>
#include <memory>
#include <functional>
#include "score.h"

class ScoreRegistry {
public:
    // Signature de la fonction qui crée un objet de score
    using ScoreCreator = std::function<std::unique_ptr<Score>(const ActiveVariants&)>;

    // Singleton pour accéder au registre partout dans le projet
    static ScoreRegistry& instance() {
        static ScoreRegistry instance;
        return instance;
    }

    //enregistre une fonction créatrice pour un mode donné
    void registerScore(ModeScore mode, ScoreCreator creator) {
        creators_[mode] = std::move(creator);
    }

    // instance du calculateur de score correspondant au mode
    std::unique_ptr<Score> create(ModeScore mode, const ActiveVariants& variants) const {
        auto it = creators_.find(mode);
        if (it != creators_.end()) {
            return it->second(variants);
        }
        return nullptr; // comportement par défaut
    }

private:
    ScoreRegistry() = default;
    std::map<ModeScore, ScoreCreator> creators_;

    // Empêcher la copie
    ScoreRegistry(const ScoreRegistry&) = delete;
    ScoreRegistry& operator=(const ScoreRegistry&) = delete;
};