#include "roottask.h"

#include <gfx/rio_PrimitiveRenderer.h>
#include <gfx/rio_Projection.h>
#include <gfx/rio_Window.h>
#include <gfx/mdl/res/rio_ModelCacher.h>
#include <gpu/rio_Drawer.h>
#include <gpu/rio_RenderState.h>
#include <misc/rio_MemUtil.h>

__attribute__((aligned(rio::Drawer::cUniformBlockAlignment))) Model::ViewBlock  RootTask::sViewBlock;
__attribute__((aligned(rio::Drawer::cUniformBlockAlignment))) Model::LightBlock RootTask::sLightBlock;

const rio::Vector3f RootTask::cModelPos[] = {
    {  0.0f,  0.0f,   0.0f },
    {  2.0f,  5.0f, -15.0f },
    { -1.5f, -2.2f,  -2.5f },
    { -3.8f, -2.0f, -12.3f },
    {  2.4f, -0.4f,  -3.5f },
    { -1.7f,  3.0f,  -7.5f },
    {  1.3f, -2.0f,  -2.5f },
    {  1.5f,  2.0f,  -2.5f },
    {  1.5f,  0.2f,  -1.5f },
    { -1.3f,  1.0f,  -1.5f }
};

RootTask::RootTask()
    : ITask("RootTask")
{
}

void RootTask::prepare_()
{
    // Set projection matrix
    {
        // Get window instance
        const rio::Window* const window = rio::Window::instance();

        // Create perspective projection instance
        rio::PerspectiveProjection proj(
            0.1f,
            100.0f,
            rio::Mathf::deg2rad(45),
            f32(window->getWidth()) / f32(window->getHeight())
        );

        // Calculate matrix
        rio::MemUtil::copy(&mProjMtx, &proj.getMatrix(), sizeof(rio::Matrix44f));

        // Set primitive renderer projection
        rio::PrimitiveRenderer::instance()->setProjection(proj);
    }

    // Create view uniform block instance
    mpViewUniformBlock = new rio::UniformBlock();
    // Set base data for view uniform block
    mpViewUniformBlock->setData(&sViewBlock, sizeof(Model::ViewBlock));

    // Set light color
    mLightColor.set(0.625f, 1.0f, 1.0f);
    // Set light position
    mLightPos.set(0.0f, 0.0f, 2.0f);

    // Create light uniform block instance
    mpLightUniformBlock = new rio::UniformBlock();
    // Set light uniform block data and invalidate cache now as it won't be modified
    sLightBlock.light_color = mLightColor;
    sLightBlock.light_pos = mLightPos;
    mpLightUniformBlock->setDataInvalidate(&sLightBlock, sizeof(Model::LightBlock));

    // Load coin model
    rio::mdl::res::Model* coin_res_mdl = rio::mdl::res::ModelCacher::instance()->loadModel("nsmbw_coin", "coin");
    RIO_ASSERT(coin_res_mdl);

    // Create model instances
    for (s32 i = 0; i < cModelNum; i++)
    {
        f32 angle = rio::Mathf::deg2rad(20) * i;

        // Model (local-to-world) matrix
        rio::Matrix34f model_mtx;
        model_mtx.makeRT(
            { angle, angle * 1.f/3.f, angle * 2.f/3.f },
            cModelPos[i]
        );

        // Create model instance
        mpModel[i] = new Model(coin_res_mdl, "cViewBlock", "cLightBlock", "cModelBlock");
        // Set model's local-to-world matrix
        mpModel[i]->setModelWorldMtx(model_mtx);
    }
}

void RootTask::calc_()
{
    rio::Window::instance()->clearColor(0.2f, 0.3f, 0.3f, 1.0f);
    rio::Window::instance()->clearDepthStencil();

    // Move camera
    mCamera.pos().set(
        std::sin(mCounter) * 10,
        0.0f,
        std::cos(mCounter) * 10
    );
    mCounter += 1.f / 60;

    // Get view matrix
    rio::Matrix34f view_mtx;
    mCamera.getMatrix(&view_mtx);

    // Set primitive renderer camera
    rio::PrimitiveRenderer::instance()->setCamera(mCamera);

    // Calculate view-projection matrix (Projection x View)
    rio::Matrix44f view_proj_mtx;
    view_proj_mtx.setMul(mProjMtx, view_mtx);

    // Update view uniform block
    sViewBlock.view_pos = mCamera.pos();
    sViewBlock.view_proj_mtx = view_proj_mtx;
    mpViewUniformBlock->setSubDataInvalidate(&sViewBlock, 0, sizeof(Model::ViewBlock));

    // Restore default GPU render state
    rio::RenderState render_state;
    render_state.apply();

    // Draw models
    for (s32 i = 0; i < cModelNum; i++)
        mpModel[i]->draw(*mpViewUniformBlock, *mpLightUniformBlock);

    rio::PrimitiveRenderer::instance()->begin();
    {
        rio::PrimitiveRenderer::instance()->drawSphere8x16(
            mLightPos, 0.125f,
            rio::Color4f{ mLightColor.x, mLightColor.y, mLightColor.z, 1.0f }
        );
    }
    rio::PrimitiveRenderer::instance()->end();
}

void RootTask::exit_()
{
    delete mpLightUniformBlock;

    for (s32 i = 0; i < cModelNum; i++)
        delete mpModel[i];

    delete mpViewUniformBlock;
}
