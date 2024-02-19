#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "lab_m1/tema2/tema_camera.h"


namespace m1
{
    class building
    {
    public:
        building();
        building(glm::vec3 building_center, float ox_side, float oy_size, float oz_side);
        ~building();

        glm::vec3 building_center;
        float ox_side, oy_side, oz_side;

        glm::vec3 corner1, corner2, corner3, corner4;
    };

    class bullet
    {

    public:
        bullet();
        bullet(glm::vec3 current_bullet_position, glm::vec3 bullet_direction, bool my_bullet);
        ~bullet();

        glm::vec3 current_bullet_position;
        glm::vec3 bullet_direction;

 

        bool my_bullet;
        bool used_bullet;
        float traveled_distance;
    };

    class tank
    {

    public:
        tank();
        tank(glm::vec3 current_position, float angle, bool my_tank);
        ~tank();


        glm::vec3 current_position;
        glm::vec3 camera_position;
        glm::vec3 current_gun_position;
        glm::vec3 left_track_position, right_track_position;

        glm::vec3 rear_tank_vertex;

        std::vector<VertexFormat> GetTankHullVertices();
        std::vector<unsigned int> GetTankHullIndices();

        std::vector<VertexFormat> GetTankTrackVertices();
        std::vector<unsigned int> GetTankTrackIndices();

        std::vector<VertexFormat> GetTankTurretVertices();
        std::vector<unsigned int> GetTankTurretIndices();

        std::vector<VertexFormat> GetTankGunVertices();
        std::vector<unsigned int> GetTankGunIndices();

        glm::vec3 GetCurrentPosition();
        glm::vec3 GetCameraPosition();
        glm::vec3 GetGunBasePosition();
        glm::vec3 GetLeftTrackPosition();
        glm::vec3 GetRightTrackPosition();

        float rotation;
        float turret_rotation;
        float reloading_time_passed;
        int health;
        int score;
        bool my_tank;
        float deformationStrength;

        glm::vec3 tank_direction;
        glm::vec3 gun_direction;
        glm::vec3 gun_barrel_tip;
        glm::vec3 shooting_direction;

        glm::vec3 hullColor;
        glm::vec3 turretColor;

        std::vector<std::pair<int, float>> enemy_tank_movement;
        std::pair<int, float> current_movement;
        float time_spent_in_current_movement;

        void UpdateTankStates(float deltaTimeSeconds);

        void RotateTank(float angle);
        void RotateGun(float angle);
        void ShootBullet(std::vector<bullet> *bullets);
       
    };


    class tema2 : public gfxc::SimpleScene
    {
     public:
         tema2();
        ~tema2();

        void Init() override;

        Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, tank* specific_tank);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;


        void ManageBullets(std::vector<building> *buildings, std::vector<bullet> *bullets, std::vector<tank> *tanks, float deltaTimeSeconds);
        void ChangeBulletsPosition(std::vector<bullet>* bullets, float deltaTimeSeconds);
        void DrawBullets(std::vector<bullet> *bullets);
        void CheckBullet_TankColision(std::vector<bullet>* bullets, std::vector<tank>* tanks);
        void CheckTank_TankColision(std::vector<tank>* tanks);
        void CheckTank_BuildingColision(std::vector<building>* buildings, std::vector<tank>* tanks);
        void CheckBullet_BuildingColision(std::vector<building>* buildings, std::vector<bullet>* bullet);
        
        void ManageTanks(std::vector<tank> *tanks, float deltaTimeSeconds);
        void DrawTanks(std::vector<tank> *tanks, float deltaTimeSeconds);
        void CheckToEngage(std::vector<tank>* tanks, float deltaTimeSeconds);

        void IncrementEnemyReloadTime(std::vector<tank>* tanks, float deltaTimeSeconds);
        void MoveEnemyTanks(std::vector<tank>* tanks, float deltaTimeSeconds);

        void ManageBuildings(std::vector<building>* buildings);
        void DrawBuildings(std::vector<building>* buildings);

        void GenerateRandomPositionedBuildings();
        void GenerateRandomPositionedEnemyTanks();

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;

        tank my_tank, enemy_tank1, enemy_tank2, enemy_tank3, enemy_tank4,
                enemy_tank5, enemy_tank6, enemy_tank7, enemy_tank8;
        building building1, building2, building3, building4, building5, building6;

        std::vector<tank> tanks;
        std::vector<bullet> bullets;
        std::vector<building> buildings;

        float game_time_passed;
        bool game_over;
    };
}   // namespace m1
