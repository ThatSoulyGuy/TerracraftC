#ifndef AABB_COLLISION_HPP
#define AABB_COLLISION_HPP

#include <glm/glm.hpp>

struct BoundingBox 
{
    glm::vec3 min;
    glm::vec3 max;

    bool IsCollidingWith(const BoundingBox& other) const 
    {
        return 
            (min.x <= other.max.x && max.x >= other.min.x) &&
            (min.y <= other.max.y && max.y >= other.min.y) &&
            (min.z <= other.max.z && max.z >= other.min.z);
    }

    static BoundingBox Register(const glm::vec3& position, const glm::vec3& size)
    {
        BoundingBox out = {};
        out.Update(position, size);
        return out;
    }

    void Update(const glm::vec3 & position)
    {
        auto size = max - min;
        Update(position, size);
    }

    void Update(const glm::vec3 & position, const glm::vec3 & size)
    {
        min = position;
        max = position + size;
    }
};

#endif // !AABB_COLLISION_HPP