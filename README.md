# Presentations

This repository holds materials for the various talks and presentations I've given. Links to video recordings where available can be found below.

Best viewed on [Github Pages](https://drowaudio.github.io/presentations)

###### Index:
  - [Can Audio Progamming be Safe?](#can-audio-programming-be-safe)
  - [Catching Real-time Safety Violations](#catching-real-time-safety-violations)
  - [C++ Online 2024: Lessons Learned from Optimising a High Performance Library](#c-online-2024-lessons-learned-from-optimising-a-high-performance-library)
  - [Why you Shouldn't Write a DAW](#why-you-shouldnt-write-a-daw)
  - [Lessons Learned from Optimising a High Performance Library](#lessons-learned-from-optimising-a-high-performance-library)
  - [Tabs vs. Spaces 2022 (Panel)](#tabs-vs-spaces-2022-panel)
  - [Optimising a Real-time Audio Processing Library](#optimising-a-real-time-audio-processing-library)
  - [Tabs vs. Spaces (Panel)](#tabs-vs-spaces-panel)
  - [Introducing Tracktion Graph: A topological processing library for audio](#introducing-tracktion-graph)
  - [Real-time Trade-offs](#real-time-trade-offs)
  - [Real-time 101 - Dave Rowland & Fabian Renn-Giles](#real-time-101---dave-rowland--fabian-renn-giles)
  - [Real-time programming in modern C++ - Dave Rowland & Fabian Renn-Giles](#real-time-programming-in-modern-c---dave-rowland--fabian-renn-giles)
  - [Tracktion Engine](#tracktion-engine)
  - [pluginval - Yeah, but Why Validate Plugins](#pluginval---yeah-but-why-validate-plugins)
  - [A Backgrounder on Background Tasks](#a-backgrounder-on-background-tasks)
  - [Using JUCE value trees and modern C++ to build large scale applications](#using-juce-valuetrees-and-modern-c-to-build-large-scale-applications)
  - [Using Modern C++ with JUCE to Improve Code Clarity](#using-modern-c-with-juce-to-improve-code-clarity)
  - [Using C++11 to Improve Code Clarity- Braced Initialisers](#using-c11-to-improve-code-clarity---braced-initialisers)

### [Can Audio Progamming be Safe?](https://conference.audio.dev/session/2024/can-audio-programming-be-safe/)
##### ADC 2024 - Nov 2024 - [PDF Slides](https://github.com/drowaudio/presentations/blob/master/ADC%202024%20-%20Can%20Audio%20Programming%20be%20Safe%3F/Can%20Audio%20Programming%20be%20Safe%3F.pdf) - [Animated Slides](ADC%202024%20-%20Can%20Audio%20Programming%20be%20Safe%3F/Can%20Audio%20Programming%20be%20Safe%3F/index.html) - [Video - TBD]()

Safety is an increasingly discussed topic in relation to programming and often little or misunderstood. With organisations like the NSA and NIST recommending developers move away from memory unsafe languages (such as C & C++), there is a lot of uncertainty in the air about what this means for current programing practices. With a focus on the audio industry, are we even exposed to any of these risks?

In this talk we look at the different types of safety, including memory and functional safety, and how these relate to security. We see what risks we might be exposed to and the current strategies for mitigating them.

We then look into what possible alternatives to “memory unsafe languages” there are, why they are safer and if they are suitable for audio use. Should we all be migrating to Rust, Hylo, Swift or Cmajor? What can we learn from the approach these languages take?

Finally we look at existing tooling such as static analysers and sanitizers and see if these provide any safety nets for memory unsafe languages. What is the most pragmatic approach to audio development with one eye on the future landscape.


### [Catching Real-time Safety Violations](https://cpponsea.uk/2024/session/catching-real-time-safety-violations)
###### C++ on Sea 2024 - July 2024 - [PDF Slides](https://drowaudio.github.io/presentations/Cpp%20on%20Sea%202024%20-%20Catching%20Real-time%20Safety%20Violations/Cpp%20on%20Sea%202024%20-%20Catching%20Real-time%20Safety%20Violations.pdf) - [Animated Slides](https://drowaudio.github.io/presentations/Cpp%20on%20Sea%202024%20-%20Catching%20Real-time%20Safety%20Violations/Cpp%20on%20Sea%202024%20-%20Catching%20Real-time%20Safety%20Violations/index.html) - [Video](https://youtu.be/n_jeX1s1rkg)
When you’re executing code with real-time requirements (or even performance critical code) you should avoid unbounded calls like locks and system calls (including memory management). Unfortunately neither the C++ standard nor most libraries make any guarantees about how real-time safe various function calls are. Sometimes we can infer this but it’s difficult and error prone.

This talk aims to look at the different strategies we can take to mitigate, or at least minimise, real-time safety violations by calling into unbounded code. It will first look at external tools and how these can be deployed to check running code. Next, code transformations and interposing via the use of libraries will be investigated. Finally there will be a look into some new, currently experimental tools that aim to simplify this process with the flick of a compiler switch.

Across this talk, the performance impacts of various techniques will be discussed as well as how you might deal with multiple threads where only some are considered real-time contexts. This talk aims to give you the tools required to ensure the real-time safety guarantees of your code base.


### [C++ Online 2024: Lessons Learned from Optimising a High Performance Library](https://cpponline.uk/session/lessons-learned-from-optimising-a-high-performance-library/)
###### C++ Online 2024 - Feb 2024 - [PDF Slides](https://drowaudio.github.io/presentations/Cpp%20Online%202024%20-%20Lessons%20Learned%20from%20Optimising%20a%20High%20Performance%20Library/Lessons%20Learned%20from%20Optimising%20a%20High%20Performance%20Library.pdf) - [Animated Slides](https://drowaudio.github.io/presentations/Cpp%20Online%202024%20-%20Lessons%20Learned%20from%20Optimising%20a%20High%20Performance%20Library/Lessons%20Learned%20from%20Optimising%20a%20High%20Performance%20Library/index.html) - [Video](https://youtu.be/c_cXuL_L0Gg)

This talk will take you through optimising the “Tracktion Engine” codebase from a practical perspective. Tracktion Engine is a large, real-time audio library powering many audio apps across desktop, mobile and embedded platforms. Filled with real-world examples and tales of success and failure, this talk will give you the tools and knowledge to approach optimising your own code in a pragmatic and confident way.

First, we’ll cover how to actually measure and compare performance across different platforms, the tools to do this, and how to integrate continuous performance measuring over time into your CI.

Next, we’ll look at the various strategies for identifying areas for optimisation and how these relate to real-world use cases. We’ll look at how CPU instructions and memory usage can influence execution time and how this varies between different hardware platforms.

Finally, we’ll look at some useful tricks and lesser known strategies to discover that sometimes what you’ve been taught doesn’t actually lead to the best results.


### [Why you Shouldn't Write a DAW](https://sched.co/1Pud4)
##### ADC 2023 - Nov 2023 - [PDF Slides](https://github.com/drowaudio/presentations/blob/master/ADC%202023%20-%20Why%20you%20Shouldn't%20Write%20a%20DAW/Why%20you%20Shouldn't%20Write%20a%20DAW.pdf) - [Animated Slides](https://drowaudio.github.io/presentations/ADC%202023%20-%20Why%20you%20Shouldn't%20Write%20a%20DAW/Why%20you%20Shouldn%E2%80%99t%20Write%20a%20DAW/index.html) - [Video](https://youtu.be/GMlnh6_9aTc)

There are surprisingly few DAWs in the music making world, especially when compared to the number of audio plugins on the market. Why is this? Could it be that all the DAWs in existence are perfect and there’s no need for another one? Perhaps there’s another reason…

In this talk we dive behind the UI/UX to take a deeper look at the technology that underpins DAWs. We’ll take a tour of some of the problems they solve, often transparently to the user, and some of the technical concepts they have to navigate in order to keep music makers in the groove.

Finally, we look at what alternatives there might be if you want to build a product that looks a bit like a DAW and why not building from scratch might save you a lot of time and money.


### [Lessons Learned from Optimising a High Performance Library](https://cpponsea.uk/2023/sessions/lessons-learned-from-optimising-a-high-performance-library.html)
###### Cpp on Sea 2023 - June 2023 - [PDF Slides](https://github.com/drowaudio/presentations/blob/master/Cpp%20on%20Sea%202023%20-%20Lessons%20Learned%20from%20Optimising%20a%20High%20Performance%20Library/Lessons%20Learned%20from%20Optimising%20a%20High%20Performance%20Library.pdf) - [Animated Slides](https://drowaudio.github.io/presentations/Cpp%20on%20Sea%202023%20-%20Lessons%20Learned%20from%20Optimising%20a%20High%20Performance%20Library/Lessons%20Learned%20from%20Optimising%20a%20High%20Performance%20Library/index.html) - [Video](https://youtu.be/TEeBcjKZhfg)

This talk will take you through optimising the “Tracktion Engine” codebase from a practical perspective. Tracktion Engine is a large, real-time audio library powering many audio apps across desktop, mobile and embedded platforms. Filled with real-world examples and tales of success and failure, this talk will give you the tools and knowledge to approach optimising your own code in a pragmatic and confident way.

First, we’ll cover how to actually measure and compare performance across different platforms, the tools to do this, and how to integrate continuous performance measuring over time into your CI.

Next, we’ll look at the various strategies for identifying areas for optimisation and how these relate to real-world use cases. We’ll look at how CPU instructions and memory usage can influence execution time and how this varies between different hardware platforms.

Finally, we’ll look at some useful tricks and lesser known strategies to discover that sometimes what you’ve been taught doesn’t actually lead to the best results.


### Tabs vs. Spaces 2022 (Panel)
###### ADC 2022 - Nov 2022 - [Video](https://youtu.be/5N6uO06jL5Y)

A group of opinionated expert programmers will argue over the right and wrong answers to a selection of programming questions which have no right or wrong answers.

We'll aim to cover a wide range of topics such as: use of locks, exceptions, polymorphism, microservices, OOP, functional paradigms, open and closed source, repository methodologies, languages, textual style and tooling.

The aim of the session is to demonstrate that there is often no clear-cut best-practice for many development topics, and to set an example of how to examine problems from multiple viewpoints.


### Optimising a Real-time Audio Processing Library
###### ADC 2022 - Nov 2022 - [Video](https://youtu.be/FpymA7NLNDs) - [PDF Slides](https://github.com/drowaudio/presentations/blob/master/ADC%202022%20-%20Optomising%20a%20Real-time%20Audio%20Processing%20Library/Optimising%20a%20Real-time%20Audio%20Processing%20Library.pdf) - [Animated Slides](https://drowaudio.github.io/presentations/ADC%202022%20-%20Optomising%20a%20Real-time%20Audio%20Processing%20Library/Optimising%20a%20Real-time%20Audio%20Processing%20Library/index.html)

This talk will take you through optimising a codebase intended for real-time use from the most practical perspective. Filled with real-world examples and tales of success and failure this should give attendees the tools and knowledge to approach optimising their own code in a pragmatic and confident way.

First, we’ll cover how to actually measure and compare performance across different platforms, the tools to do this and most importantly how to do this continuously over time with CI. Next, we’ll look at the various strategies for identifying areas for optimisation and how these relate to real-world use cases. We’ll look at tradeoffs between CPU and memory and the environments these may have the most influence over.

Finally, we’ll look at some useful tricks and lesser known strategies and where sometimes what you’ve been taught doesn’t actually lead to the best results.


### Tabs vs. Spaces (Panel)
###### ADC 2021 - Nov 2021 - [Video](https://youtu.be/BTQxodkv5Uw)

A group of opinionated expert programmers will argue over the right and wrong answers to a selection of programming questions which have no right or wrong answers.

We'll aim to cover a wide range of topics such as: use of locks, exceptions, polymorphism, microservices, OOP, functional paradigms, open and closed source, repository methodologies, languages, textual style and tooling.

The aim of the session is to demonstrate that there is often no clear-cut best-practice for many development topics, and to set an example of how to examine problems from multiple viewpoints.


### Introducing Tracktion Graph
###### ADC 2020 - Nov 2020 - [Video](https://youtu.be/Mkz908eP_4g) - [PDF Slides](https://github.com/drowaudio/presentations/blob/master/ADC%202020%20-%20Introducing%20Tracktion%20Graph/Introducing%20Tracktion%20Graph.pdf) - [Animated Slides](https://drowaudio.github.io/presentations/ADC%202020%20-%20Introducing%20Tracktion%20Graph/Introducing%20Tracktion%20Graph/index.html)

Tracktion Graph is a new open source library designed to build and process complex topological graphs of audio and MIDI sources. It was designed to replace the internal processing of the Tracktion Engine framework in order to solve problems such as plugin delay compensation and optimise processing for multi-core CPUs. The resulting library has no dependency on the Tracktion Engine framework and can be used for many generic audio processing applications.

This talk aims to introduce the concept of an audio graph, what the inherent difficulties are and how the Tracktion Graph library solves them. The talk will mainly focus on looking at audio applications as high-level graph abstractions and then introduce the library as a way to realise those abstractions. This should leave the attendee with a broader knowledge of audio graph topologies, how to structure their audio applications and how to utilise Tracktion Graph to make this a quick, painless and safe task.


### Real-time Trade-offs
###### The Audio Programmer Meetup - Feb 2020 - [Content](https://github.com/drowaudio/presentations/blob/master/The%20Audio%20Programmer%20Meetup%20Feb%202020%20-%20Real-time%20Trade-offs/Real-time%20Trade-offs.pdf) - [Video](https://youtu.be/JCNyd1KGjMk) - [Animated Slides](https://drowaudio.github.io/presentations/The%20Audio%20Programmer%20Meetup%20Feb%202020%20-%20Real-time%20Trade-offs/Real-time%20Trade-offs/index.html)

Writing real-time applications can be tricky, usually you obtain real-time guarantees by making sacrifices elsewhere and the costs paid can be subtle.
We’ll take a look at a concrete example of making an existing mechanism real-time safe and what decisions need to made along the way.
Spoiler - there won’t be a right answer but there will be lots of questions!


### Real-time 101 - Dave Rowland & Fabian Renn-Giles
###### ADC 2019 - [Content](https://github.com/drowaudio/presentations/blob/master/ADC%202019%20-%20Real-time%20101/Real-time%20101.pdf) - [Video Pt. 1](https://youtu.be/Q0vrQFyAdWI) - [Video Pt. 2](https://youtu.be/PoZAo2Vikbo) - [Animated Slides](https://drowaudio.github.io/presentations/ADC%202019%20-%20Real-time%20101/Real-time%20101/index.html)

N.B. This is a longer version of the below talk from Meeting C++. We go in to slightly more detail and it is a slower pace so could be more appropriate for those new to the subject.

“Real-time” is a term that gets used a lot, but what does it really mean to different industries? What happens when our “real-time” system doesn’t perform in real-time? And how can we ensure that we don’t get in to this situation?

This talk aims to discuss what we mean by a real-time system, the practices that can be used to try and make sure it stays real-time and in particular how these can be subtly or accidentally abused increasing the risk of violating your real-time constraints.

We’ll take a detailed look at some of the considerations for real-time systems and the costs they involve such as system calls, allocations and priority inversion. Then the common tools in a programmer’s box such as mutexes, condition variables and atomics, how these interact with real-time threads and what costs they can incur.

This talk aims to ensure attendees of all experience levels leave with a solid understanding of the problems in the real-time domain and an overview of the tools commonly used to solve them.


### Real-time programming in modern C++ - Dave Rowland & Fabian Renn-Giles
###### Meeting C++ - Nov 2019 - [Content](https://github.com/drowaudio/presentations/blob/master/MeetingCpp%202019%20-%20Real-time%20101/Real-time%20101/index.html) - [Video](https://youtu.be/ndeN983j_GQ) - [Animated Slides](https://drowaudio.github.io/presentations/MeetingCpp%202019%20-%20Real-time%20101/Real-time%20101/assets/player/KeynoteDHTMLPlayer.html)

N.B. This is a slightly condensed, faster paced version of the above ADC 2019 talk which may be of interest if you're short on time.

“Real-time” is a term that gets used a lot, but what does it really mean to different industries? What happens when our “real-time” system doesn’t perform in real-time? And how can we ensure that we don’t get in to this situation?

This talk is presented in two parts. The first will discuss what we mean by a real-time system, the practices that can be used to try and make sure it stays real-time and in particular how these can be subtly or accidentally abused increasing the risk of violating your real-time constraints. We’ll take a look at some of the considerations for real-time systems and the costs they involve such as system calls, allocations and priority inversion. Then the common tools in a programmer’s box such as mutexes, condition variables and atomics and how these can be deployed effectively. We’ll also look at some common mechanisms built out of these primitives and where they’re most frequently used.

The second part of this talk takes an in-depth look at the difficult problem of synchronization between real-time and non-real-time threads. This talk will share insights, tricks and design patterns, that the author has established over years of real-time audio programming, and has ultimately led to the creation of the open-source farbot library. At the end of this talk, you will be equipped with a set of simple design rules guiding you to the correct solution for various real-time challenges and synchronization situations.


### Tracktion Engine
###### London Audio Developers Meetup - May 2019 - [Content](https://github.com/drowaudio/presentations/blob/master/Audio%20Developer%20Meetup%20May%202019%20-%20Tracktion%20Engine/Audio%20Developer%20Meetup%20May%202019%20-%20Tracktion%20Engine.pdf) - [Video](https://skillsmatter.com/skillscasts/13815-london-audio-developers-the-tracktion-engine-and-interacting-neurons-make-noise)

In November 2018, Tracktion open-sourced their powerful audio engine - the same engine behind their DAW Waveform. Jules and Dave - the authors of Tracktion and Waveform - will give us a tour of the inner working of the engine.

This is a relatively high level overview of the basic Engine principles and goals along with some example projects and discussion around potential use cases.


### pluginval - Yeah, but Why Validate Plugins
###### ADC 2018 - [Content](https://github.com/drowaudio/presentations/tree/master/ADC%202018%20-%20pluginval,%20Yeah,%20but%20Why%20Validate%20Plugins) - [Video](https://www.youtube.com/watch?v=Q97LBXqgMus)

Earlier in the year we released an open source tool “pluginval” for validating audio plugins. The aim of this is to make it easier to automate the testing and validation process of developing plugins in order to catch more bugs, and sooner in the development process. Additionally, this can be used by host developers, and even end users, to check for plugin compatibility with JUCE based hosts. To start I’ll give an overview of pluginval, the architecture, deployment considerations and how everyone can make use of it.

Secondly I’ll discuss some of the cases that inspired pluginval’s creation in the first place. Developing plugins to an API may seem simple but there’s a lot of “grey” in that area. I’ll dig through some of it giving examples of what crazy behaviour both hosts and plugins alike can do and the tests employed by pluginval to ensure you’re not caught out.


### A Backgrounder on Background Tasks
###### London Audio Developers Meetup - April 2018 - [Content](https://github.com/drowaudio/presentations/tree/master/Audio%20Developer%20Meetup%20April%202018) - [Video](https://skillsmatter.com/skillscasts/11632-audio-developers-meet-up-april)

Apps need to be responsive, this means taking time consuming work off off the main thread and doing it in the background. But what’s the best way to go about this? How do you communicate safely back to the message thread when you’re done? How do you inform users of the state of these tasks?

There are a lot of tricky problems to solve when dealing with background tasks. This talk delves in to them, building a task running system from first principles and solving common pitfalls one by one. Once that’s done we’ll look at how to build this system in to a real world app using patterns that are compose-able and reusable.

Using strict types and free functions enables you to write far less code which contains less bloat and is more expressive.


### Using JUCE ValueTrees and Modern C++ to Build Large Scale Applications
###### ADC 2017  - [Content](https://github.com/drowaudio/presentations/tree/master/ADC%202017%20-%20Using%20JUCE%20ValueTrees%20and%20Modern%20C%2B%2B%20to%20Build%20Large%20Scale%20Applications) - [Video](https://youtu.be/3IaMjH5lBEY)

JUCE ValueTrees are a tree based data structure capable of holding free-form data. They have a callback interface for being notified of changes to data members or the tree structure and have undo capability built in. Think XML on steroids!
These features make them an ideal candidate for the data model of many applications.
Building on the ideas presented at last year’s ADC “Using Modern C++ with JUCE to Improve Code Clarity” this talk aims to unwrap the complexities involved in dealing with ValueTrees and expose the elegant nature contained within them, utilising them to build large scale graphical and audio applications quickly.

The talk aims to give an in-depth explanation of ValueTree best practices and how apply them to many coding situations using clear, concise, modern C++. Throughout the talk there will be special emphasis on the reference counted nature of these objects and the synchronous/asynchronous nature of the callback events generated and how these relate to performance, thread safety and debugging. Common application tasks such as serialisation, copy/paste and undo/redo are also explored and how they are simply implemented with ValueTrees. Additionally the talk will look at ways to build your own utility classes on top of ValueTrees where it is sensible to do so. Custom developed classes such as the ValueTreeObjectList are explored which allow type-safe, concrete object creation managed by ValueTree state.

This example-led talk aims to ensure attendees of all experience levels leave with a solid understanding of how to quickly build C++ JUCE applications utilising ValueTrees in a modern, safe and fun way.


### Using Modern C++ with JUCE to Improve Code Clarity
###### ADC 2016 - [Content](https://github.com/drowaudio/presentations/tree/master/ADC%202016%20-%20Using%20Modern%20C%2B%2B%20to%20Improve%20Code%20Clarity) - (Unfortunately this talk was not recorded)

Building on the ideas behind last year’s "Using C\++11 to Improve Code Clarity: Braced Initialisers" of reducing code bloat to improve clarity, performance and robustness, this talk takes a wider look at modern C++ coding features and how to utilise them.

One major aspect of event based, application programming is the notion of "when something happens, do this". C++ can often get in the way of clearly expressing this intent. This talk aims to use modern coding styles, in combination with JUCE classes to more clearly and concisely express this intent.

In particular, a number of possible lambda and std::function applications are demonstrated, ranging from timers and asynchronous callbacks, to drawing methods and general delegation used to reduce dependancies on inheritance. A concise, practical look at std::async is also included with an aim to improve app responsiveness by simply and effectively parallelising intensive areas of code.

However, when transitioning to modern coding styles there can be some pitfalls. This talk also demonstrates some of these pitfalls and explains how to avoid them. It provides good practices for mixing JUCE and C++ code. In particular it looks at the behaviour of auto and type deduction used in conjunction with JUCE smart pointers and when to prefer std alternatives.

This example-led talk aims to introduce new paradigms and increase the number of tools in your toolbox whilst keeping code clear, robust and maintainable. It is not an explanation of how C++11 features work under the hood, but the possibilities they unlock.


### Using C++11 to Improve Code Clarity - Braced Initialisers
###### JUCE Summit 2015 - [Content](https://github.com/drowaudio/presentations/tree/master/JUCE%20Summit%202015%20-%20Using%20C%2B%2B11%20to%20Improve%20Code%20Clarity-%20Braced%20Initialisers) - [Video](https://www.youtube.com/watch?v=SmriQ5zXeAk)

There are a lot of things a programmer has to think about whilst writing code. These include but are not limited to: performance, readability, maintainability, robustness, security, portability, compatibility and scaleability. In order to make writing code simpler to reason about, one solution is to write less code.

C++11/14 provides us with several new tools to reduce boilerplate code so we can more simply and concisely express intent. Some of the major new features are: type deduction, threads and futures, lambdas, function objects, variadic templates, range-­based for loops, object lifetime management and braced initialisers.

The term "braced initialisers" is used here to describe a number of new applications of braces including: aggregate initialisation, member declaration brace­or­equal initialisers, object constructor deduction.

This talk goes through several JUCE based, real­world related examples of simplifying and improving code using the brace-related features. The aim is to not only improve intelligibility but also performance and robustness with less time spent typing and reading.
