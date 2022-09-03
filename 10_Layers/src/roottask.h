#include <gfx/lyr/rio_Renderer.h>
#include <gfx/rio_Camera.h>
#include <gfx/rio_Projection.h>
#include <gpu/rio_Texture.h>
#include <gpu/rio_UniformBlock.h>
#include <math/rio_Matrix.h>
#include <math/rio_Vector.h>
#include <task/rio_Task.h>

#include "model.h"

class RootTask : public rio::ITask, public rio::lyr::IDrawable
{
public:
    RootTask();

    void drawFront(const rio::lyr::DrawInfo&);
    void drawBack(const rio::lyr::DrawInfo&);

private:
    void prepare_() override;
    void calc_() override;
    void exit_() override;

private:
    enum Layer
    {
        LAYER_FRONT = 0,
        LAYER_BACK,
        LAYER_MAX
    };

    struct
    {
        rio::lyr::Layer::iterator   it;
        rio::lyr::Layer*            ptr;
    } mLayers[LAYER_MAX];

    rio::PerspectiveProjection  mFrontProjection;
    rio::LookAtCamera           mFrontCamera;
    f32                         mCounter;

    rio::Texture2D*             mBackTex;

    rio::Vector3f               mLightColor;
    rio::Vector3f               mLightPos;

    rio::UniformBlock*          mpViewUniformBlock;
    rio::UniformBlock*          mpLightUniformBlock;

    static Model::ViewBlock     sViewBlock;
    static Model::LightBlock    sLightBlock;

    // Number of models
    static constexpr s32 cModelNum = 10;
    // Model positions
    static const rio::Vector3f cModelPos[cModelNum];

    Model* mpModel[cModelNum];
};
