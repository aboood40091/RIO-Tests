#include "roottask.h"

#include <gfx/mdl/res/rio_ModelCacher.h>
#include <gfx/rio_PrimitiveRenderer.h>
#include <misc/rio_MemUtil.h>

namespace {

struct Vertex
{
    rio::BaseVec3f pos;
    rio::BaseVec2f texCoord;
    rio::BaseVec3f normal;
};

static const Vertex vertices[] = {
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },
    { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },

    { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },

    { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },

    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },

    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },

    { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
    { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } }
};

}

void RootTask::prepare_()
{
    // Get window and make context current
    mWindow = rio::Window::instance();
    mWindow->makeContextCurrent();

    RIO_LOG("Window Width: %d\n", mWindow->getWidth());
    RIO_LOG("Window Height: %d\n", mWindow->getHeight());

    // Create our layers
    // Layer order is independent of insertion order
    mLayerMiddleIt = rio::lyr::Renderer::instance()->addLayer("middle", Layer_Order_Middle);
    mLayerFrontIt  = rio::lyr::Renderer::instance()->addLayer("front",  Layer_Order_Front);
    mLayerBackIt   = rio::lyr::Renderer::instance()->addLayer("back",   Layer_Order_Back);

    // Initialize back layer
    mLayerBack = &const_cast<rio::lyr::Layer&>(*mLayerBackIt);
    mLayerBack->setClearColor({ 0.25f, 0.5f, 0.75f, 1.0f });
    mLayerBack->setClearDepth();
    mLayerBack->addRenderStep("Back");
    mLayerBack->addDrawMethod(0, rio::lyr::DrawMethod(this, &RootTask::drawBack));

    // Initialize middle layer
    mLayerMiddle = &const_cast<rio::lyr::Layer&>(*mLayerMiddleIt);
    mLayerMiddle->setClearDepth();
    mLayerMiddle->addRenderStep("Middle");
    mLayerMiddle->addDrawMethod(0, rio::lyr::DrawMethod(this, &RootTask::drawMiddle));

    // Initialize front layer
    mLayerFront = &const_cast<rio::lyr::Layer&>(*mLayerFrontIt);
    mLayerFront->setClearDepth();
    mLayerFront->addRenderStep("Front");
    mLayerFront->addDrawMethod(0, rio::lyr::DrawMethod(this, &RootTask::drawFront));

    // Projection
    mProjection.set(
        0.1f,
        100.0f,
        rio::Mathf::deg2rad(45),
        f32(mWindow->getWidth()) / mWindow->getHeight()
    );
    const rio::Matrix44f& proj = static_cast<const rio::Matrix44f&>(mProjection.getMatrix());

    // Set projection for primitive renderer
    rio::PrimitiveRenderer::instance()->setProjection(mProjection);

    // Move view
    rio::Vec3f& cameraPos = mCamera.pos();
    cameraPos.x = rio::Mathf::rsqrt(2) * 7.5f;
    cameraPos.y = rio::Mathf::rsqrt(2) * 7.5f;
    cameraPos.z = rio::Mathf::rsqrt(2) * 7.5f;

    rio::BaseMtx34f view;
    mCamera.getMatrix(&view);

    // Set camera for primitive renderer
    rio::PrimitiveRenderer::instance()->setCamera(mCamera);

    // Calculate Projection x View matrix
    mViewProj.setMul(proj, view);

    mCounter = 0;

    mLightColor.set(1.0f, 1.0f, 1.0f);
    mLightPos.set(0.0f, 3.0f, 3.0f);

    mViewBlock = (ViewBlock*)rio::MemUtil::alloc(sizeof(ViewBlock), rio::Drawer::cUniformBlockAlignment);
    mLightBlock = (LightBlock*)rio::MemUtil::alloc(sizeof(LightBlock), rio::Drawer::cUniformBlockAlignment);

    mViewBlock->viewPos = mCamera.pos();
    mViewBlock->viewProj = mViewProj;
    mViewUniformBlock.setDataInvalidate(mViewBlock, sizeof(ViewBlock));

    mLightBlock->lightColor = mLightColor;
    mLightBlock->lightPos = mLightPos;
    mLightUniformBlock.setDataInvalidate(mLightBlock, sizeof(LightBlock));
}

void RootTask::enter_()
{
    // Load stall model
    rio::mdl::res::Model* res_mdl = rio::mdl::res::ModelCacher::instance()->loadModel("stall", "stall");
    if (res_mdl)
        mModel = new Model(res_mdl);

    else
    {
        RIO_LOG("Failed to load stall model...\n");
    }

    // Load coin model
    rio::mdl::res::Model* coin_res_mdl = rio::mdl::res::ModelCacher::instance()->loadModel("coin", "coin");
    if (coin_res_mdl)
    {
        mCoinModel1 = new Model(coin_res_mdl);
        mCoinModel2 = new Model(coin_res_mdl);
        mCoinModel3 = new Model(coin_res_mdl);
        mCoinModel4 = new Model(coin_res_mdl);
    }
    else
    {
        RIO_LOG("Failed to load coin model...\n");
    }
}

