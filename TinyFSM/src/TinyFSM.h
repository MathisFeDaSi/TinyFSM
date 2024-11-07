#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "State.h"

#include <Arduino.h>

namespace namespaceTinyFSM
{
#define EVENT_QUEUE_SIZE 16
#define INFINITE_LOOP_QUEUE_THRESHOLD 16
#define INFINITE_LOOP_INITIAL_THRESHOLD 16
#define INFINITE_LOOP_PARENT_THRESHOLD 16

    class TinyFSM
    {
    public:
        TinyFSM(unsigned int stateCount = 1);
        ~TinyFSM();
        // states
        void setState(unsigned int state, unsigned int parent = STATE_ROOT);
        void setInitial(unsigned int state, unsigned int initialState);
        void addTransition(unsigned int state, unsigned int event, unsigned int transitionState);
        // events
        void addEvent(unsigned int event);
        void readQueue();
        // activation
        void start();
        unsigned int getCurrentState();

    private:
        // states
        State **_states;
        unsigned int _stateCount;
        // events
        unsigned int *_eventQueue;
        unsigned short _eventQueueStart;
        unsigned short _eventQueueEnd;
        // activation
        unsigned int _currentState;

        // states
        void activateState(unsigned int state, unsigned int infiniteLoop = 1);
        // events
        bool popEvent(unsigned int *event);
        bool switchEvent(unsigned int state, unsigned int event, unsigned int infiniteLoop = 1);
    };
}

using namespaceTinyFSM::TinyFSM;

#endif
