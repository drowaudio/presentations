/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "../../shared/ValueTreeItems.h"
#include "../../shared/drow_Utilities.h"
#include "../../shared/drow_ValueTreeObjectList.h"
#include "ValueTreesDemo.h"

#include <future>

//==============================================================================
template<typename Type>
struct AtomicWrapper
{
    AtomicWrapper() = default;

    template<typename OtherType>
    AtomicWrapper (const OtherType& other)
    {
        value.store (other);
    }

    AtomicWrapper (const AtomicWrapper& other)
    {
        value.store (other.value);
    }

    AtomicWrapper& operator= (const AtomicWrapper& other) noexcept
    {
        value.store (other.value);
        return *this;
    }

    bool operator== (const AtomicWrapper& other) const noexcept
    {
        return value.load() == other.value.load();
    }

    bool operator!= (const AtomicWrapper& other) const noexcept
    {
        return value.load() != other.value.load();
    }

    operator var() const noexcept
    {
        return value.load();
    }

    operator Type() const noexcept
    {
        return value.load();
    }

    std::atomic<Type> value { Type() };
};

//==============================================================================
template<typename Type, typename Constrainer>
struct ConstrainerWrapper
{
    ConstrainerWrapper() = default;

    template<typename OtherType>
    ConstrainerWrapper (const OtherType& other)
    {
        value = Constrainer::constrain (other);
    }

    ConstrainerWrapper (const ConstrainerWrapper& other)
    {
        value = other.value;
    }

    ConstrainerWrapper& operator= (const ConstrainerWrapper& other) noexcept
    {
        value = Constrainer::constrain (other.value);
        return *this;
    }

    bool operator== (const ConstrainerWrapper& other) const noexcept    { return value == other.value; }
    bool operator!= (const ConstrainerWrapper& other) const noexcept    { return value != other.value; }

    operator var() const noexcept                                       { return Constrainer::constrain (value); }
    operator Type() const noexcept                                      { return Constrainer::constrain (value); }

    Type value = Type();
};

//==============================================================================
template<typename Type, int StartValue, int EndValue>
struct RangeConstrainer
{
    static Type constrain (const Type& v)
    {
        const Type start = static_cast<Type> (StartValue);
        const Type end = static_cast<Type> (EndValue);

        return Range<Type> (start, end).clipValue (v);
    }
};

//==============================================================================
namespace juce
{
    template<>
    struct VariantConverter<Image>
    {
        static Image fromVar (const var& v)
        {
            if (auto* mb = v.getBinaryData())
                return ImageFileFormat::loadFrom (mb->getData(), mb->getSize());

            return {};
        }

        static var toVar (const Image& i)
        {
            MemoryBlock mb;

            {
                MemoryOutputStream os (mb, false);

                if (! JPEGImageFormat().writeImageToStream (i, os))
                    return {};
            }

            return std::move (mb);
        }
    };

    template<>
    struct VariantConverter<Colour>
    {
        static Colour fromVar (const var& v)
        {
            return Colour::fromString (v.toString());
        }

        static var toVar (const Colour& c)
        {
            return c.toString();
        }
    };
}

String getVarType (const var& v)
{
    if (v.isVoid())         return "void";
    if (v.isUndefined())    return "undefined";
    if (v.isInt())          return "int";
    if (v.isInt64())        return "int64";
    if (v.isBool())         return "bool";
    if (v.isDouble())       return "double";
    if (v.isString())       return "String";
    if (v.isObject())       return "Object";
    if (v.isArray())        return "Array";
    if (v.isBinaryData())   return "BinaryData";
    if (v.isMethod())       return "Method";

    jassertfalse;
    return "error";
}

void varExample()
{
    var v (3.141);
    DBG("Type: " << getVarType (v));
    DBG("Value: " << static_cast<double> (v));
    DBG("Convert to a String: " << v.toString());
    DBG("Type: " << getVarType (v)); // Still a double

    v = "Hello World!";
    DBG("Type: " << getVarType (v));
}

