#include "entity_manager.h"
#include "player.h"
#include "utility.h"
#include <algorithm>
#include <cstdlib>

static constexpr float kSplitInvincibleDuration = 0.6f;
static constexpr int kBubblesPerPowerUp = 5;

void EntityManager::loadLevel(std::vector<SpawnEntry>& entries)
{
    pending = std::move(entries);
    active.clear();
    bubblesPopped = 0;
}

void EntityManager::update(float delta, float timer, Player& player,
                           Sound& popSound, bool& playerHit, Vector2& dropPowerUpAt)
{
    playerHit = false;
    dropPowerUpAt = { -1000.0f, -1000.0f }; // invalid by default

    for (auto& entry : pending)
    {
        if (timer >= entry.spawnTime && entry.bubble.getActive() && !entry.spawned)
        {
            entry.spawned = true;
            Bubble b = std::move(entry.bubble);
            b.restart();
            active.push_back(std::move(b));
        }
    }

    std::vector<int> toSplit;

    for (int i = 0; i < (int)active.size(); i++)
    {
        Bubble& bubble = active[i];
        if (!bubble.getActive()) continue;

        if (bubble.checkPlayerCollision(player))
        {
            playerHit = true;
            return;
        }

        bool bulletHit = false;
        if (player.isFiring())
        {
            bulletHit = bubble.checkBulletCollision(player);
            if (!bulletHit)
            {
                Vector2 ropeTop = player.getBulletMidPos();
                Vector2 ropeBottom = player.getBulletTraceTail();
                bulletHit = bubble.checkTraceCollision(ropeTop, ropeBottom);
            }
        }

        if (bulletHit)
        {
            // Vary pitch based on bubble size, pan based on position
            float pitch = 0.8f + (bubble.getRadius() / 40.0f) * 0.4f; // smaller = higher pitch
            float pan = (bubble.getPos().x / kVirtualScreenWidth) * 2.0f - 1.0f; // -1 to 1
            SetSoundPitch(popSound, pitch);
            SetSoundPan(popSound, pan);
            PlaySound(popSound);
            
            player.setPoints(bubble.getRadius() * (std::rand() % 100 + 1));
            bubble.stop();
            toSplit.push_back(i);

            bubblesPopped++;
            if (bubblesPopped % kBubblesPerPowerUp == 0)
                dropPowerUpAt = bubble.getPos();

            continue;
        }

        bubble.update(delta);
    }

    for (int idx : toSplit)
        spawnSplitBubbles(active[idx]);

    active.erase(
        std::remove_if(active.begin(), active.end(),
            [](const Bubble& b) { return !b.getActive(); }),
        active.end());
}

void EntityManager::spawnSplitBubbles(const Bubble& parent)
{
    int childRadius = parent.getRadius() / 2;
    if (childRadius < 8) return;

    Vector2 parentPos = parent.getPos();
    float childSpeed = parent.getBounceSpeed() * 1.2f;

    Bubble left(parentPos, childRadius, false, childSpeed);
    left.load();
    left.setInvincible(kSplitInvincibleDuration);
    left.setInitialBounce(-180.0f);
    active.push_back(std::move(left));

    Bubble right(parentPos, childRadius, true, childSpeed);
    right.load();
    right.setInvincible(kSplitInvincibleDuration);
    right.setInitialBounce(-180.0f);
    active.push_back(std::move(right));
}

void EntityManager::restart()
{
    active.clear();
    bubblesPopped = 0;
    for (auto& entry : pending)
    {
        entry.spawned = false;
        entry.bubble.restart();
    }
}

bool EntityManager::allInactive() const
{
    for (const auto& e : pending)
        if (!e.spawned) return false;
    return active.empty();
}
