#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


tema2::tema2()
{
}


tema2::~tema2()
{
}

#define GAME_PLANE_SIZE 100
#define GRASS_COLOR glm::vec3(0.68, 1, 0.68)
#define SKY_COLOR glm::vec3(0.53, 0.81, 0.94)

#define FORWARD_TANK_SPEED 4
#define BACKWARDS_TANK_SPEED 2.5
#define DIRECTION_CHANGE_SPEED 2


void tema2::Init()
{
    game_time_passed = 0;
    game_over = false;

    my_tank = tank(glm::vec3(0, 0.45, 5), 0, true);


    GenerateRandomPositionedBuildings();
    GenerateRandomPositionedEnemyTanks();

    camera = new implemented::Camera();
    camera->Set(my_tank.GetCameraPosition(), my_tank.GetCurrentPosition(), glm::vec3(0, 1, 0));
    

    // Create tank
    {
        vector<VertexFormat> verticesTankHull = my_tank.GetTankHullVertices();
        vector<unsigned int> indicesTankHull = my_tank.GetTankHullIndices();
        CreateMesh("tankHull", verticesTankHull, indicesTankHull);

        vector<VertexFormat> verticesTankTrack = my_tank.GetTankTrackVertices();
        vector<unsigned int> indicesTankTrack = my_tank.GetTankTrackIndices();
        CreateMesh("tankTrack", verticesTankTrack, indicesTankTrack);

        vector<VertexFormat> verticesTankTurret = my_tank.GetTankTurretVertices();
        vector<unsigned int> indicesTankTurret = my_tank.GetTankTurretIndices();
        CreateMesh("tankTurret", verticesTankTurret, indicesTankTurret);

        vector<VertexFormat> verticesTankGun = my_tank.GetTankGunVertices();
        vector<unsigned int> indicesTankGun = my_tank.GetTankGunIndices();
        CreateMesh("tankGun", verticesTankGun, indicesTankGun);
    }

    {
        vector<VertexFormat> verticesTankHull = enemy_tank1.GetTankHullVertices();
        vector<unsigned int> indicesTankHull = enemy_tank1.GetTankHullIndices();
        CreateMesh("enemyTankHull", verticesTankHull, indicesTankHull);

        vector<VertexFormat> verticesTankTrack = enemy_tank1.GetTankTrackVertices();
        vector<unsigned int> indicesTankTrack = enemy_tank1.GetTankTrackIndices();
        CreateMesh("enemyTankTrack", verticesTankTrack, indicesTankTrack);

        vector<VertexFormat> verticesTankTurret = enemy_tank1.GetTankTurretVertices();
        vector<unsigned int> indicesTankTurret = enemy_tank1.GetTankTurretIndices();
        CreateMesh("enemyTankTurret", verticesTankTurret, indicesTankTurret);

        vector<VertexFormat> verticesTankGun = enemy_tank1.GetTankGunVertices();
        vector<unsigned int> indicesTankGun = enemy_tank1.GetTankGunIndices();
        CreateMesh("enemyTankGun", verticesTankGun, indicesTankGun);
    }


    // Import sphere (bullet)
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple cube
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3( 1, -1,  1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3( 1,  1,  1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.7, 0.3, 0.7)),
            VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.3, 0.5, 0.4)),
            VertexFormat(glm::vec3( 1, -1, -1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.5, 0.2, 0.9)),
            VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.7, 0.0, 0.7)),
            VertexFormat(glm::vec3( 1,  1, -1), glm::vec3(0.8, 0.8, 0.8), glm::vec3(0.1, 0.5, 0.8)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh("cube", vertices, indices);
    }

    // Create the grass
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(GAME_PLANE_SIZE, 0, GAME_PLANE_SIZE), GRASS_COLOR, GRASS_COLOR),
            VertexFormat(glm::vec3(-GAME_PLANE_SIZE, 0, GAME_PLANE_SIZE), GRASS_COLOR, GRASS_COLOR),
            VertexFormat(glm::vec3(GAME_PLANE_SIZE, 0, -GAME_PLANE_SIZE), GRASS_COLOR, GRASS_COLOR),
            VertexFormat(glm::vec3(-GAME_PLANE_SIZE, 0, -GAME_PLANE_SIZE), GRASS_COLOR, GRASS_COLOR)
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            3, 2, 1
        };

        CreateMesh("grass", vertices, indices);
    }

    // Create sky
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-100, 0,  100), SKY_COLOR, glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(100, 0,  100), SKY_COLOR, glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-100,  100,  100), SKY_COLOR, glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3(100,  100,  100), SKY_COLOR, glm::vec3(0.7, 0.3, 0.7)),
            VertexFormat(glm::vec3(-100, 0, -100), SKY_COLOR, glm::vec3(0.3, 0.5, 0.4)),
            VertexFormat(glm::vec3(100, 0, -100), SKY_COLOR, glm::vec3(0.5, 0.2, 0.9)),
            VertexFormat(glm::vec3(-100,  100, -100), SKY_COLOR, glm::vec3(0.7, 0.0, 0.7)),
            VertexFormat(glm::vec3(100,  100, -100), SKY_COLOR, glm::vec3(0.1, 0.5, 0.8)),


        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh("sky", vertices, indices);
    }



    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shadersT2", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shadersT2", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projectionMatrix = glm::perspective(RADIANS(70), window->props.aspectRatio, 0.01f, 200.0f);
}


