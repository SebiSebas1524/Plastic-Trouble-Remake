#pragma once
#include "bubble.h"
#include <vector>

class Player;

class EntityManager
{
public:
    struct SpawnEntry
    {
        float spawnTime = 0.0f;
        Bubble bubble;
        bool spawned = false;

        SpawnEntry() = default;
        SpawnEntry(float t, Bubble b)
            : spawnTime(t), bubble(std::move(b)), spawned(false) {}
        SpawnEntry(SpawnEntry&&) = default;
        SpawnEntry& operator=(SpawnEntry&&) = default;
        SpawnEntry(const SpawnEntry&) = delete;
        SpawnEntry& operator=(const SpawnEntry&) = delete;
    };

    void loadLevel(std::vector<SpawnEntry> entries);
    void update(float delta, float timer, Player& player,
                Sound& popSound, bool& playerHit, Vector2& dropPowerUpAt);

    void restart();
    bool allInactive() const;

private:
    void spawnSplitBubbles(const Bubble& parent);

    std::vector<SpawnEntry> pending;
    std::vector<Bubble> active;

    int bubblesPopped = 0; // track pops for power-up drops
};