void identifierExample()
{
    DBG(IDs::TREE.toString());
    DBG(IDs::pi.toString());

    const Identifier localTree ("TREE");
    const String treeString ("TREE");
    const Identifier treeFromString (treeString);

    if (IDs::TREE == localTree && localTree == treeFromString)
        DBG("All equivalent");
    else
        DBG("Not the same");
}

void valueTreeExample()
{
    ValueTree v (IDs::TREE);
    DBG(v.toXmlString());

    ValueTree clip (IDs::CLIP);
    clip.setProperty (IDs::start, 42.0, nullptr);
    clip.setProperty (IDs::length, 10.0, nullptr);
    v.addChild (clip, -1, nullptr);
    DBG(v.toXmlString());
}

void valueTreeReferenceCountingExample()
{
    ValueTree v1 (IDs::TREE);
    DBG("1. Ref count v1: " << v1.getReferenceCount());

    ValueTree v2 (v1);
    DBG("2. Ref count v1: " << v1.getReferenceCount());
    DBG("3. Ref count v2: " << v2.getReferenceCount());

    ValueTree v3 (v1.createCopy());
    DBG("4. Ref count v3: " << v3.getReferenceCount());
}

void valueTreeAsyncUpdater()
{
    struct Widget   : public ValueTree::Listener,
                      private AsyncUpdater
    {
        Widget (ValueTree v) : state (v)
        {
            state.addListener (this);
        }

        ValueTree state;

        void handleAsyncUpdate() override
        {
            // Sort notes
        }

        void valueTreePropertyChanged (ValueTree& v, const Identifier& id) override
        {
            if (v.hasType (IDs::NOTE))
                if (id == IDs::start)
                triggerAsyncUpdate();
        }
        void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
        void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
        void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
        void valueTreeParentChanged (ValueTree&) override {}
    };
}

void valueTreeCallbacks()
{
    struct Widget   : public ValueTree::Listener
    {
        Widget (ValueTree v) : state (v)
        {
            state.addListener (this);
        }

        ValueTree state;

        void valueTreePropertyChanged (ValueTree&, const Identifier&) override {}
        void valueTreeChildAdded (ValueTree&, ValueTree&) override {}
        void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override {}
        void valueTreeChildOrderChanged (ValueTree&, int, int) override {}
        void valueTreeParentChanged (ValueTree&) override {}
    };
}

void valueTreeUndoRedo()
{
    UndoManager um;
    ValueTree v (IDs::CLIP);
    v.setProperty (IDs::start, 0.0, &um);
    v.setProperty (IDs::length, 42.0, &um);
    DBG(v.toXmlString());

    um.beginNewTransaction();
    v.setProperty (IDs::length, 10.0, &um);
    DBG(v.toXmlString());

    um.undo();
    DBG("Undoing:");
    DBG(v.toXmlString());
}

void valueExample()
{
    Value v1 (42);
    Value v2 (3.141);

    DBG("v1: " << v1.toString());
    DBG("v2: " << v2.toString());

    v2.referTo (v1);
    DBG("v2: " << v2.toString());
}

void syncronousValueSource()
{
    struct SyncronousValueSource    : public Value::ValueSource
    {
        SyncronousValueSource() = default;

        SyncronousValueSource (const var& initialValue)
            : value (initialValue)
        {
        }

        var getValue() const override
        {
            return value;
        }

        void setValue (const var& newValue) override
        {
            if (! newValue.equalsWithSameType (value))
            {
                value = newValue;
                sendChangeMessage (true);
            }
        }

    private:
        var value;
    };
}

