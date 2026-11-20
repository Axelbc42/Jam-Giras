#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

int main()
{
    const int anchoPantalla = 1280;
    const int altoPantalla = 720;

    InitWindow(anchoPantalla, altoPantalla, "Prueba raylib + ImGui");
    SetTargetFPS(60);
    rlImGuiSetup(true);                       // true = tema oscuro

    // Estado del juego
    Vector2 pos = { anchoPantalla / 2.0f, altoPantalla / 2.0f };
    float radio = 30.0f;
    float velocidad = 300.0f;                 // píxeles por segundo
    float color[3] = { 0.8f, 0.2f, 0.2f };    // RGB entre 0 y 1
    bool mostrarDemo = false;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // --- Actualizar (solo si ImGui no está capturando el teclado) ---
        if (!ImGui::GetIO().WantCaptureKeyboard)
        {
            if (IsKeyDown(KEY_RIGHT)) pos.x += velocidad * dt;
            if (IsKeyDown(KEY_LEFT))  pos.x -= velocidad * dt;
            if (IsKeyDown(KEY_DOWN))  pos.y += velocidad * dt;
            if (IsKeyDown(KEY_UP))    pos.y -= velocidad * dt;
        }

        // Mantener la bola dentro de la ventana
        if (pos.x < radio) pos.x = radio;
        if (pos.x > anchoPantalla - radio) pos.x = anchoPantalla - radio;
        if (pos.y < radio) pos.y = radio;
        if (pos.y > altoPantalla - radio) pos.y = altoPantalla - radio;

        // --- Dibujar ---
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Parte raylib
        Color c = {
            (unsigned char)(color[0] * 255),
            (unsigned char)(color[1] * 255),
            (unsigned char)(color[2] * 255),
            255
        };
        DrawCircleV(pos, radio, c);
        DrawText("Muevete con las flechas", 10, 10, 20, RAYWHITE);
        DrawFPS(anchoPantalla - 90, 10);

        // Parte ImGui
        rlImGuiBegin();

        ImGui::Begin("Panel de control");
        ImGui::Text("Pos: %.0f, %.0f", pos.x, pos.y);
        ImGui::SliderFloat("Velocidad", &velocidad, 50.0f, 800.0f);
        ImGui::SliderFloat("Radio", &radio, 5.0f, 100.0f);
        ImGui::ColorEdit3("Color", color);
        if (ImGui::Button("Centrar bola"))
            pos = { anchoPantalla / 2.0f, altoPantalla / 2.0f };
        ImGui::Checkbox("Mostrar demo de ImGui", &mostrarDemo);
        ImGui::End();

        if (mostrarDemo) ImGui::ShowDemoWindow(&mostrarDemo);

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    //CloseWindow();
    return 0;
}