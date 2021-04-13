/*
 * Input Base class
 *
 * The abstract class that specific inputs use for shared functionality.
 * Allows for easy input dispatching while allowing a wide range of inputs to be used.
 *
 * (c) Daniel Cooper 2021
 */

#ifndef INPUT_BASE_INCLUDED
#define INPUT_BASE_INCLUDED

#include <functional>
/*
 * Event details is populated by the type of input
 * 
 * Should return for true if the event should propagate, false for no propagation.
 */
typedef bool (*inputCallbackProvided) (void*, float);
typedef std::function<const bool(void* eventDetails, float dt)> inputCallback;

//A placeholder class to allow the input base to have a pointer to the arbiter
class InputArbiter;

class InputBase {
    protected:

    typedef struct inputStackElement_s {
        inputCallback cb;
        int id;
        inputStackElement_s* nextElem;
    } InputStackElement;

    InputStackElement* stackTop;

    InputArbiter* arbiter;

    inline InputBase(InputArbiter* arbiter) : arbiter(arbiter) { this->stackTop = nullptr; }

    //Pushes the callback to the top of the stack
    int stackPush(inputCallback cb);

    //Unlike normal stack operations, this can remove from anywhere in the stack, not just the top
    //This is because otherwise the only way to guarantee that a detach correctly pops the right
    //callback is if it is called from the callback itself, since that would guarantee that it is
    //the top of the stack.
    //However, in GUIs for example, a button could change the action of another input when there could
    //be something on top of the target callback to remove, which would cause the wrong one to be popped.
    void stackPop(int id);

    public:

    /**
     * The method called to attach a callback to the input
     * The callbacks are represented with a stack, so that the most
     * recently registered callback is the one dispatched.
     *
     * Callback is a function of type inputCallback
     * context can be null, and if non-null, should point towards the object
     * pointer the function needs.
     *
     * Returns the id of the callback registration, used with detach
     */    
    int attach(inputCallback callback);

    /*
     * The method to remove a callback from the call stack
     */
    void detach(int id);

    /**
     * The method the input arbiter invokes to poll for events to dispatch
     * The dispatch location should be the top callback on the stack.
     */
    virtual void poll(float dt) = 0;
};

#endif //INPUT_BASE_INCLUDED