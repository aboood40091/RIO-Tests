#include <gfx/lyr/rio_Renderer.h>
#include <gfx/rio_Camera.h>
#include <gfx/rio_Projection.h>
#include <gfx/rio_Window.h>
#include <math/rio_Matrix.h>
#include <math/rio_Vector.h>
#include <task/rio_TaskMgr.h>

#include "model.h"

struct ViewBlock
{
    rio::BaseVec3f viewPos;  u32 _padding;
    rio::Matrix44f viewProj;
};

struct LightBlock
{
    rio::BaseVec3f lightColor; u32 _padding_0;
    rio::BaseVec3f lightPos;   u32 _padding_1;
};

class RootTask : public rio::ITask, public rio::lyr::IDrawable
{
    enum LayerOrder
    {
        Layer_Order_Front  = 0,
        Layer_Order_Middle,
        Layer_Order_Back
    };

    RootTask()
        : rio::ITask("RootTask")
        , mModel(nullptr)
        , mCoinModel1(nullptr)
        , mCoinModel2(nullptr)
        , mCoinModel3(nullptr)
        , mCoinModel4(nullptr)
        , mViewUniformBlock(rio::UniformBlock::STAGE_ALL, 1)
        , mLightUniformBlock(rio::UniformBlock::STAGE_FRAGMENT_SHADER, 3)
    {
    }

    virtual ~RootTask()
    {
    }

    void prepare_() override;
    void enter_() override;
    void exit_() override;
    void calc_() override;

    void drawFront(const rio::lyr::DrawInfo&);
    void drawMiddle(const rio::lyr::DrawInfo&);
    void drawBack(const rio::lyr::DrawInfo&);

    rio::Window* mWindow;

    rio::lyr::Layer::iterator mLayerFrontIt;
    rio::lyr::Layer* mLayerFront;
    rio::lyr::Layer::iterator mLayerMiddleIt;
    rio::lyr::Layer* mLayerMiddle;
    rio::lyr::Layer::iterator mLayerBackIt;
    rio::lyr::Layer* mLayerBack;

    rio::LookAtCamera mCamera;
    rio::PerspectiveProjection mProjection;
    rio::Matrix44f mViewProj;

    f32 mCounter;

    rio::Vector3f mLightColor;
    rio::Vector3f mLightPos;

    Model* mModel;
    Model* mCoinModel1;
    Model* mCoinModel2;
    Model* mCoinModel3;
    Model* mCoinModel4;

    rio::UniformBlock mViewUniformBlock;
    rio::UniformBlock mLightUniformBlock;

    ViewBlock* mViewBlock;
    LightBlock* mLightBlock;

    friend class rio::TaskMgr;
};
