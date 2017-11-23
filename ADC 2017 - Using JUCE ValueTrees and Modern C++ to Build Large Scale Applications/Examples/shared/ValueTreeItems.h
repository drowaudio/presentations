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

#include "Components.h"
#include "Identifiers.h"

namespace Helpers
{
    template<typename TreeViewItemType>
    inline OwnedArray<ValueTree> getSelectedTreeViewItems (TreeView& treeView)
    {
        OwnedArray<ValueTree> items;
        const int numSelected = treeView.getNumSelectedItems();

        for (int i = 0; i < numSelected; ++i)
            if (auto* vti = dynamic_cast<TreeViewItemType*> (treeView.getSelectedItem (i)))
                items.add (new ValueTree (vti->getState()));

        return items;
    }

    inline void moveItems (TreeView& treeView, const OwnedArray<ValueTree>& items,
                           ValueTree newParent, int insertIndex, UndoManager& undoManager)
    {
        if (items.isEmpty())
            return;

        std::unique_ptr<XmlElement> oldOpenness (treeView.getOpennessState (false));

        for (int i = items.size(); --i >= 0;)
        {
            ValueTree& v = *items.getUnchecked (i);

            if (v.getParent().isValid() && newParent != v && ! newParent.isAChildOf (v))
            {
                if (v.getParent() == newParent && newParent.indexOf (v) < insertIndex)
                    --insertIndex;

                v.getParent().removeChild (v, &undoManager);
                newParent.addChild (v, insertIndex, &undoManager);
            }
        }

        if (oldOpenness != nullptr)
            treeView.restoreOpennessState (*oldOpenness, false);
    }

    inline ValueTree createUuidProperty (ValueTree& v)
    {
        if (! v.hasProperty (IDs::uuid))
            v.setProperty (IDs::uuid, Uuid().toString(), nullptr);

        return v;
    }

    inline ValueTree createDefaultEdit()
    {
        ValueTree edit (IDs::EDIT);
        edit.setProperty (IDs::name, "My First Edit", nullptr);
        Helpers::createUuidProperty (edit);

        for (int tn = 0; tn < 8; ++tn)
        {
            ValueTree t (IDs::TRACK);
            const String trackName ("Track " + String (tn + 1));
            t.setProperty (IDs::colour, Colour::fromHSV ((1.0f / 8.0f) * tn, 0.65f, 0.65f, 1.0f).toString(), nullptr);
            t.setProperty (IDs::name, trackName, nullptr);
            Helpers::createUuidProperty (t);

            for (int cn = 0; cn < 3; ++cn)
            {
                ValueTree c (IDs::CLIP);
                Helpers::createUuidProperty (c);
                c.setProperty (IDs::name, trackName + ", Clip " + String (cn + 1), nullptr);
                c.setProperty (IDs::start, cn, nullptr);
                c.setProperty (IDs::length, 1.0, nullptr);
                t.addChild (c, -1, nullptr);
            }

            edit.addChild (t, -1, nullptr);
        }

        return edit;
    }
}

//==============================================================================
class ValueTreeItem;

/** Creates the various concrete types below. */
ValueTreeItem* createValueTreeItemForType (const ValueTree&, UndoManager&);

//==============================================================================
class ValueTreeItem : public TreeViewItem,
                      protected ValueTree::Listener
{
public:
    ValueTreeItem (const ValueTree& v, UndoManager& um)
        : state (v), undoManager (um)
    {
        state.addListener (this);
    }

    ValueTree getState() const
    {
        return state;
    }

    UndoManager* getUndoManager() const
    {
        return &undoManager;
    }

    virtual String getDisplayText()
    {
        return state[IDs::name].toString();
    }

    String getUniqueName() const override
    {
        if (state.hasProperty (IDs::uuid))
            return state[IDs::uuid].toString();

        return state[IDs::mediaId].toString();
    }

    bool mightContainSubItems() override
    {
        return state.getNumChildren() > 0;
    }

    void paintItem (Graphics& g, int width, int height) override
    {
        if (isSelected())
        {
            g.setColour (Colours::red);
            g.drawRect ({ (float) width, (float) height }, 1.5f);
        }

        const auto col = Colour::fromString (state[IDs::colour].toString());

        if (! col.isTransparent())
            g.fillAll (col.withAlpha (0.5f));

        g.setColour (getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::defaultText,
                                             Colours::black));
        g.setFont (15.0f);
        g.drawText (getDisplayText(), 4, 0, width - 4, height,
                    Justification::centredLeft, true);
    }

    void itemOpennessChanged (bool isNowOpen) override
    {
        if (isNowOpen && getNumSubItems() == 0)
            refreshSubItems();
        else
            clearSubItems();
    }

    void itemSelectionChanged (bool /*isNowSelected*/) override
    {
        if (auto* ov = getOwnerView())
            if (auto* cb = dynamic_cast<ChangeBroadcaster*> (ov->getRootItem()))
                cb->sendChangeMessage();
    }

    var getDragSourceDescription() override
    {
        return state.getType().toString();
    }

