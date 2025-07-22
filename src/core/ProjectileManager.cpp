#include "ProjectileManager.h"
#include "Macros.h"
#include <algorithm>

ProjectileManager &ProjectileManager::Instance()
{
    static ProjectileManager instance;
    return instance;
}

void ProjectileManager::Init(std::shared_ptr<const Settings> settings)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;

    if (!m_isInitialized)
    {
        m_projectiles.clear();
        m_isInitialized = true;
    }

    CT_LOG_INFO("ProjectileManager initialized.");
}

void ProjectileManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("ProjectileManager", "Shutdown");

    if (m_isInitialized)
    {
        m_projectiles.clear();
        m_isInitialized = false;
    }

    CT_LOG_INFO("ProjectileManager shutdown.");
}

bool ProjectileManager::IsInitialized() const
{
    return m_isInitialized;
}

void ProjectileManager::AddProjectile(const std::shared_ptr<BaseProjectile> &projectile)
{
    if (projectile)
    {
        m_projectiles.push_back(projectile);
    }
}

void ProjectileManager::RemoveProjectile(const std::shared_ptr<BaseProjectile> &projectile)
{
    m_projectiles.erase(std::remove(m_projectiles.begin(), m_projectiles.end(), projectile), m_projectiles.end());
}

void ProjectileManager::ClearAllProjectiles()
{
    m_projectiles.clear();
}

std::vector<std::shared_ptr<BaseProjectile>> ProjectileManager::GetProjectiles() const
{
    return m_projectiles;
}

void ProjectileManager::Update(float dt)
{
    for (auto &p : m_projectiles)
    {
        if (p && p->IsAlive())
        {
            p->Update(dt);
        }
    }

    // Remove dead or null projectiles
    for (auto it = m_projectiles.begin(); it != m_projectiles.end();)
    {
        if (!(*it) || !(*it)->IsAlive())
        {
            it = m_projectiles.erase(it);
        }

        else
        {
            ++it;
        }
    }
}

void ProjectileManager::Draw(sf::RenderTarget &target)
{
    for (const auto &p : m_projectiles)
    {
        if (p && p->IsAlive())
        {
            p->Draw(target);
        }
    }
}
