/*
  ==============================================================================

    Utilities.h
    Created: 2 Oct 2017 12:14:45pm
    Author:  David Rowland

  ==============================================================================
*/

#pragma once

namespace IDs
{
    #define DECLARE_ID(name) const juce::Identifier name (#name);

    DECLARE_ID (TREE)
    DECLARE_ID (pi)

    DECLARE_ID (EDIT)
    DECLARE_ID (uuid)
    DECLARE_ID (mediaId)

    DECLARE_ID (TRACK)
    DECLARE_ID (colour)
    DECLARE_ID (name)
    DECLARE_ID (image)

    DECLARE_ID (CLIP)
    DECLARE_ID (start)
    DECLARE_ID (length)
    DECLARE_ID (timestretchOptions)
    DECLARE_ID (NOTE)

    #undef DECLARE_ID
}