void RootTask::exit_()
{
    if (mModel)
        delete mModel;

    if (mCoinModel1)
    {
        delete mCoinModel1;
        delete mCoinModel2;
        delete mCoinModel3;
        delete mCoinModel4;
    }
}

void RootTask::calc_()
{
    static f32 r_step = 0.01 / 2;
    static f32 g_step = 0.02 / 2;
    static f32 b_step = 0.04 / 2;

    mLightColor.x += r_step;
    mLightColor.y += g_step;
    mLightColor.z += b_step;

    if (mLightColor.x >= 1.0 || mLightColor.x <= 0.0)
    {
        r_step = -r_step;

        if (mLightColor.x > 1.0)
            mLightColor.x = 1.0;
        else if (mLightColor.x < 0.0)
            mLightColor.x = 0.0;
    }

    if (mLightColor.y >= 1.0 || mLightColor.y <= 0.0)
    {
        g_step = -g_step;

        if (mLightColor.y > 1.0)
            mLightColor.y = 1.0;
        else if (mLightColor.y < 0.0)
            mLightColor.y = 0.0;
    }

    if (mLightColor.z >= 1.0 || mLightColor.z <= 0.0)
    {
        b_step = -b_step;

        if (mLightColor.z > 1.0)
            mLightColor.z = 1.0;
        else if (mLightColor.z < 0.0)
            mLightColor.z = 0.0;
    }

    mLightUniformBlock.setSubDataInvalidate(&mLightColor.x, offsetof(LightBlock, lightColor), sizeof(rio::Vector3f));

    rio::Matrix34f model;
    model.makeSRT({ 0.25f, 0.25f, 0.25f },
                  { 0.0f, mCounter * 1.5f, 0.0f },
                  { 0.0f, -0.5f, 0.25f });

    mModel->setModelWorldMtx(model);

    rio::Vec3f coin_rotation = { 0.0f, mCounter * 6.0f, 0.0f };

    // Coin 1
    {
        rio::Matrix34f model;
        model.makeSRT({ 0.75f, 0.75f, 0.75f },
                      coin_rotation,
                      { 0.0f, 3.0f, 0.0f });

        mCoinModel1->setModelWorldMtx(model);
    }
    // Coin 2
    {
        rio::Matrix34f model;
        model.makeSRT({ 0.75f, 0.75f, 0.75f },
                      coin_rotation,
                      { 3.0f, 0.0f, 0.0f });

        mCoinModel2->setModelWorldMtx(model);
    }
    // Coin 3
    {
        rio::Matrix34f model;
        model.makeSRT({ 0.75f, 0.75f, 0.75f },
                      coin_rotation,
                      { 0.0f, -3.0f, 0.0f });

        mCoinModel3->setModelWorldMtx(model);
    }
    // Coin 4
    {
        rio::Matrix34f model;
        model.makeSRT({ 0.75f, 0.75f, 0.75f },
                      coin_rotation,
                      { -3.0f, 0.0f, 0.0f });

        mCoinModel4->setModelWorldMtx(model);
    }

    mCounter += 0.016666666666666666f;
}

void RootTask::drawFront(const rio::lyr::DrawInfo&)
{
    // Disable culling
    rio::RenderState render_state;
    render_state.setCullingMode(rio::Graphics::CULLING_MODE_NONE);
    render_state.apply();

    rio::PrimitiveRenderer::instance()->begin();
    {
        rio::PrimitiveRenderer::instance()->setModelMatrix(rio::Matrix34f::ident);

        rio::PrimitiveRenderer::instance()->drawCube(
            rio::PrimitiveRenderer::CubeArg()
                .setCenter(mLightPos)
                .setSize({ 0.2f, 0.2f, 0.2f })
                .setColor({ mLightColor.x, mLightColor.y, mLightColor.z, 1.0f })
        );
    }
    rio::PrimitiveRenderer::instance()->end();
}

void RootTask::drawMiddle(const rio::lyr::DrawInfo&)
{
    if (mModel)
        mModel->draw(mViewUniformBlock, mLightUniformBlock);

    if (mCoinModel1)
    {
        mCoinModel1->draw(mViewUniformBlock, mLightUniformBlock);
        mCoinModel2->draw(mViewUniformBlock, mLightUniformBlock);
        mCoinModel3->draw(mViewUniformBlock, mLightUniformBlock);
        mCoinModel4->draw(mViewUniformBlock, mLightUniformBlock);
    }
}

void RootTask::drawBack(const rio::lyr::DrawInfo&)
{
    // Nothing here yet
}
