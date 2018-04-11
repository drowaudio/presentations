/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

  name:             ADMeetup_TaskRunner
  version:          0.1
  vendor:           dRowAudio
  website:          www.github.com/drowaudio

  dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics, juce_gui_basics
  exporters:        xcode_mac

  type:             Component
  mainClass:        MyComponent

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once

#define EXAMPLE_1 1
#define EXAMPLE_2 0
#define EXAMPLE_3 0

#define ASSERT_MESSAGE_THREAD \
jassert (MessageManager::getInstance()->isThisTheMessageThread());

//==============================================================================
/**
    Runs a number of tasks sequentially on a background thread.
*/
struct TaskRunner  : private Thread
{
    /** Creates a TaskRunner with an optional thread name. */
    TaskRunner (const String& threadName = String());

    /** Destructor. */
    ~TaskRunner();

    /** Adds a task to be run on a background thread. */
    void addTask (std::function<void()>);

private:
    CriticalSection tasksLock;
    std::vector<std::unique_ptr<std::function<void()>>> tasks;

    std::unique_ptr<std::function<void()>> getNextTask();
    void run() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TaskRunner)
};

//==============================================================================
TaskRunner::TaskRunner (const String& threadName)
    : Thread (threadName)
{
}

TaskRunner::~TaskRunner()
{
    stopThread (5000);
}

void TaskRunner::addTask (std::function<void()> task)
{
    {
        const ScopedLock sl (tasksLock);
        tasks.push_back (std::make_unique<std::function<void()>> (std::move (task)));
    }

    startThread (1);
    notify();
}

std::unique_ptr<std::function<void()>> TaskRunner::getNextTask()
{
    const ScopedLock sl (tasksLock);

    if (tasks.empty())
        return nullptr;

    auto task = std::move (tasks.front());
    tasks.erase (tasks.begin());

    return task;
}

void TaskRunner::run()
{
    while (! threadShouldExit())
    {
        if (auto task = getNextTask())
            (*task)();
        else
            wait (1000);
    }
}

//==============================================================================
/** Simulates the running of a long but low priority task. */
void runTenSecondTask (double& progress)
{
    const double durationInSeconds = 10.0;
    const auto startTime = Time::getCurrentTime();
    const auto timeToEndAt = startTime + RelativeTime::seconds (durationInSeconds);

    for (;;)
    {
        const auto currentTime = Time::getCurrentTime();
        progress = jlimit (0.0, 1.0, (currentTime - startTime).inSeconds() / durationInSeconds);

        if (Thread::currentThreadShouldExit())
            break;

        if (currentTime > timeToEndAt)
            break;

        Thread::sleep (100);
    }
}

/** Simulates the running of a long but low priority task. */
void runTenSecondTask (std::function<bool (double)> updateProgressFn)
{
    const double durationInSeconds = 10.0;
    const auto startTime = Time::getCurrentTime();
    const auto timeToEndAt = startTime + RelativeTime::seconds (durationInSeconds);

    for (;;)
    {
        const auto currentTime = Time::getCurrentTime();

        if (! updateProgressFn (jlimit (0.0, 1.0, (currentTime - startTime).inSeconds() / durationInSeconds)))
            break;

        if (Thread::currentThreadShouldExit())
            break;

        if (currentTime > timeToEndAt)
            break;

        Thread::sleep (100);
    }
}

//==============================================================================
struct Progress
{
    Progress (String nameToUse)
        : name (std::move (nameToUse))
    {
    }

    String getName() const
    {
        return name;
    }

    void setProgress (double newProgress) noexcept
    {
        jassert (isPositiveAndNotGreaterThan (newProgress, 1.0));
        progress.store (newProgress);
    }

    double getProgress() const noexcept
    {
        return progress;
    }

    void cancel() noexcept
    {
        cancelled = true;
    }

    bool hasBeenCancelled() const noexcept
    {
        return cancelled;
    }

private:
    const String name;
    std::atomic<double> progress { 0.0 };
    std::atomic<bool> cancelled { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Progress)
};

//==============================================================================
struct ProgressList : public ChangeBroadcaster,
                      private Timer
{
    //==============================================================================
    ProgressList() = default;

    std::shared_ptr<Progress> createProgress (const String& name)
    {
        ASSERT_MESSAGE_THREAD
        auto progress = std::make_shared<Progress> (name);
        progresses.push_back (progress);
        startTimer (100);
        sendChangeMessage();

        return progress;
    }

    double getTotalProgress() const
    {
        ASSERT_MESSAGE_THREAD
        if (progresses.empty())
            return 1.0;

        double total = 0.0;
        int count = 0;

        for (auto p : progresses)
        {
            const double progress = p->getProgress();

            if (progress < 0.0)
                return -1.0;

            total += jlimit (0.0, 1.0, progress);
            ++count;
        }

        return total / count;
    }

    std::vector<std::shared_ptr<Progress>> getProgresses() const
    {
        ASSERT_MESSAGE_THREAD
        return progresses;
    }

private:
    std::vector<std::shared_ptr<Progress>> progresses;

    void timerCallback() override
    {
        removeExpiredProgresses();

        if (progresses.size() == 0)
            stopTimer();
    }

    void removeExpiredProgresses()
    {
        ASSERT_MESSAGE_THREAD
        const auto sizeBefore = progresses.size();
        progresses.erase (remove_if (begin (progresses), end (progresses),
                                     [] (auto p)
                                     {
                                         return p->hasBeenCancelled() || p->getProgress() >= 1.0;
                                     }),
                          end (progresses));

        if (sizeBefore > progresses.size())
            sendChangeMessage();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProgressList)
};