void customValueSource()
{
    struct Transport : public ChangeBroadcaster
    {
        Transport() = default;
        void start()    { isPlaying = true; sendSynchronousChangeMessage(); }
        void stop()     { isPlaying = false; sendSynchronousChangeMessage(); }

        bool isPlaying = false;
    };

    struct TransportValueSource : public Value::ValueSource,
                                  private ChangeListener
    {
        TransportValueSource (Transport& t)
            : transport (t)
        {
            transport.addChangeListener (this);
        }

        ~TransportValueSource()
        {
            transport.removeChangeListener (this);
        }

        var getValue() const override
        {
            return transport.isPlaying;
        }

        void setValue (const var& newValue) override
        {
            if (newValue)   transport.start();
            else            transport.stop();
        }

        void changeListenerCallback (ChangeBroadcaster*) override
        {
            sendChangeMessage (true);
        }

        Transport& transport;
    };

    Transport transport;
    Value transportValue (Value (new TransportValueSource (transport)));
    DBG("playing: " << (int) transport.isPlaying);
    DBG("value: " << (int) transportValue.getValue());

    DBG("\nSTARTING");
    transport.start();
    DBG("playing: " << (int) transport.isPlaying);
    DBG("value: " << (int) transportValue.getValue());

    DBG("\nSETTING VALUE: 0");
    transportValue.setValue (false);
    DBG("playing: " << (int) transport.isPlaying);
    DBG("value: " << (int) transportValue.getValue());
}

void serialisationTypes()
{
    ValueTree v (IDs::TREE);
    v.setProperty (IDs::pi, double_Pi, nullptr);
    DBG("Type before: " << getVarType (v[IDs::pi]));

    std::unique_ptr<XmlElement> xml (v.createXml());
    DBG("Type after XML: " << getVarType (ValueTree::fromXml (*xml)[IDs::pi]));

    MemoryBlock memory;

    {
        MemoryOutputStream mos (memory, false);
        v.writeToStream (mos);
    }

    MemoryInputStream mis (memory, false);
    DBG("Type after binary: " << getVarType (ValueTree::readFromStream (mis)[IDs::pi]));


    MemoryBlock memory2;

    {
        MemoryOutputStream mos (memory2, false);
        GZIPCompressorOutputStream gzip (&mos);
        v.writeToStream (gzip);
    }

    MemoryInputStream mis2 (memory2, false);
    DBG("Type after compressed: " << getVarType (ValueTree::readFromGZIPData (mis2.getData(), mis2.getDataSize())[IDs::pi]));
}

void timetstretchOptions()
{
    struct TimestretchOptions
    {
        TimestretchOptions() = default;
        TimestretchOptions (const String& s)
        {
            auto tokens = StringArray::fromTokens (s, "|", "");
            stereoMS            = tokens[0].getIntValue() != 0;
            syncroniseTimePitch = tokens[1].getIntValue() != 0;
            preserveFormants    = tokens[2].getIntValue() != 0;
            envelopeOrder       = tokens[3].getIntValue();
        }

        String toString() const
        {
            StringArray s;
            s.add (stereoMS ? "1" : "0");
            s.add (syncroniseTimePitch ? "1" : "0");
            s.add (preserveFormants ? "1" : "0");
            s.add (String (envelopeOrder));
            return s.joinIntoString ("|");
        }

        bool stereoMS = false, syncroniseTimePitch = false, preserveFormants = false;
        int envelopeOrder = 64;
    };

    TimestretchOptions options;
    options.syncroniseTimePitch = true;
    options.preserveFormants = true;
    options.envelopeOrder = 128;

    ValueTree v (IDs::CLIP);
    v.setProperty (IDs::timestretchOptions, options.toString(), nullptr);
    DBG(TimestretchOptions (v[IDs::timestretchOptions].toString()).toString());
}

namespace examples
{
    struct Clip : public ReferenceCountedObject
    {
        Clip (const ValueTree& v) : state (v)
        {
            jassert (v.hasType (IDs::CLIP));
        }

        double getStart() const
        {
            return state[IDs::start];
        }

        void setStart (double time)
        {
            jassert (time > 0.0);
            state.setProperty (IDs::start, jmax (0.0, time), nullptr);
        }

        Colour getColour() const
        {
            return Colour::fromString (state[IDs::colour].toString());
        }

        void setColour (Colour c)
        {
            state.setProperty (IDs::colour, c.toString(), nullptr);
        }

        ValueTree state;
    };

    void stronglyTypedObjects()
    {
        ValueTree clipState (IDs::CLIP);
        Clip c (clipState);
        c.setStart (-1.0);          // Triggers an assertion (or throws exception)
        c.setColour (Colours::red); // Has to be a colour
        DBG(c.state.toXmlString()); // Start time is correctly 0.0
    }

