/*
  ==============================================================================

    Utilities.h
    Created: 2 Oct 2017 12:14:45pm
    Author:  David Rowland

  ==============================================================================
*/

#pragma once

namespace drow
{

//==============================================================================
/** Iterates through a list of Components, calling a function on each.
    @param fn   The signature of the fn should be equivalent to "void fn (Component* c)"
*/
template<typename FunctionType>
inline void visitComponents (std::initializer_list<Component*> comps, FunctionType&& fn)
{
    std::for_each (std::begin (comps), std::end (comps), fn);
}

//==============================================================================
/** Adds and makes visible any number of Components to a parent. */
inline void addAndMakeVisible (Component& parent, Array<Component*> comps)
{
    std::for_each (std::begin (comps), std::end (comps),
                   [&parent] (Component* c) { parent.addAndMakeVisible (c); });
}

/** Adds and makes visible any number of Components to a parent. */
inline void addAndMakeVisible (Component& parent, std::initializer_list<Component*> comps)
{
    std::for_each (std::begin (comps), std::end (comps),
                   [&parent] (Component* c) { parent.addAndMakeVisible (c); });
}

//==============================================================================
/** Attempts to load a ValueTree from a file. */
static inline juce::ValueTree loadValueTree (const juce::File& file, bool asXml)
{
    if (asXml)
    {
        if (auto xml = std::unique_ptr<juce::XmlElement> (juce::XmlDocument::parse (file)))
            return juce::ValueTree::fromXml (*xml);
    }
    else
    {
        juce::FileInputStream is (file);

        if (is.openedOk())
            return juce::ValueTree::readFromStream (is);
    }

    return {};
}

/** Saves a ValueTree to a File. */
static inline bool saveValueTree (const juce::ValueTree& v, const juce::File& file, bool asXml)
{
    const juce::TemporaryFile temp (file);

    {
        juce::FileOutputStream os (temp.getFile());

        if (! os.getStatus().wasOk())
            return false;

        if (asXml)
        {
            if (auto xml = std::unique_ptr<juce::XmlElement> (v.createXml()))
                xml->writeToStream (os, juce::StringRef());
        }
        else
        {
            v.writeToStream (os);
        }
    }

    if (temp.getFile().existsAsFile())
        return temp.overwriteTargetFileWithTemporary();

    return false;
}

//==============================================================================
/**
    Utility wrapper for ValueTree::Listener's that only want to override valueTreePropertyChanged.
*/
struct ValueTreePropertyChangeListener  : public ValueTree::Listener
{
    void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
    void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
    void valueTreeParentChanged (ValueTree&) override {}
    void valueTreeRedirected (ValueTree&) override {}
};

}
