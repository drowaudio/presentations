# Presentations

This repository holds materials for the various talks and presentations I've given. Links to video recordings where available can be found below.

###### Index:
  - [Using Modern C++ with JUCE to Improve Code Clarity](#Using-Modern-C++-with-JUCE-to-Improve-Code-Clarity)
  - [Using JUCE value trees and modern C++ to build large scale applications](#Using-JUCE-ValueTrees-and-Modern-C++-to-Build-Large-Scale-Applications)
  - [Using Modern C++ with JUCE to Improve Code Clarity](#Using-Modern-C++-with-JUCE-to-Improve-Code-Clarity)
  - [Using C++11 to Improve Code Clarity- Braced Initialisers](#Using-C++11-to-Improve-Code-Clarity---Braced-Initialisers)

### Using Modern C++ with JUCE to Improve Code Clarity
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
