#pragma once

#include <SFML/Graphics.hpp>

class WindowManager {
public:
    static WindowManager& Instance();

    void Init(const sf::Vector2u& resolution, bool fullscreen, unsigned int framerate = 60);
    void Shutdown();
    bool IsOpen() const;
    void BeginDraw();
    void EndDraw();
    void Update();
    sf::RenderWindow& GetWindow();
    sf::Vector2u GetSize() const;

    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
    WindowManager(WindowManager&&) = delete;
    WindowManager& operator=(WindowManager&&) = delete;

private:
    WindowManager() = default;
    ~WindowManager() = default;

    sf::RenderWindow window;
    sf::View view;
};