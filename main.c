/*******************************************************************************************
*
*   raylib [models] example - Detect basic 3d collisions (box vs sphere vs box)
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - box collisions");

    // Define the camera to look into our 3d world
    Camera camera = {{ 0.0f, 5.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    //Model model = LoadModel("raylib/examples/models/resources/models/turret.obj");                 // Load OBJ model
    //Texture2D texture = LoadTexture("raylib/examples/models/resources/models/turret_diffuse.png"); // Load model texture
    Model model = LoadModel("char.obj");                 // Load OBJ model
    Texture2D texture = LoadTexture("charcolor.png"); // Load model texture
    model.materials[0].maps[MAP_DIFFUSE].texture = texture;                 // Set map diffuse texture
    
    Vector3 playerPosition = { 0.0f, 3.5f, 2.0f };
    Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
    Color playerColor = GREEN;
    
    Vector3 enemyBoxPos = { -4.0f, 1.0f, 0.0f };
    Vector3 enemyBoxSize = { 2.0f, 2.0f, 2.0f };
    
    Vector3 enemySpherePos = { 4.0f, 0.0f, 0.0f };
    float enemySphereSize = 1.5f;
    
    bool collision = false;

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    //
    float yaw = 0.0f;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        
        // Move player
        if (IsKeyDown(KEY_RIGHT)) {
            camera.target.x += 0.2f;
            camera.position.x += 0.2f;
            yaw = -90.0f;
            playerPosition.x += 0.2f;
        } else if (IsKeyDown(KEY_LEFT)) {
            camera.target.x -= 0.2f;
            camera.position.x -= 0.2f;
            yaw = 90.0f;
            playerPosition.x -= 0.2f;
        } else if (IsKeyDown(KEY_DOWN)) {
            camera.target.z += 0.2f;
            camera.position.z += 0.2f;
            yaw = 0.0f;
            playerPosition.z += 0.2f;
        } else if (IsKeyDown(KEY_UP)) {
            camera.target.z -= 0.2f;
            camera.position.z -= 0.2f;
            yaw = 180.0f;
            playerPosition.z -= 0.2f;
        }

        Matrix transform = MatrixIdentity();
        transform = MatrixMultiply(transform, MatrixRotateY(DEG2RAD * yaw));
        model.transform = transform;
        
        collision = false;
        
        // Check collisions player vs enemy-box
        if (CheckCollisionBoxes(
            (BoundingBox){(Vector3){ playerPosition.x - playerSize.x/2, 
                                     playerPosition.y - playerSize.y/2, 
                                     playerPosition.z - playerSize.z/2 }, 
                          (Vector3){ playerPosition.x + playerSize.x/2,
                                     playerPosition.y + playerSize.y/2, 
                                     playerPosition.z + playerSize.z/2 }},
            (BoundingBox){(Vector3){ enemyBoxPos.x - enemyBoxSize.x/2, 
                                     enemyBoxPos.y - enemyBoxSize.y/2, 
                                     enemyBoxPos.z - enemyBoxSize.z/2 }, 
                          (Vector3){ enemyBoxPos.x + enemyBoxSize.x/2,
                                     enemyBoxPos.y + enemyBoxSize.y/2, 
                                     enemyBoxPos.z + enemyBoxSize.z/2 }})) collision = true;
        
        // Check collisions player vs enemy-sphere
        if (CheckCollisionBoxSphere(
            (BoundingBox){(Vector3){ playerPosition.x - playerSize.x/2, 
                                     playerPosition.y - playerSize.y/2, 
                                     playerPosition.z - playerSize.z/2 }, 
                          (Vector3){ playerPosition.x + playerSize.x/2,
                                     playerPosition.y + playerSize.y/2, 
                                     playerPosition.z + playerSize.z/2 }}, 
            enemySpherePos, enemySphereSize)) collision = true;
        
        if (collision) playerColor = RED;
        else playerColor = GREEN;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw enemy-box
                DrawCube(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
                DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);
                
                // Draw enemy-sphere
                DrawSphere(enemySpherePos, enemySphereSize, GRAY);
                DrawSphereWires(enemySpherePos, enemySphereSize, 16, 16, DARKGRAY);
                
                // Draw player
                DrawModel(model, playerPosition, 0.20f, WHITE);   // Draw 3d model with texture
                //DrawCubeV(playerPosition, playerSize, playerColor);

                DrawGrid(10, 1.0f);        // Draw a grid

            EndMode3D();
            
            DrawText("Move player with cursors to collide", 220, 40, 20, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(model);         // Unload model

    ClearDroppedFiles();        // Clear internal buffers

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
