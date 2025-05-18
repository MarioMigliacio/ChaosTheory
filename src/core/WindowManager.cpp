// ============================================================================
//  File        : WindowManager.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Window Manager is the CT library wrapper for the SFML
//                window object, with configurable settings
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "WindowManager.h"
#include "Macros.h"
#include "ResolutionScaleManager.h"
#include "SceneTransitionManager.h"
#include "Settings.h"

/// @brief These static references to certain sf objects are used for short circuit logic where the RenderWindow might
/// not yet be initialized, so doing routine logic would be dangerous.
namespace
{
/// @brief An empty, but valid RenderWindow.
static sf::RenderWindow dummyWindow;
} // namespace

/// @brief Get the current Instance for this WindowManager singleton.
/// @return reference to existing WindowManager interface.
WindowManager &WindowManager::Instance()
{
    static WindowManager instance;
    return instance;
}

/// @brief Initializes the SFML window using the provided settings.
/// @param settings Settings object to initialiize with.
/// @param style sf::Style for window type.
void WindowManager::Init(std::shared_ptr<Settings> settings, sf::Uint32 style)
{
    CF_EXIT_EARLY_IF_ALREADY_INITIALIZED();

    m_settings = settings;
    m_isFullscreen = m_settings->m_isFullscreen;

    m_isInitialized = true;
    ApplySettings(style);

    CT_LOG_INFO("WindowManager initialized.");
}

/// @brief Shuts down the window and resets internal state.
void WindowManager::Shutdown()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "Shutdown");

    if (m_window->isOpen())
    {
        m_window->close();
    }

    m_window.reset();
    m_settings.reset();
    m_isInitialized = false;

    CT_LOG_INFO("WindowManager shutdown.");
}

/// @brief Returns whether the WindowManager has been initialized.
/// @return m_isInitialized.
bool WindowManager::IsInitialized() const
{
    return m_isInitialized;
}

/// @brief Returns whether the SFML window is currently open.
/// @return true / false
bool WindowManager::IsOpen() const
{
    CT_WARN_IF_UNINITIALIZED_RET("WindowManager", "IsOpen", false);

    return m_window && m_window->isOpen();
}

/// @brief Prepares for a new frame.
void WindowManager::BeginDraw()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "BeginDraw");

    m_window->clear(m_clearColor);
}

/// @brief Completes rendering for the current frame.
void WindowManager::EndDraw()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "EndDraw");

    m_window->display();
}

/// @brief Custom recreate window with optional style, and aspect dimensions.
/// @param width Window width x.
/// @param height Window height y.
/// @param title Window title string representation.
/// @param style Window style type.
void WindowManager::Recreate(const unsigned int width, const unsigned int height, const std::string &title,
                             sf::Uint32 style)
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "Recreate");

    if (m_window->getSize().x == width && m_window->getSize().y == height && m_title == title && m_style == style)
    {
        CT_LOG_INFO("WindowManager::Recreate skipped (no changes needed).");

        return;
    }

    sf::VideoMode mode(width, height);

    m_window = std::make_unique<sf::RenderWindow>(mode, title, style);
    m_window->setFramerateLimit(m_settings->m_targetFramerate);
    m_window->setVerticalSyncEnabled(m_settings->m_verticleSyncEnabled);

    m_title = title;
    m_style = style;
}

/// @brief Applies synchronization between the manager settings of the SFML window and the Settings object.
/// @param style sf::Style for window type.
void WindowManager::ApplySettings(sf::Uint32 style)
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "ApplySettings");

    sf::Vector2u size = GetResolutionSize(m_settings->m_resolution);
    sf::VideoMode mode(size.x, size.y);

    m_window = std::make_unique<sf::RenderWindow>(mode, m_settings->m_windowTitle, style);

    m_title = m_settings->m_windowTitle;
    m_style = style;

    m_window->setFramerateLimit(m_settings->m_targetFramerate);
    m_window->setVerticalSyncEnabled(m_settings->m_verticleSyncEnabled);

    ResolutionScaleManager::Instance().SetReferenceResolution(ResolutionSetting::Res720p);
    ResolutionScaleManager::Instance().SetCurrentResolution(m_window->getSize());

    CT_LOG_INFO("Applied initial settings: {}x{}", m_window->getSize().x, m_window->getSize().y);
}

/// @brief Applies the Resolution settings for the window.
/// @param res ResolutionSettings size.
void WindowManager::ApplyResolution(ResolutionSetting res)
{
    sf::Vector2u size;

    switch (res)
    {
        case ResolutionSetting::Res720p:
            size = {1280, 720};
            break;
        case ResolutionSetting::Res1080p:
            size = {1920, 1080};
            break;
        case ResolutionSetting::Fullscreen:
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            size = {desktop.width, desktop.height};
            break;
    }

    sf::Uint32 style = (res == ResolutionSetting::Fullscreen) ? sf::Style::Fullscreen : sf::Style::Close;

    m_window->create(sf::VideoMode(size.x, size.y), m_settings->m_windowTitle, style);

    // Delay before requestFocus - OS can silently fail, delay min helps run more consistently.
    sf::sleep(sf::milliseconds(100));
    m_window->requestFocus();

    m_window->setVerticalSyncEnabled(m_settings->m_verticleSyncEnabled);

    ResolutionScaleManager::Instance().SetReferenceResolution(ResolutionSetting::Res720p);
    ResolutionScaleManager::Instance().SetCurrentResolution(m_window->getSize());

    CT_LOG_INFO("Applied new resolution: {}x{} - vsync: {}", size.x, size.y, m_settings->m_verticleSyncEnabled);
}

/// @brief Returns the resolution size for this window.
/// @param setting ResolutionSettings to help return size of window.
/// @return Vector2u of {width, height}.
sf::Vector2u WindowManager::GetResolutionSize(ResolutionSetting setting) const
{
    switch (setting)
    {
        case ResolutionSetting::Res720p:
        default:
            return {1280, 720};
        case ResolutionSetting::Res1080p:
            return {1920, 1080};
        case ResolutionSetting::Fullscreen:
        {
            sf::VideoMode mode = sf::VideoMode::getDesktopMode();
            return {mode.width, mode.height};
        }
    }
}

/// @brief Sets the clear color for the window, which can be useful during startdraw / enddraw steps.
/// @param color new m_clearColor.
void WindowManager::SetClearColor(const sf::Color &color)
{
    m_clearColor = color;
}

/// @brief Changes the internal window state for full screen on / off.
void WindowManager::ToggleFullscreen()
{
    CT_WARN_IF_UNINITIALIZED("WindowManager", "ToggleFullscreen");

    m_isFullscreen = !m_isFullscreen;
    m_settings->m_isFullscreen = m_isFullscreen;

    ApplySettings(sf::Style::Fullscreen);
}

/// @brief Polls all pending window events.
/// @param event event to pop from queue during pollEvent.
/// @return True if an event was returned, or false if the event queue was empty.
bool WindowManager::PollEvent(sf::Event &event)
{
    CT_WARN_IF_UNINITIALIZED_RET("WindowManager", "PollEvent", false);

    return m_window && m_window->pollEvent(event);
}

/// @brief Returns a reference to the Window managers internal SFML window.
/// @return m_window.
sf::RenderWindow &WindowManager::GetWindow()
{
    CT_WARN_IF_UNINITIALIZED_RET("WindowManager", "GetWindow", dummyWindow);

    return *m_window;
}
