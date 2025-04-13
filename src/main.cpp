// ============================================================================
//  File        : main.cpp
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Main entry point for the Chaos Theory Project executable
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#include "Settings.h"
#include "core/Application.h"
#include "core/AudioManager.h"
#include <memory>

#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main()
{
    auto settings = std::make_shared<Settings>();

    Application app(settings);
    app.Init();

    // Testing purposes for audio development
    AudioManager::Instance().PlayMusic(settings->m_audioDirectory +
                                       "RootMenu_clean.wav");

    app.Run();

    return 0;
}
