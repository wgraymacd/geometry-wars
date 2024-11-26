#pragma once

#include "Entity.hpp"

using EntityVec = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    std::map<std::string, EntityVec> m_entityMap;
    size_t m_totalEntities = 0;

    void removeDeadEntities(EntityVec& vec)
    {
        // remove all entities from the input vector, this func is called by update function
    }

public:

    EntityManager() = default;

    void update()
    {
        // add entities from m_entiteiesToAdd to the proper locations (first vector of al entities then inside the map with the tag as the key)

        // remove dead entities from the vector of all entities
        removeDeadEntities(m_entities);

        // rmeove dead entities from each vector in the entity map
        // c++20 way or iterating through [key, value] pairs in a map
        for (auto& [tag, entityVec] : m_entityMap)
        {
            removeDeadEntities(entityVec);
        }
    }

    std::shared_ptr<Entity> addEntity(const std::string& tag)
    {
        // create the entity shared pointer
        auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

        // add it to the vec of all entities 
        m_entitiesToAdd.push_back(entity);

        // add it to the entity map
        if (m_entityMap.find(tag) == m_entityMap.end())
        {
            m_entityMap[tag] = EntityVec();
        }
        m_entityMap[tag].push_back(entity);

        return entity;
    }

    const EntityVec& getEntities(const std::string& tag)
    {
        if (m_entityMap.find(tag) == m_entityMap.end())
        {
            m_entityMap[tag] = EntityVec();
        }
        return m_entityMap[tag];
    }

    const std::map<std::string, EntityVec>& getEntityMap()
    {
        return m_entityMap;
    }
};