protected:
    ValueTree state;
    UndoManager& undoManager;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override          { repaintItem(); }
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override           { treeChildrenChanged (parentTree); }
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override    { treeChildrenChanged (parentTree); }
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override      { treeChildrenChanged (parentTree); }
    void valueTreeParentChanged (ValueTree&) override                               {}

    void treeChildrenChanged (const ValueTree& parentTree)
    {
        if (parentTree == state)
        {
            refreshSubItems();
            treeHasChanged();
            setOpen (true);
        }
    }

private:
    void refreshSubItems()
    {
        clearSubItems();

        for (const auto& v : state)
            if (auto* item = createValueTreeItemForType (v, undoManager))
                addSubItem (item);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeItem)
};

//==============================================================================
class Clip : public ValueTreeItem
{
public:
    Clip (const ValueTree& v, UndoManager& um)
        : ValueTreeItem (v, um)
    {
        jassert (state.hasType (IDs::CLIP));
    }

    bool mightContainSubItems() override
    {
        return false;
    }

    String getDisplayText() override
    {
        auto timeRange = Range<double>::withStartAndLength (state[IDs::start], state[IDs::length]);
        return ValueTreeItem::getDisplayText() + " (" + String (timeRange.getStart(), 2) + " - " + String (timeRange.getEnd(), 2) + ")";
    }

    bool isInterestedInDragSource (const DragAndDropTarget::SourceDetails&) override
    {
        return false;
    }
};

//==============================================================================
class Track : public ValueTreeItem
{
public:
    Track (const ValueTree& v, UndoManager& um)
        : ValueTreeItem (v, um)
    {
        jassert (state.hasType (IDs::TRACK));
    }

    bool isInterestedInDragSource (const DragAndDropTarget::SourceDetails& dragSourceDetails) override
    {
        return dragSourceDetails.description == IDs::CLIP.toString();
    }

    void itemDropped (const DragAndDropTarget::SourceDetails&, int insertIndex) override
    {
        Helpers::moveItems (*getOwnerView(), Helpers::getSelectedTreeViewItems<Clip> (*getOwnerView()), state, insertIndex, undoManager);
    }
};

//==============================================================================
class Edit  : public ValueTreeItem,
              public ChangeBroadcaster
{
public:
    Edit (const ValueTree& v, UndoManager& um)
        : ValueTreeItem (v, um)
    {
        jassert (state.hasType (IDs::EDIT));
    }

    bool isInterestedInDragSource (const DragAndDropTarget::SourceDetails& dragSourceDetails) override
    {
        return dragSourceDetails.description == IDs::TRACK.toString();
    }

    void itemDropped (const DragAndDropTarget::SourceDetails&, int insertIndex) override
    {
        Helpers::moveItems (*getOwnerView(), Helpers::getSelectedTreeViewItems<Track> (*getOwnerView()), state, insertIndex, undoManager);
    }
};

//==============================================================================
inline ValueTreeItem* createValueTreeItemForType (const ValueTree& v, UndoManager& um)
{
    if (v.hasType (IDs::EDIT))  return new Edit (v, um);
    if (v.hasType (IDs::TRACK)) return new Track (v, um);
    if (v.hasType (IDs::CLIP))  return new Clip (v, um);

    //jassertfalse;
    return nullptr;
}