#if EXAMPLE_1
//==============================================================================
class MyComponent   : public Component
{
public:
    //==============================================================================
    MyComponent()
    {
        statusLabel.setJustificationType (Justification::centred);
        addAndMakeVisible (statusLabel);
        addAndMakeVisible (progressBar);
        setSize (600, 400);

//        taskRunner.addTask ([this]
//                            {
//                                runTenSecondTask (progress);
//
//                                // Attempt 1: 'this' may have been deleted and not ok
//                                //            to call from a background thread!
//                                displayTime (Time::getCurrentTime());
//                            });
//
//        taskRunner.addTask ([sp = SafePointer<MyComponent> (this)] () mutable
//                            {
//                                runTenSecondTask (sp->progress);
//
//                                // Attempt 2: Can't call through sp as it my be deleted after the nullptr check!
//                                if (sp != nullptr)
//                                    sp->displayTime (Time::getCurrentTime());
//                            });
//
//        taskRunner.addTask ([this]
//                            {
//                                runTenSecondTask (progress);
//
//                                // Attempt 3: Ok as long as the taskRunner is guaranteed to outlive the this (MyComponent) pointer!
//                                MessageManager::getInstance()->callAsync ([this] { displayTime (Time::getCurrentTime()); });
//                            });

        taskRunner.addTask ([sp = SafePointer<MyComponent> (this)] () mutable
                            {
                                // Technically this still isn't safe as sp->progress could be dangling which we've passed on to our task!
                                // We'll see how to improve on this in the next example
                                runTenSecondTask (sp->progress);

                                // Attempt 4: This is safe as SafePointer uses a ReferenceCountedObject as it's ref count so is atomic
                                MessageManager::getInstance()->callAsync ([sp] () mutable
                                                                          {
                                                                              if (sp != nullptr)
                                                                                  sp->displayTime (Time::getCurrentTime());
                                                                          });
                            });
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (5);
        statusLabel.setBounds (r.removeFromTop (50));
        progressBar.setBounds (r.withSizeKeepingCentre (r.getWidth(), 40));
    }

private:
    //==============================================================================
    TaskRunner taskRunner;
    double progress = 1.0;
    ProgressBar progressBar { progress };

    Label statusLabel;

    void displayTime (Time time)
    {
        ASSERT_MESSAGE_THREAD
        statusLabel.setText (time.toString (true, true), dontSendNotification);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyComponent)
};
#endif // EXAMPLE_1

#if EXAMPLE_2
//==============================================================================
class MyComponent   : public Component,
                      private ChangeListener,
                      private Timer,
                      private ListBoxModel
{
public:
    //==============================================================================
    MyComponent()
    {
        statusLabel.setJustificationType (Justification::centred);
        addAndMakeVisible (statusLabel);
        addAndMakeVisible (box);
        addAndMakeVisible (totalProgressBar);
        addAndMakeVisible (addTaskButton);

        addTaskButton.onClick = [this]
                                {
                                    auto progress = progressList.createProgress ("Task " + String (++numTasksStarted));
                                    Thread::launch ([sp = SafePointer<MyComponent> (this), progress] () mutable
                                                    {
                                                        runTenSecondTask ([progress] (double p)
                                                                          {
                                                                              progress->setProgress (p);
                                                                              return ! progress->hasBeenCancelled();
                                                                          });

                                                        MessageManager::getInstance()->callAsync ([sp] () mutable
                                                                                                  {
                                                                                                      if (sp != nullptr)
                                                                                                          sp->taskCompleted();
                                                                                                  });
                                                    });
                                };

        progressList.addChangeListener (this);
        updateStatusLabel();

        setSize (600, 400);
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (5);
        statusLabel.setBounds (r.removeFromTop (50));
        totalProgressBar.setBounds (r.removeFromTop (30));
        addTaskButton.setBounds (r.removeFromBottom (30).withSizeKeepingCentre (120, 30));
        box.setBounds (r.reduced (0, 5));
    }

private:
    //==============================================================================
    ProgressList progressList;
    int numTasksStarted = 0, numTasksCompleted = 0;

    double progress = 1.0;
    ProgressBar totalProgressBar { progress };

    Label statusLabel;
    ListBox box { {}, this };
    TextButton addTaskButton { "Add Task" };

    void updateStatusLabel()
    {
        statusLabel.setText ("Tasks completed: " + String (numTasksCompleted), dontSendNotification);
    }

    void updateProgressAndList()
    {
        progress = progressList.getTotalProgress();
        box.repaint();
    }

    void taskCompleted()
    {
        ++numTasksCompleted;
        updateStatusLabel();
    }

    void changeListenerCallback (ChangeBroadcaster*) override
    {
        if (progressList.getProgresses().empty())
            stopTimer();
        else
            startTimerHz (25);

        updateProgressAndList();
        box.updateContent();
    }

    void timerCallback() override
    {
        updateProgressAndList();
    }

    int getNumRows() override
    {
        return (int) progressList.getProgresses().size();
    }

    void paintListBoxItem (int row, Graphics& g, int w, int h, bool isSelected) override
    {
        auto progresses = progressList.getProgresses();

        if (row >= progresses.size())
            return;

        if (auto progress = progresses[row])
        {
            AttributedString s;
            s.setFont (14.0f);
            s.append  (progress->getName() + ": ", progress->hasBeenCancelled() ? Colours::red : Colours::white);
            s.append  (String (progress->getProgress() * 100.0) + "%", Colours::grey);
            s.draw (g, Rectangle<int> { w, h, }.reduced (2).toFloat());
        }
    }

    void listBoxItemClicked (int row, const MouseEvent&) override
    {
        auto progresses = progressList.getProgresses();

        if (row >= progresses.size())
            return;

        if (auto progress = progresses[row])
            progress->cancel();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyComponent)
};
#endif