    struct CachedValueClip : public ReferenceCountedObject
    {
        CachedValueClip (const ValueTree& v) : state (v)
        {
            jassert (v.hasType (IDs::CLIP));
            start.referTo (state, IDs::start, nullptr);
            colour.referTo (state, IDs::colour, nullptr);
        }

        ValueTree state;
        CachedValue<double> start;
        CachedValue<Colour> colour;
    };

    void stronglyTypedObjectsWithCachedValues()
    {
        ValueTree clipState (IDs::CLIP);
        CachedValueClip c (clipState);
        c.start = -1.0;
        c.colour = Colours::red;
        DBG(c.state.toXmlString());
    }

    void valueTreeObjectList()
    {
        struct ClipList : public drow::ValueTreeObjectList<Clip>
        {
            ClipList (const ValueTree& v)
                : drow::ValueTreeObjectList<Clip> (v)
            {
                rebuildObjects();
            }

            ~ClipList()
            {
                freeObjects();
            }

            bool isSuitableType (const ValueTree& v) const override
            {
                return v.hasType (IDs::CLIP);
            }

            Clip* createNewObject (const juce::ValueTree& v) override
            {
                return new Clip (v);
            }

            void deleteObject (Clip* c) override
            {
                delete c;
            }

            void newObjectAdded (Clip*) override    {}
            void objectRemoved (Clip*) override     {}
            void objectOrderChanged() override      {}
        };

        ValueTree track (IDs::TRACK);
        track.addChild (ValueTree (IDs::CLIP), -1, nullptr);
        track.addChild (ValueTree (IDs::CLIP), -1, nullptr);

        ClipList clipList (track);
        DBG("Num Clips: " << clipList.objects.size());

        // Add another CLIP child
        track.addChild (ValueTree (IDs::CLIP), -1, nullptr);
        DBG("Num Clips: " << clipList.objects.size());

        // Call some methods
        for (auto c : clipList.objects)
            c->setColour (Colours::blue);

        DBG(track.toXmlString());
    }

    void referenceCountedValueTreeObjectList()
    {
        struct ClipList : private drow::ValueTreeObjectList<Clip>
        {
            ClipList (const ValueTree& v)
                : drow::ValueTreeObjectList<Clip> (v)
            {
                rebuildObjects();
            }

            ~ClipList()                                     { freeObjects(); }

            /** Returns the clips in a thread-safe way. */
            ReferenceCountedArray<Clip> getClips() const    { return clips; }

        private:
            bool isSuitableType (const ValueTree& v) const override
            {
                return v.hasType (IDs::CLIP);
            }

            Clip* createNewObject (const juce::ValueTree& v) override
            {
                auto c = new Clip (v);
                clips.add (c);
                return c;
            }

            void deleteObject (Clip* c) override
            {
                clips.removeObject (c);
            }

            void newObjectAdded (Clip*) override    {}
            void objectRemoved (Clip*) override     {}
            void objectOrderChanged() override      {}

            ReferenceCountedArray<Clip, CriticalSection> clips;
        };

        ReferenceCountedObjectPtr<Clip> clip;

        {
            ValueTree track (IDs::TRACK);
            track.addChild (ValueTree (IDs::CLIP), -1, nullptr);
            track.addChild (ValueTree (IDs::CLIP), -1, nullptr);

            ClipList clipList (track);
            DBG("Num Clips: " << clipList.getClips().size());

            // Call some methods
            for (auto c : clipList.getClips())
                c->setColour (Colours::blue);

            clip = clipList.getClips()[0];
        }

        DBG(clip->state.toXmlString());
    }

    void atomicCachedValue()
    {
        CachedValue<AtomicWrapper<double>> start;
//        CachedValue<double> start; // Causes a data-race!

        ValueTree c (IDs::CLIP);
        start.referTo (c, IDs::start, nullptr);

        {
            auto read = std::async (std::launch::async, [&start]
                                    {
                                        for (int i = 0; i < 10000; ++i)
                                            DBG(start.get());
                                    });

            for (int i = 0; i < 10000; ++i)
                start = start.get() + 1.0f;
        }

        DBG("start: " << static_cast<double> (start.get()));
    }

