#include "State.h"

namespace namespaceTinyFSM
{
    State::State(unsigned int id, unsigned int parent)
    {
        _events = new uint64_t[1]();
        _maxEventCount = 1;
        _transitions = new unsigned short[BITMASK_SIZE];
        _maxTransitionCount = BITMASK_SIZE;
        _id = id;
        _parent = parent;
        _hasInitial = false;
    }

    State::~State()
    {
        delete[] _events;
        delete[] _transitions;
    }

    void State::addTransition(unsigned int event, unsigned int state)
    {
        unsigned short ind = event / BITMASK_SIZE;
        if (ind >= _maxEventCount)
        {
            // extend events
            uint64_t *eventsTmp = new uint64_t[ind]();
            for (unsigned short i = 0; i < _maxEventCount; i++)
            {
                eventsTmp[i] = _events[i];
            }
            delete _events;
            _events = eventsTmp;
            _maxEventCount = ind;
            // extend transitions
            unsigned int transitionMaxCountTmp = ind * BITMASK_SIZE;
            unsigned short *transitionsTmp = new unsigned short[transitionMaxCountTmp];
            for (unsigned int i = 0; i < _maxTransitionCount; i++)
            {
                transitionsTmp[i] = _transitions[i];
            }
            delete _transitions;
            _transitions = transitionsTmp;
            _maxTransitionCount = transitionMaxCountTmp;
        }
        _events[ind] |= (1 << (event % BITMASK_SIZE));
        _transitions[event] = state;
    }

    bool State::hasEvent(unsigned int event, unsigned int *transition)
    {
        if (event >= _maxTransitionCount)
        {
            return false;
        }
        unsigned short ind = event / BITMASK_SIZE;
        uint64_t bitmask = _events[ind];
        ind = event % BITMASK_SIZE;
        if (bitmask & (1 << ind))
        {
            *transition = _transitions[event];
            return true;
        }
        return false;
    }

    unsigned int State::getParent()
    {
        return _parent;
    }

    void State::setInitial(unsigned int state)
    {
        if (state == _id)
        {
            Serial.println("WARNING ! The initial state (" + String(state) + ") cannot be set to itself.");
            return;
        }
        _initial = state;
        _hasInitial = true;
    }

    bool State::activate(unsigned int *initial)
    {
        // TODO: add onEntry
        if (_hasInitial)
        {
            *initial = _initial;
            return true;
        }
        return false;
    }

    void State::desactivate()
    {
        // TODO: add onExit
    }
}
