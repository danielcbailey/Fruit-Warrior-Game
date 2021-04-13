/*
 * Input Base class
 *
 * The abstract class that specific inputs use for shared functionality.
 * Allows for easy input dispatching while allowing a wide range of inputs to be used.
 *
 * (c) Daniel Cooper 2021
 */

#include "inputBase.hpp"
#include "inputArbiter.hpp"
#include "globals.hpp"
#include <functional>

int InputBase::attach(inputCallback callback) {
    
    return this->stackPush(callback);
}

int InputBase::stackPush(inputCallback cb) {
    InputStackElement* newElem = new(InputStackElement);
    newElem->nextElem = this->stackTop;
    newElem->cb = cb;
    newElem->id = this->arbiter->getNextId();

    this->stackTop = newElem;
    return newElem->id;
}

void InputBase::detach(int id) {
    this->stackPop(id);
}

void InputBase::stackPop(int id) {
    InputStackElement* prev = nullptr;
    for (InputStackElement* curr = this->stackTop; curr != nullptr; curr = curr->nextElem) {
        if (curr->id == id) {
            //found the element

            if (prev != nullptr) {
                //Must link the previous and the next to delete the middle
                prev->nextElem = curr->nextElem;
            } else {
                //Is the head node, just has to update the head
                this->stackTop = curr->nextElem;
            }

            //cleaning up the removed element
            delete(curr);
            break;
        }

        prev = curr;
    }
}