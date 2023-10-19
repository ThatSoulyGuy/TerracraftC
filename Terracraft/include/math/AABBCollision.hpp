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

        out.min = position;
        out.max = size;

        return out;
    }
};

#endif // !AABB_COLLISION_HPP