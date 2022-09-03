#include "roottask.h"

#include <rio.h>

int main()
{
    // Initialize RIO with root task
    if (!rio::Initialize<RootTask>())
        return false;

    // Main loop
    rio::EnterMainLoop();

    // Exit RIO
    rio::Exit();

    return 0;
}
