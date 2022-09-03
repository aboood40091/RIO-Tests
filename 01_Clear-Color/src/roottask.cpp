#include "roottask.h"

#include <gfx/rio_Window.h>

RootTask::RootTask()
    : ITask("RootTask")
{
}

void RootTask::prepare_()
{
    mB = (mG = (mR = 0.0f));

    cStepR = 0.01f;
    cStepG = 0.02f;
    cStepB = 0.04f;
}

void RootTask::calc_()
{
    rio::Window::instance()->clearColor(mR, mG, mB, 1.0f);

    mR += cStepR;
    mG += cStepG;
    mB += cStepB;

    if (mR >= 1.0 || mR <= 0.0)
    {
        cStepR = -cStepR;

        if (mR > 1.0)
            mR = 1.0;
        else if (mR < 0.0)
            mR = 0.0;
    }

    if (mG >= 1.0 || mG <= 0.0)
    {
        cStepG = -cStepG;

        if (mG > 1.0)
            mG = 1.0;
        else if (mG < 0.0)
            mG = 0.0;
    }

    if (mB >= 1.0 || mB <= 0.0)
    {
        cStepB = -cStepB;

        if (mB > 1.0)
            mB = 1.0;
        else if (mB < 0.0)
            mB = 0.0;
    }
}
