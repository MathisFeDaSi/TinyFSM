#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

namespace namespaceTinyFSM
{
#define BITMASK_SIZE 64 // 64 due to the use of uint64_t in bitmask of events
#define STATE_ROOT 0

    class State
    {
    public:
        State(unsigned int id, unsigned int parent = STATE_ROOT);
        ~State();
        // transitions
        void addTransition(unsigned int event, unsigned int state);
        bool hasEvent(unsigned int event, unsigned int *transition);
        // properties
        unsigned int getParent();
        void setInitial(unsigned int state);
        // activation
        bool activate(unsigned int *initial);
        void desactivate();

    private:
        // transitions
        uint64_t *_events;                // bitmask of events
        unsigned short _maxEventCount;    // count by 1
        unsigned short *_transitions;     // list of transitions
        unsigned int _maxTransitionCount; // count by 64 (TRANSITION_SIZE)
        // properties
        unsigned int _id;
        unsigned int _parent;
        unsigned int _initial;
        bool _hasInitial;
    };
}

#endif