#if EXAMPLE_3
//==============================================================================
class MyComponent   : public Component,
                      private ChangeListener,
                      private Timer,
                      private ListBoxModel
{
public:
    //==============================================================================
    MyComponent()
    {
        statusLabel.setJustificationType (Justification::centred);
        addAndMakeVisible (statusLabel);
        addAndMakeVisible (box);
        addAndMakeVisible (totalProgressBar);
        addAndMakeVisible (addTaskButton);

        addTaskButton.onClick = [this]
                                {
                                    auto progress = progressList.createProgress ("Task " + String (++numTasksStarted));
                                    threadPool.addJob ([sp = SafePointer<MyComponent> (this), progress] () mutable
                                                       {
                                                           runTenSecondTask ([progress] (double p)
                                                                             {
                                                                                 progress->setProgress (p);
                                                                                 return ! progress->hasBeenCancelled();
                                                                             });

                                                           MessageManager::getInstance()->callAsync ([sp] () mutable
                                                                                                     {
                                                                                                         if (sp != nullptr)
                                                                                                             sp->taskCompleted();
                                                                                                     });
                                                       });
                                };

        progressList.addChangeListener (this);
        updateStatusLabel();

        setSize (600, 400);
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (5);
        statusLabel.setBounds (r.removeFromTop (50));
        totalProgressBar.setBounds (r.removeFromTop (30));
        addTaskButton.setBounds (r.removeFromBottom (30).withSizeKeepingCentre (120, 30));
        box.setBounds (r.reduced (0, 5));
    }

private:
    //==============================================================================
    ThreadPool threadPool;
    ProgressList progressList;
    int numTasksStarted = 0, numTasksCompleted = 0;

    double progress = 1.0;
    ProgressBar totalProgressBar { progress };

    Label statusLabel;
    ListBox box { {}, this };
    TextButton addTaskButton { "Add Task" };

    void updateStatusLabel()
    {
        statusLabel.setText ("Tasks completed: " + String (numTasksCompleted), dontSendNotification);
    }

    void updateProgressAndList()
    {
        progress = progressList.getTotalProgress();
        box.repaint();
    }

    void taskCompleted()
    {
        ++numTasksCompleted;
        updateStatusLabel();
    }

    void changeListenerCallback (ChangeBroadcaster*) override
    {
        if (progressList.getProgresses().empty())
            stopTimer();
        else
            startTimerHz (25);

        updateProgressAndList();
        box.updateContent();
    }

    void timerCallback() override
    {
        updateProgressAndList();
    }

    int getNumRows() override
    {
        return (int) progressList.getProgresses().size();
    }

    void paintListBoxItem (int row, Graphics& g, int w, int h, bool isSelected) override
    {
        auto progresses = progressList.getProgresses();

        if (row >= progresses.size())
            return;

        if (auto progress = progresses[row])
        {
            AttributedString s;
            s.setFont (14.0f);
            s.append  (progress->getName() + ": ", progress->hasBeenCancelled() ? Colours::red : Colours::white);
            s.append  (String (progress->getProgress() * 100.0) + "%", Colours::grey);
            s.draw (g, Rectangle<int> { w, h, }.reduced (2).toFloat());
        }
    }

    void listBoxItemClicked (int row, const MouseEvent&) override
    {
        auto progresses = progressList.getProgresses();

        if (row >= progresses.size())
            return;

        if (auto progress = progresses[row])
            progress->cancel();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyComponent)
};
#endif
