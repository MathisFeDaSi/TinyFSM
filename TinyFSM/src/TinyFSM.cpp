#include "TinyFSM.h"

namespace namespaceTinyFSM
{
    TinyFSM::TinyFSM(unsigned int stateCount)
    {
        if (stateCount < 1)
        {
            Serial.println("WARNING ! The root state must be created.");
            stateCount = 1;
        }
        _states = new State *[stateCount];
        _stateCount = stateCount;
        _eventQueue = new unsigned int[EVENT_QUEUE_SIZE]();
        _eventQueueStart = 0;
        _eventQueueEnd = 0;
        _currentState = STATE_ROOT;
    }

    TinyFSM::~TinyFSM()
    {
        delete[] _eventQueue;
        if (_states)
        {
            for (unsigned int i = 0; i < _stateCount; i++)
            {
                delete _states[i];
            }
            delete[] _states;
        }
    }

    void TinyFSM::setState(unsigned int state, unsigned int parent)
    {
        if (state >= _stateCount)
        {
            Serial.println("State " + String(state) + " couldn't be added...maximum state is " + String(_stateCount - 1) + "(state count is " + String(_stateCount) + ")");
            return;
        }
        _states[state] = new State(state, parent);
        Serial.println("State " + String(state) + " created with state " + String(parent) + " as parent");
    }

    void TinyFSM::setInitial(unsigned int state, unsigned int initialState)
    {
        _states[state]->setInitial(initialState);
        Serial.println("State " + String(state) + " has as initial state " + String(initialState));
    }

    void TinyFSM::addTransition(unsigned int state, unsigned int event, unsigned int transitionState)
    {
        _states[state]->addTransition(event, transitionState);
        Serial.println("Transition from state " + String(state) + " to state " + String(transitionState) + " by event " + String(event) + " added");
    }

    void TinyFSM::addEvent(unsigned int event)
    {
        unsigned short nextInd = _eventQueueEnd + 1;
        if (nextInd >= EVENT_QUEUE_SIZE)
        {
            nextInd = 0;
        }
        if (nextInd == _eventQueueStart)
        {
            Serial.println("WARNING ! The event queue is full.");
            return;
        }
        _eventQueue[_eventQueueEnd] = event;
        _eventQueueEnd = nextInd;
    }

    void TinyFSM::readQueue()
    {
        unsigned int infiniteLoop = 1;
        unsigned int event;
        while (popEvent(&event))
        {
            if (infiniteLoop >= INFINITE_LOOP_QUEUE_THRESHOLD)
            {
                Serial.println("WARNING ! Potential infinite loop detected in queue.");
                return;
            }
            infiniteLoop++;
            switchEvent(_currentState, event);
        }
    }

    void TinyFSM::start()
    {
        activateState(STATE_ROOT);
    }

    unsigned int TinyFSM::getCurrentState()
    {
        return _currentState;
    }

    void TinyFSM::activateState(unsigned int state, unsigned int infiniteLoop)
    {
        if (infiniteLoop >= INFINITE_LOOP_INITIAL_THRESHOLD)
        {
            Serial.println("WARNING ! Potential infinite loop detected in initial.");
            return;
        }
        _currentState = state;
        Serial.println("state " + String(state) + " activated");
        unsigned int initial;
        if (_states[state]->activate(&initial))
        {
            activateState(initial, infiniteLoop + 1);
        }
    }

    bool TinyFSM::popEvent(unsigned int *event)
    {
        if (_eventQueueStart == _eventQueueEnd)
        {
            return false;
        }
        *event = _eventQueue[_eventQueueStart];
        unsigned int nextInd = _eventQueueStart + 1;
        if (nextInd >= EVENT_QUEUE_SIZE)
        {
            nextInd = 0;
        }
        _eventQueueStart = nextInd;
        return true;
    }

    bool TinyFSM::switchEvent(unsigned int state, unsigned int event, unsigned int infiniteLoop)
    {
        if (state == STATE_ROOT)
        {
            return false;
        }
        unsigned int transition;
        if (_states[state]->hasEvent(event, &transition))
        {
            Serial.println("state " + String(state) + " has event " + String(event));
            _states[state]->desactivate();
            activateState(transition);
            return true;
        }
        if (infiniteLoop >= INFINITE_LOOP_PARENT_THRESHOLD)
        {
            Serial.println("WARNING ! Potential infinite loop detected in parent.");
            return false;
        }
        unsigned int parent = _states[state]->getParent();
        if (parent != STATE_ROOT)
        {
            bool hasSwitched = switchEvent(parent, event, infiniteLoop + 1);
            if (hasSwitched)
            {
                _states[state]->desactivate();
                return true;
            }
        }
        return false;
    }
}
