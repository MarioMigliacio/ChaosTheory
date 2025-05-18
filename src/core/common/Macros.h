// ============================================================================
//  File        : Macros.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-04-13
//  Description : Useful macros to reduce repetitiveness
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "LogManager.h"

#define CT_LOG_TRACE(...) LogManager::Instance().GetLogger()->trace(__VA_ARGS__)
#define CT_LOG_DEBUG(...) LogManager::Instance().GetLogger()->debug(__VA_ARGS__)
#define CT_LOG_INFO(...) LogManager::Instance().GetLogger()->info(__VA_ARGS__)
#define CT_LOG_WARN(...) LogManager::Instance().GetLogger()->warn(__VA_ARGS__)
#define CT_LOG_ERROR(...) LogManager::Instance().GetLogger()->error(__VA_ARGS__)
#define CT_LOG_CRITICAL(...) LogManager::Instance().GetLogger()->critical(__VA_ARGS__)

#define CF_EXIT_EARLY_IF_ALREADY_INITIALIZED()                                                                         \
    if (m_isInitialized)                                                                                               \
    {                                                                                                                  \
        return;                                                                                                        \
    }

#define CT_WARN_IF_UNINITIALIZED(context, task)                                                                        \
    if (!m_isInitialized)                                                                                              \
    {                                                                                                                  \
        CT_LOG_WARN("{}: Attempted to {} without initialization!", context, task);                                     \
        return;                                                                                                        \
    }

#define CT_WARN_IF_UNINITIALIZED_RET(context, task, retval)                                                            \
    if (!m_isInitialized)                                                                                              \
    {                                                                                                                  \
        CT_LOG_WARN("{}: Attempted to {} without initialization!", context, task);                                     \
        return retval;                                                                                                 \
    }