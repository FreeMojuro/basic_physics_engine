#include "solver.hpp"
#include <raylib.h>

constexpr float spawn_interval = 0.05f;
float spawn_timer = 0.0f;
float x_acc_offset{1};
bool x_acc_offset_direction = true;
bool isgenerating{true};

int main(){
    InitWindow(window_width, window_height, "phy");
    SetTargetFPS(60);

    ObjectArray objects{};

    while(!WindowShouldClose()){
        dt = GetFrameTime();
        spawn_timer += dt;
        if(num_active_objects >= num_objects)isgenerating=false;

        update(objects,dt);

        BeginDrawing();
        ClearBackground(GRAY);

        DrawCircle(area_pos.x, area_pos.y, area_radius, BLACK);
        for(int i{0}; i<num_active_objects;i++){
            DrawCircle(objects.current_position[i].x, objects.current_position[i].y, objects.radius[i] ,objects.color[i]);
        }
        if(isgenerating && spawn_timer >= spawn_interval){
            if(x_acc_offset_direction){
                if(x_acc_offset>=6){
                    x_acc_offset_direction = false;
                }
            }
            else if(!x_acc_offset_direction){
                if(x_acc_offset <= -6){
                    x_acc_offset_direction = true;
                }
            }
            if(x_acc_offset_direction)x_acc_offset+=0.5;
            else{x_acc_offset-=0.5;}

            num_active_objects+=1;
            objects.initial_x_offset[num_active_objects-1] = x_acc_offset;
            objects.previous_position[num_active_objects-1] = {area_pos.x,area_pos.y-300};
            objects.current_position[num_active_objects-1] = {area_pos.x+x_acc_offset,area_pos.y -297};
            objects.acceleration[num_active_objects-1] = {};
            spawn_timer = 0.0f;
            Color rand_color{static_cast<unsigned char>(GetRandomValue(0, 255)),static_cast<unsigned char>(GetRandomValue(0, 255)),static_cast<unsigned char>(GetRandomValue(0, 255)),255};
            objects.color[num_active_objects-1] = rand_color;
            objects.radius[num_active_objects-1] = GetRandomValue(ball_radius-2, ball_radius+2);
           

        }
        
        DrawText(TextFormat("Active Objects:%i", num_active_objects), 20, window_height-40, 25, WHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
