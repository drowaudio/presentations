/*
  ==============================================================================

    ArrangeView.h
    Created: 2 Oct 2017 1:01:56pm
    Author:  David Rowland

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../shared/drow_Utilities.h"
#include "../../shared/drow_ValueTreeObjectList.h"
#include "../../shared/Identifiers.h"
#include "../../shared/ValueTreeItems.h"

struct ArrangeClasses
{
struct ArrangeTrack;

//==============================================================================
struct ArrangeClip  : public Component,
                      public drow::ValueTreePropertyChangeListener
{
    ArrangeClip (ValueTree v, ArrangeTrack& at)
        : state (v), track (at)
    {
        state.addListener (this);
    }

    void paint (Graphics& g) override
    {
        auto r = getLocalBounds();

        const auto bg = getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground).darker (0.15f);
        g.setColour (bg.overlaidWith (track.getColour().withAlpha (0.5f)));
        g.fillAll();

        g.setColour (Colours::black.withAlpha (0.6f));
        g.drawRect (r);

        g.setFont (10.0f);
        g.drawText (state[IDs::name].toString(), r.reduced (2), Justification::topLeft);
    }

    ValueTree state;

private:
    ArrangeTrack& track;

    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override
    {
        if (v == state)
            if (i == IDs::name || i == IDs::colour)
                repaint();
    }
};

//==============================================================================
struct ClipList : public Component,
                  public drow::ValueTreeObjectList<ArrangeClip>
{
    ClipList (ArrangeTrack& at)
        : drow::ValueTreeObjectList<ArrangeClip> (at.state),
          track (at)
    {
        rebuildObjects();
    }

    ~ClipList()
    {
        freeObjects();
    }

    void resized() override
    {
        auto r = getLocalBounds();
        const float maxTime = 30.0f;

        for (auto* ac : objects)
        {
            auto v = ac->state;
            const auto times = Range<float>::withStartAndLength (v[IDs::start], v[IDs::length]);
            const int x = proportionOfWidth (times.getStart() / maxTime);
            const int w = proportionOfWidth (times.getLength() / maxTime);
            ac->setBounds (r.withX (x).withWidth (w));
        }
    }

    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::CLIP);
    }

    ArrangeClip* createNewObject (const juce::ValueTree& v) override
    {
        auto* ac = new ArrangeClip (v, track);
        addAndMakeVisible (ac);
        return ac;
    }

    void deleteObject (ArrangeClip* ac) override
    {
        delete ac;
    }

    void newObjectAdded (ArrangeClip*) override     { resized(); }
    void objectRemoved (ArrangeClip*) override      { resized(); }
    void objectOrderChanged() override              { resized(); }

private:
    ArrangeTrack& track;

    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override
    {
        if (isSuitableType (v))
            if (i == IDs::start || i == IDs::length)
                resized();

        drow::ValueTreeObjectList<ArrangeClip>::valueTreePropertyChanged (v, i);
    }
};

//==============================================================================
struct ArrangeTrack : public Component,
                      public drow::ValueTreePropertyChangeListener
{
    ArrangeTrack (ValueTree v) : state (v)
    {
        addAndMakeVisible (*(clipList = std::make_unique<ClipList> (*this)));
        state.addListener (this);
    }

    Colour getColour() const
    {
        return Colour::fromString (state[IDs::colour].toString());
    }

    void resized() override
    {
        auto r = getLocalBounds();
        clipList->setBounds (r.withX (clipX));
    }

    void paint (Graphics& g) override
    {
        auto r = getLocalBounds();

        g.setColour (getColour().withAlpha (0.5f));
        g.fillAll();
        g.fillRect (r.withRight (clipX));

        g.setColour (getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::defaultText,
                                             Colours::black));
        g.setFont (jlimit (8.0f, 15.0f, getHeight() * 0.9f));
        g.drawText (state[IDs::name].toString(), r.removeFromLeft (clipX).reduced (4, 0), Justification::centredLeft, true);
    }

    ValueTree state;

private:
    const int clipX = 150;
    std::unique_ptr<ClipList> clipList;

    void valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i) override
    {
        if (v == state)
            if (i == IDs::name || i == IDs::colour)
                repaint();
    }
};

//==============================================================================
struct TrackList    : public Component,
                      public drow::ValueTreeObjectList<ArrangeTrack>
{
    TrackList (ValueTree editTree)
        : drow::ValueTreeObjectList<ArrangeTrack> (editTree)
    {
        rebuildObjects();
    }

    ~TrackList()
    {
        freeObjects();
    }

    void resized() override
    {
        auto r = getLocalBounds();
        const int h = r.getHeight() / objects.size();

        for (auto* at : objects)
            at->setBounds (r.removeFromTop (h));
    }

    bool isSuitableType (const juce::ValueTree& v) const override
    {
        return v.hasType (IDs::TRACK);
    }

    ArrangeTrack* createNewObject (const juce::ValueTree& v) override
    {
        auto* at = new ArrangeTrack (v);
        addAndMakeVisible (at);
        return at;
    }

    void deleteObject (ArrangeTrack* at) override
    {
        delete at;
    }

    void newObjectAdded (ArrangeTrack*) override    { resized(); }
    void objectRemoved (ArrangeTrack*) override     { resized(); }
    void objectOrderChanged() override              { resized(); }
};
};

//==============================================================================
class ArrangeView   : public Component
{
public:
    ArrangeView (ValueTree editToUse)
    {
        addAndMakeVisible (*(trackList = std::make_unique<ArrangeClasses::TrackList> (editToUse)));

        setSize (800, 600);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground).darker (0.15f));
    }

    void resized() override
    {
        Rectangle<int> r (getLocalBounds().reduced (4));
        trackList->setBounds (r);
    }

private:
    std::unique_ptr<ArrangeClasses::TrackList> trackList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangeView)
};
