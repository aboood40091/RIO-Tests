#include <gfx/rio_Camera.h>
#include <gpu/rio_UniformBlock.h>
#include <math/rio_Matrix.h>
#include <math/rio_Vector.h>
#include <task/rio_Task.h>

#include "model.h"

class RootTask : public rio::ITask
{
public:
    RootTask();

private:
    void prepare_() override;
    void calc_() override;
    void exit_() override;

private:
    rio::Matrix44f      mProjMtx;
    rio::LookAtCamera   mCamera;
    f32                 mCounter;

    rio::Vector3f       mLightColor;
    rio::Vector3f       mLightPos;

    rio::UniformBlock*  mpViewUniformBlock;
    rio::UniformBlock*  mpLightUniformBlock;

    static Model::ViewBlock     sViewBlock;
    static Model::LightBlock    sLightBlock;

    // Number of models
    static constexpr s32 cModelNum = 10;
    // Model positions
    static const rio::Vector3f cModelPos[cModelNum];

    Model* mpModel[cModelNum];
};
