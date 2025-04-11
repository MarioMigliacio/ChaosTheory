#pragma once

class Application {
public:
    Application() = default;
    ~Application() = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Run();

private:
    void Init();
    void Shutdown();
    void ProcessEvents();
    void Update(float dt);
    void Render();

    bool isRunning = true;
};