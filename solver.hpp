#pragma once

#include <cmath>
#include <raylib.h>
#include <array>

constexpr int window_width{1400};
constexpr int window_height{1000};
constexpr Vector2 area_pos{700,500};
constexpr float area_radius{400};
constexpr float ball_radius{8};
inline float dt{};
inline int substeps{4};
inline int num_active_objects{0};
constexpr int num_objects{2000};


inline Vector2 operator-(const Vector2& v1, const Vector2& v2){
    return Vector2{v1.x-v2.x,v1.y-v2.y};
}
inline Vector2 operator+(const Vector2& v1, const Vector2& v2){
    return Vector2{v1.x+v2.x,v1.y+v2.y};
}
inline Vector2 operator*(const Vector2& v1, float s){
    return Vector2{v1.x*s,v1.y*s};
}
inline Vector2 operator/(const Vector2& v1, float s){
    return Vector2{v1.x/s,v1.y/s};
}
inline bool operator==(const Vector2& v1, const Vector2& v2){
    if(v1.x == v2.x && v1.y == v2.y)return true;
    else return false;
}

struct ObjectArray{
    std::array<Vector2, num_objects> previous_position;
    std::array<Vector2, num_objects> current_position;
    std::array<Vector2, num_objects> acceleration;
    std::array<Color, num_objects> color;
    std::array<int,num_objects> radius;
    std::array<float,num_objects> initial_x_offset;

};

inline void accelarate(ObjectArray& objects,const Vector2& acc){
    for(int i{0}; i<num_active_objects;++i){
        objects.acceleration[i] = objects.acceleration[i] + acc;
    }
}

inline void integrate_position(ObjectArray& objects, float dt){
    for(int i{0}; i<num_active_objects;++i){
        Vector2 velocity = (objects.current_position[i] - objects.previous_position[i]);
        objects.previous_position[i] = objects.current_position[i];
        objects.current_position[i] = objects.current_position[i] + velocity+ objects.acceleration[i] * dt * dt;
        objects.acceleration[i] = {};
    }

}


inline void applygravity(ObjectArray& objects){
    Vector2 gravity{0,1000};
    accelarate(objects, gravity);
}

inline void applyconstraints(ObjectArray& objects) {

    for (int i = 0; i < num_active_objects; ++i) {
        const float max_dist = area_radius - objects.radius[i];
        const Vector2 offset = objects.current_position[i] - area_pos;
        const float dist = std::sqrt(offset.x * offset.x + offset.y * offset.y);

        if (dist > max_dist && dist > 0.0f) {
            const Vector2 correction = offset / dist * (dist - max_dist);
            objects.current_position[i] = objects.current_position[i] - correction;
            objects.initial_x_offset[i] = 0;

        }
    }
}

inline void solvecollisions(ObjectArray& objects){
    for(int i{0}; i < num_active_objects; ++i){
        for(int j{i+1}; j < num_active_objects; ++j){
            const Vector2 collision_axis = objects.current_position[i] - objects.current_position[j];
            const float dist = std::sqrt(collision_axis.x * collision_axis.x + collision_axis.y * collision_axis.y);
            if(dist < objects.radius[i]+objects.radius[j] && dist>0.0f){
                const Vector2 n = collision_axis / dist;
                const float delta = objects.radius[i]+objects.radius[j] - dist;
                objects.current_position[i] = objects.current_position[i] + n * delta * 0.5f;
                objects.current_position[j] = objects.current_position[j] - n * delta * 0.5f;
            }



        }
    }

}

inline void update(ObjectArray& objects, float dt){
    const float sub_dt = dt/float(substeps);
    for(int i{0};i<substeps; i++){
        applygravity(objects);
        integrate_position(objects, sub_dt);
        applyconstraints(objects);
        solvecollisions(objects);
        
    }
}
