#include <gfx/rio_Camera.h>
#include <gpu/rio_Texture.h>
#include <task/rio_Task.h>

class RootTask : public rio::ITask
{
public:
    RootTask();

private:
    void prepare_() override;
    void calc_() override;
    void exit_() override;

private:
    rio::Texture2D*     mpTexture;

    rio::LookAtCamera   mCamera;
    f32                 mCounter;

    // Number of cubes
    static constexpr s32 cCubeNum = 10;
    // Cube positions
    static const rio::Vector3f cCubePos[cCubeNum];
};
