// ============================================================================
//  File        : InputManager.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-11
//  Description : Input Manager is the CT library dedicated manager of 
//                user inputs from any source
//  
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "Settings.h"

class InputManager {
public:
    static InputManager& Instance();

    void Init(const Settings& settings);
    void Shutdown();
    void Update();

    bool IsKeyPressed(int key) const;

private:
    InputManager() = default;
    ~InputManager() = default;

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

private:
    const Settings* settings = nullptr;
};