    void rangedCachedValue()
    {
        {
            struct StartTimeConstrainer
            {
                static double constrain (const double& v)   { return Range<double> (0.0, 42.0).clipValue (v); }
            };

            struct CachedValueClip : public ReferenceCountedObject
            {
                CachedValueClip (const ValueTree& v) : state (v)
                {
                    jassert (v.hasType (IDs::CLIP));
                    start.referTo (state, IDs::start, nullptr);
                    colour.referTo (state, IDs::colour, nullptr);
                }

                ValueTree state;
                CachedValue<ConstrainerWrapper<double, StartTimeConstrainer>> start;
                CachedValue<Colour> colour;
            };

            ValueTree clipState (IDs::CLIP);
            CachedValueClip c (clipState);

            c.start = 0.0;
            DBG("start: " << c.start.get());

            c.start = 10.0;
            DBG("start: " << c.start.get());

            c.start = 43.0;
            DBG("start: " << c.start.get());

            c.start = -1.0;
            DBG("start: " << c.start.get());

            DBG(clipState.toXmlString());
        }

        {
            struct CachedValueClip
            {
                CachedValueClip (const ValueTree& v) : state (v)
                {
                    jassert (v.hasType (IDs::CLIP));
                    start.referTo (state, IDs::start, nullptr);
                    colour.referTo (state, IDs::colour, nullptr);
                }

                ValueTree state;
                CachedValue<ConstrainerWrapper<double, RangeConstrainer<double, 0, 42>>> start;
                CachedValue<Colour> colour;
            };

            ValueTree clipState (IDs::CLIP);
            CachedValueClip c (clipState);

            c.start = 0.0;
            DBG("start: " << c.start.get());

            c.start = 10.0;
            DBG("start: " << c.start.get());

            c.start = 43.0;
            DBG("start: " << c.start.get());

            c.start = -1.0;
            DBG("start: " << c.start.get());
        }

        {
            struct NumberConstrainer
            {
                static String constrain (const String& v)    { return v.retainCharacters ("0123456789"); }
            };

            struct LetterConstrainer
            {
                static String constrain (const String& v)
                {
                    MemoryOutputStream os;
                    auto p = v.getCharPointer();

                    do
                    {
                        if (p.isLetter())
                            os << String::charToString (*p);
                    }
                    while (p.getAndAdvance());

                    return os.toString();
                }
            };

            ValueTree c (IDs::CLIP);
            CachedValue<ConstrainerWrapper<String, LetterConstrainer>> name;
            name.referTo (c, IDs::name, nullptr);
            name = "He110 W0r1d";
            DBG("name: " << name.get());
        }
    }
}

#define RUN_EXAMPLE(ex) \
std::cout << "//==============================================================================\n"; \
std::cout << "//  " << JUCE_STRINGIFY(ex) << "\n"; \
std::cout << "//==============================================================================\n"; \
ex(); \
std::cout << "\n\n";

//==============================================================================
int main (int, char**)
{
    ScopedJuceInitialiser_GUI juce;

    using namespace examples;
    RUN_EXAMPLE(varExample)
    RUN_EXAMPLE(identifierExample);
    RUN_EXAMPLE(valueTreeExample);
    RUN_EXAMPLE(valueTreeReferenceCountingExample);
    RUN_EXAMPLE(valueTreeUndoRedo);
    RUN_EXAMPLE(valueExample);
    RUN_EXAMPLE(customValueSource);
    RUN_EXAMPLE(serialisationTypes);
    RUN_EXAMPLE(timetstretchOptions);
    RUN_EXAMPLE(stronglyTypedObjects);
    RUN_EXAMPLE(stronglyTypedObjectsWithCachedValues);
    RUN_EXAMPLE(valueTreeObjectList);
    RUN_EXAMPLE(referenceCountedValueTreeObjectList);
    RUN_EXAMPLE(atomicCachedValue);
    RUN_EXAMPLE(rangedCachedValue);

    return 0;
}
