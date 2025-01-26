#include <iostream>
#include <vector>
#include <cmath>

struct Vector2
{
    float x;
    float y;

    Vector2 (float x=0, float y=0) : x(x),y(y) {}

    float distance(const Vector2& other) const
    {
        return std::sqrt((x-other.x)*(x-other.x) + (y-other.y)*(y-other.y));
    }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x+other.x, y+other.y);
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x-other.x, y-other.y);
    }

    Vector2 operator *(float scalar) const
    {
        return Vector2(x*scalar , y*scalar);
    }
};

class Boid
{
    public:

    Vector2 position;
    Vector2 velocity;

    Boid (float x , float y) : position(x,y) , velocity(0,0) {}

    void updatePosition(const std::vector<Boid>& boids, float separationDistance, float alignmentDistance, float cohesionDistance)
    {
        Vector2 separationForce = separation(boids , separationDistance);
        Vector2 alignmentForce = alignment(boids, alignmentDistance);
        Vector2 cohesionForce = cohesion(boids, cohesionDistance);

        velocity = velocity + separationForce + alignmentForce + cohesionForce;
        position = position + velocity;
    }

    Vector2 separation(const std::vector<Boid>& boids, float desiredSeparation);
    Vector2 alignment(const std::vector<Boid>& boids, float neighborDistance);
    Vector2 cohesion(const std::vector<Boid>& boids, float neighborDistance);
};

Vector2 Boid::separation(const std::vector<Boid>& boids, float desiredSeparation)
{
    Vector2 steer(0,0); //yönünü değiştirmek için Boid
    int count_boids = 0;

    for(const auto& other : boids)
    {
        float d = position.distance(other.position);
        if ( d>0 && d < desiredSeparation)
        {
            Vector2 diff = position - other.position;
            diff = diff * (1.0f / d);
            steer = steer + diff;
            count_boids++;
        }
    }

    if ( count_boids > 0)
    {
        steer = steer * (1.0f / count_boids);
    }

    return steer;
}

Vector2 Boid::alignment(const std::vector<Boid>& boids, float neighborDistance)
{
    Vector2 avgVelocity(0,0); //ortalama hız vektörü
    int count = 0;

    for(const auto& other : boids)
    {
        float d = position.distance(other.position); // uzaklığını ölçer iki pos'un.
        if( d > 0 && d < neighborDistance)
        {
            avgVelocity  =avgVelocity+ other.velocity;
            count++;
        }
    }

    if ( count > 0)
    {
        avgVelocity = avgVelocity * (1.0f / count); // ortalama hız bulunur.
        avgVelocity = avgVelocity - velocity; // mevcut hız farkını hesapla..
    }

    return avgVelocity * 0.05f;

}

Vector2 Boid::cohesion(const std::vector<Boid>& boids, float neighborDistance)
{
    Vector2 centerOfMass(0,0); // komşuların ortalama konumu
    int count = 0 ;

    for(const auto& other : boids)
    {
        float d = position.distance(other.position);
        if(d > 0 && d< neighborDistance)
        {
            centerOfMass = centerOfMass + other.position; // komşuların toplam pozisyonu
            count++;
        }
    }

        if (count > 0)
        {
            centerOfMass = centerOfMass * (1.0f / count);
            Vector2 cohesionVector = centerOfMass - position; // boid'in mevcut pos'a göre yön belirle
            return cohesionVector * 0.05f ; //hareketin hızını kontrol etmek.

        }

        return Vector2(0,0);
        
    }
    


int main()
{
    std::vector<Boid> flock;
    flock.push_back(Boid(10,20));
    flock.push_back(Boid(15,25));
    flock.push_back(Boid(30,40));

    flock[0].velocity = Vector2(1, -1);
    flock[1].velocity = Vector2(-1, 1);
    flock[2].velocity = Vector2(0, 0);

    for(int k = 0; k<=10; k++)
    {

    
    for(Boid& boid : flock)
    {
        boid.updatePosition(flock, 10.0f, 20.0f, 30.0f);
    }

    for ( size_t i=0; i< flock.size(); i++)
    {
        std::cout << "Boid" << i << " position: (" <<flock[i].position.x
        << ", "<<flock[i].position.y << " )\n";
    }

    }
    

    return 0;
}