Mesh* tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================


    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema2::Update(float deltaTimeSeconds)
{   
    game_time_passed += deltaTimeSeconds;

    if (game_time_passed > 180) {
        cout << "Game time ran out! :(" << endl;
        cout << "Score: " << my_tank.score << endl;
        game_over = true;
    }

    if (my_tank.health <= 0) {
        cout << "Your tank has been destroyed!" << endl;
        my_tank.score = 0;
        exit(0);
    }

    if (!game_over) {
        my_tank.UpdateTankStates(deltaTimeSeconds);
    }

    ManageBullets(&buildings, &bullets, &tanks, deltaTimeSeconds);
    ManageTanks(&tanks, deltaTimeSeconds);
    ManageBuildings(&buildings);

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, my_tank.GetCurrentPosition());
        modelMatrix = glm::rotate(modelMatrix, my_tank.rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
        RenderSimpleMesh(meshes["tankHull"], shaders["LabShader"], modelMatrix, &my_tank);
        
    }


    {
        glm::mat4 modelMatrix = glm::mat4(1);
        
        modelMatrix = glm::translate(modelMatrix, my_tank.GetCurrentPosition());
        modelMatrix = glm::rotate(modelMatrix, my_tank.rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, my_tank.GetLeftTrackPosition());
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.08, 1.18));
        RenderSimpleMesh(meshes["tankTrack"], shaders["LabShader"], modelMatrix, &my_tank);

        modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, my_tank.GetCurrentPosition());
        modelMatrix = glm::rotate(modelMatrix, my_tank.rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, my_tank.GetRightTrackPosition());

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.08, 1.18));
        RenderSimpleMesh(meshes["tankTrack"], shaders["LabShader"], modelMatrix, &my_tank);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        
        modelMatrix = glm::translate(modelMatrix, my_tank.GetCurrentPosition());
        modelMatrix = glm::rotate(modelMatrix, my_tank.turret_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, my_tank.rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
        
        RenderSimpleMesh(meshes["tankTurret"], shaders["LabShader"], modelMatrix, &my_tank);
    }

    {
        float stabilisation = 4.71238;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, my_tank.GetGunBasePosition());
        modelMatrix = glm::rotate(modelMatrix, my_tank.turret_rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, my_tank.rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, stabilisation, glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["tankGun"], shaders["LabShader"], modelMatrix, &my_tank);

    }


    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["grass"], shaders["LabShader"], modelMatrix, &my_tank);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["sky"], shaders["LabShader"], modelMatrix, &my_tank);
    }
}


void tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, tank *specfic_tank)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int location1 = glGetUniformLocation(shader->GetProgramID(), "Model");

    glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int location2 = glGetUniformLocation(shader->GetProgramID(), "View");

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int location3 = glGetUniformLocation(shader->GetProgramID(), "Projection");

    // Trimitere date integritate tank

    if (mesh == meshes["enemyTankHull"] || mesh == meshes["enemyTankTrack"] || mesh == meshes["enemyTankTurret"]) {
        int deformationStrength_location = glGetUniformLocation(shader->GetProgramID(), "deformationStrength");
        glUniform1f(deformationStrength_location, specfic_tank->deformationStrength);
    }
    else {
        int deformationStrength_location = glGetUniformLocation(shader->GetProgramID(), "deformationStrength");
        glUniform1f(deformationStrength_location, 0);
    }

    glUniformMatrix4fv(location3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));



    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Add key press event
    if (window->KeyHold(GLFW_KEY_W) && !game_over) {
        float distance = deltaTime * FORWARD_TANK_SPEED;
        my_tank.current_position += my_tank.tank_direction * distance;
        my_tank.current_gun_position += my_tank.tank_direction * distance;
        my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;

        camera->MoveForward(deltaTime * FORWARD_TANK_SPEED, my_tank.tank_direction);
        
    }
    else if (window->KeyHold(GLFW_KEY_A) && !game_over) {
        my_tank.rotation += deltaTime * DIRECTION_CHANGE_SPEED;
        my_tank.RotateTank(deltaTime * DIRECTION_CHANGE_SPEED);
        my_tank.RotateGun(deltaTime * DIRECTION_CHANGE_SPEED);
        camera->RotateThirdPerson_OY(deltaTime * DIRECTION_CHANGE_SPEED, my_tank.GetCurrentPosition());

    }
    else if (window->KeyHold(GLFW_KEY_D) && !game_over) {
        my_tank.rotation -= deltaTime * DIRECTION_CHANGE_SPEED;
        my_tank.RotateTank(-deltaTime * DIRECTION_CHANGE_SPEED);
        my_tank.RotateGun(-deltaTime * DIRECTION_CHANGE_SPEED);
        camera->RotateThirdPerson_OY(-deltaTime * DIRECTION_CHANGE_SPEED, my_tank.GetCurrentPosition());
    }
    else if (window->KeyHold(GLFW_KEY_S) && !game_over) {
        float distance = deltaTime * BACKWARDS_TANK_SPEED;
        my_tank.current_position -= my_tank.tank_direction * distance;
        my_tank.current_gun_position -= my_tank.tank_direction * distance;
        my_tank.gun_barrel_tip = my_tank.current_gun_position + my_tank.gun_direction * 2.0f;

        camera->MoveForward(-deltaTime * BACKWARDS_TANK_SPEED, my_tank.tank_direction);
    }
    else if (window->KeyHold(GLFW_KEY_Z)) {
        my_tank.current_position = glm::vec3(10, 2, 3);
    }
   
}


void tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
        
            camera->RotateThirdPerson_OY(sensivityOY * deltaX, my_tank.GetCurrentPosition());
            camera->RotateThirdPerson_OX(sensivityOX * deltaY);
        }
    }
    else {
        float sensivityOX = 0.005f;

        my_tank.turret_rotation += -sensivityOX * deltaX;
        my_tank.RotateGun(-sensivityOX * deltaX);

    }
}


void tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !game_over) {
        my_tank.ShootBullet(&bullets);
    }
}


void tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema2::OnWindowResize(int width, int height)
{
}
