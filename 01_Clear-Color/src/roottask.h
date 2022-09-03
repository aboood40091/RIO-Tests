#include <task/rio_Task.h>

class RootTask : public rio::ITask
{
public:
    RootTask();

private:
    void prepare_() override;
    void calc_() override;

private:
    f32 mR, mG, mB;
    f32 cStepR, cStepG, cStepB;
};
