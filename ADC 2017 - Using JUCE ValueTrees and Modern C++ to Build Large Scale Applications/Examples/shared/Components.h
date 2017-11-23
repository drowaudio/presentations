/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

//==============================================================================
/** A TextButton that pops up a colour chooser to change its colours. */
class ColourChangeButton  : public TextButton,
                            public ChangeListener,
                            private Value::Listener
{
public:
    ColourChangeButton (const String& name = String())
        : TextButton (name)
    {
        colourValue.addListener (this);
        updateColour();
    }

    Value& getColourValueObject()
    {
        return colourValue;
    }

    void clicked() override
    {
        ColourSelector* colourSelector = new ColourSelector();
        colourSelector->setName ("background");
        colourSelector->setCurrentColour (findColour (TextButton::buttonColourId));
        colourSelector->addChangeListener (this);
        colourSelector->setColour (ColourSelector::backgroundColourId, Colours::transparentBlack);
        colourSelector->setSize (300, 400);

        CallOutBox::launchAsynchronously (colourSelector, getScreenBounds(), nullptr);
    }

    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if (ColourSelector* cs = dynamic_cast<ColourSelector*> (source))
            colourValue = cs->getCurrentColour().toString();
    }

private:
    Value colourValue;

    void valueChanged (Value&) override
    {
        updateColour();
    }

    void updateColour()
    {
        setColour (TextButton::buttonColourId, Colour::fromString (colourValue.toString()));
    }
};

inline Colour getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour uiColour, Colour fallback = Colour (0xff4d4d4d))
{
    if (auto* v4 = dynamic_cast<LookAndFeel_V4*> (&LookAndFeel::getDefaultLookAndFeel()))
        return v4->getCurrentColourScheme().getUIColour (uiColour);

    return fallback;
}
