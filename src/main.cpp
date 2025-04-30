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

#include "Application.h"
#include <iostream>

#if defined(_MSC_VER) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main()
{
    Application app;
    app.Init();
    app.Run();

    std::cout << "CT application successfully concluded." << std::endl;
    return 0;
}
