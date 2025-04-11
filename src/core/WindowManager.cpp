#include "WindowManager.h"

WindowManager& WindowManager::Instance() {
    static WindowManager instance;
    return instance;
}

void WindowManager::Init(const sf::Vector2u& resolution, bool fullscreen, unsigned int framerate) {
    sf::Uint32 style = fullscreen ? sf::Style::Fullscreen : sf::Style::Close | sf::Style::Resize;
    window.create(sf::VideoMode(resolution.x, resolution.y), "ChaosTheory", style);
    window.setFramerateLimit(framerate);
    view.setSize(static_cast<float>(resolution.x), static_cast<float>(resolution.y));
    view.setCenter(view.getSize() / 2.f);
    window.setView(view);
}

void WindowManager::Shutdown() {
    window.close();
}

bool WindowManager::IsOpen() const {
    return window.isOpen();
}

void WindowManager::BeginDraw() {
    window.clear();
}

void WindowManager::EndDraw() {
    window.display();
}

void WindowManager::Update() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::Resized) {
            view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
            view.setCenter(view.getSize() / 2.f);
            window.setView(view);
        }
    }
}

sf::RenderWindow& WindowManager::GetWindow() {
    return window;
}

sf::Vector2u WindowManager::GetSize() const {
    return window.getSize();
}