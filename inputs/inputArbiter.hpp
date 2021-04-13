

#ifndef INPUT_ARBITER_INCLUDED
#define INPUT_ARBITER_INCLUDED

#include "inputBase.hpp"
#include <algorithm>
#include <list>

class InputArbiter {
    private:

    int currentId;
    std::list<InputBase*> inputs;

    public:

    inline InputArbiter() : currentId(0) {};

    inline void AddInput(InputBase* input) { this->inputs.push_back(input); }

    inline int getNextId() { return ++(this->currentId); }

    inline void pollInputs(float dt) {
        std::for_each(this->inputs.begin(), this->inputs.end(), [&dt](InputBase* input) { input->poll(dt); });
    }
};

#endif //INPUT_ARBITER_INCLUDED