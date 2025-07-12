// ============================================================================
//  File        : DialogQueue.h
//  Project     : ChaosTheory (CT)
//  Author      : Mario Migliacio
//  Created     : 2025-07-12
//  Description : Host definition for helper structs towards a dialog flow
//                using the UIChatBox.
//
//  License     : N/A Open source
//                Copyright (c) 2025 Mario Migliacio
// ============================================================================

#pragma once

#include "DialogLine.h"
#include <queue>

// ============================================================================
//  Class       : DialogQueue
//  Purpose     : Handles a sequence of DialogLine objects and progression logic.
//
//  Responsibilities:
//      - Holds text lines in queue collection.
//      - Advances to next text line.
//
// ============================================================================
class DialogQueue
{
  public:
    DialogQueue() = default;
    ~DialogQueue() = default;

    void AddLine(const DialogLine &line)
    {
        m_queue.push(line);
    }

    bool HasNext() const
    {
        return !m_queue.empty();
    }

    DialogLine Next()
    {
        DialogLine line = m_queue.front();
        m_queue.pop();

        return line;
    }

    void Clear()
    {
        std::queue<DialogLine> empty;
        std::swap(m_queue, empty);
    }

    bool IsEmpty() const
    {
        return m_queue.empty();
    }

  private:
    std::queue<DialogLine> m_queue;
};
