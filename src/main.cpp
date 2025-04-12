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

int main()
{
    Settings settings;
    Application app(settings);
    app.Run();

    return 0;
}
