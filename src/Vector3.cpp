#include "Vector3.h"

double Vec3::dot(Vec3 other)
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vec3 Vec3::cross(Vec3 other)
{
    double res_x = this->y * other.z - this->z * other.y;
    double res_y = this->z * other.x - this->x * other.z;
    double res_z = this->x * other.y - this->y * other.x;

    return Vec3(res_x, res_y, res_z);
}

double Vec3::mag()
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::normalized()
{
    double mag = this->mag();
    if (mag != 0) 
    {
        return Vec3(this->x / mag, this->y / mag, this->z / mag);
    }
    
    // mag == 0
    return Vec3();
}
