#include "roottask.h"

#include <gfx/rio_PrimitiveRenderer.h>
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
    // Load background texture
    mBackTex = new rio::Texture2D("bg");

    // Create our layers
    // (Layer order is independent of insertion order)
    mLayers[LAYER_BACK].it  = rio::lyr::Renderer::instance()->addLayer("back",  LAYER_BACK);
    mLayers[LAYER_FRONT].it = rio::lyr::Renderer::instance()->addLayer("front", LAYER_FRONT);

    // Initialize back layer
    mLayers[LAYER_BACK].ptr = &const_cast<rio::lyr::Layer&>(*(mLayers[LAYER_BACK].it));
    // Clear color buffer before drawing layer
    mLayers[LAYER_BACK].ptr->setClearColor({ 0.25f, 0.5f, 0.75f, 1.0f });
    // Clear depth-stencil buffer before drawing layer
    mLayers[LAYER_BACK].ptr->setClearDepthStencil();
    // Add render step and draw method
    mLayers[LAYER_BACK].ptr->addRenderStep("Back");
    mLayers[LAYER_BACK].ptr->addDrawMethod(0, rio::lyr::DrawMethod(this, &RootTask::drawBack));

    // Initialize front layer
    mLayers[LAYER_FRONT].ptr = &const_cast<rio::lyr::Layer&>(*(mLayers[LAYER_FRONT].it));
    // Clear depth buffer before drawing layer
    mLayers[LAYER_FRONT].ptr->setClearDepth();
    // Add render step and draw method
    mLayers[LAYER_FRONT].ptr->addRenderStep("Front");
    mLayers[LAYER_FRONT].ptr->addDrawMethod(0, rio::lyr::DrawMethod(this, &RootTask::drawFront));

    // Set projection
    {
        // Get window instance
        const rio::Window* const window = rio::Window::instance();

        // Initialize perspective projection instance
        mFrontProjection.set(
            0.1f,
            100.0f,
            rio::Mathf::deg2rad(45),
            f32(window->getWidth()) / f32(window->getHeight())
        );
    }

    // Set front layer camera and projection
    mLayers[LAYER_FRONT].ptr->setCamera(&mFrontCamera);
    mLayers[LAYER_FRONT].ptr->setProjection(&mFrontProjection);

    // Create view uniform block instance
    mpViewUniformBlock = new rio::UniformBlock();
    // Set base data for view uniform block
    mpViewUniformBlock->setData(&sViewBlock, sizeof(Model::ViewBlock));

    // Set light color
    mLightColor.set(
        0xff / 255.f,
        0xe8 / 255.f,
        0xd8 / 255.f
    );
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
    // Move camera
    mFrontCamera.pos().set(
        std::sin(mCounter) * 10,
        0.0f,
        std::cos(mCounter) * 10
    );
    mCounter += 1.f / 60;
}

void RootTask::exit_()
{
    delete mpLightUniformBlock;

    for (s32 i = 0; i < cModelNum; i++)
        delete mpModel[i];

    delete mpViewUniformBlock;

    delete mBackTex;
}

void RootTask::drawFront(const rio::lyr::DrawInfo& draw_info)
{
    const rio::LookAtCamera& camera = *static_cast<const rio::LookAtCamera*>(draw_info.parent_layer.camera());
    const rio::Projection& projection = *draw_info.parent_layer.projection();

    rio::PrimitiveRenderer* const primitive_renderer = rio::PrimitiveRenderer::instance();

    // Set primitive renderer projection
    primitive_renderer->setProjection(projection);

    // Set primitive renderer camera
    primitive_renderer->setCamera(camera);

    // Get view matrix
    rio::Matrix34f view_mtx;
    camera.getMatrix(&view_mtx);

    // Get projection matrix
    const rio::Matrix44f& proj_mtx = static_cast<const rio::Matrix44f&>(projection.getMatrix());

    // Calculate view-projection matrix (Projection x View)
    rio::Matrix44f view_proj_mtx;
    view_proj_mtx.setMul(proj_mtx, view_mtx);

    // Update view uniform block
    sViewBlock.view_pos = camera.pos();
    sViewBlock.view_proj_mtx = view_proj_mtx;
    mpViewUniformBlock->setSubDataInvalidate(&sViewBlock, 0, sizeof(Model::ViewBlock));

    // Restore default GPU render state
    rio::RenderState render_state;
    render_state.apply();

    // Draw models
    for (s32 i = 0; i < cModelNum; i++)
        mpModel[i]->draw(*mpViewUniformBlock, *mpLightUniformBlock);

    // Draw light source
    primitive_renderer->begin();
    {
        primitive_renderer->drawSphere8x16(
            mLightPos, 0.125f,
            rio::Color4f{ mLightColor.x, mLightColor.y, mLightColor.z, 1.0f }
        );
    }
    primitive_renderer->end();
}

void RootTask::drawBack(const rio::lyr::DrawInfo& draw_info)
{
    const rio::Camera& camera = *draw_info.parent_layer.camera();
    const rio::Projection& projection = *draw_info.parent_layer.projection();

    rio::PrimitiveRenderer* const primitive_renderer = rio::PrimitiveRenderer::instance();
    const rio::Window* const window = rio::Window::instance();

    // Set primitive renderer projection
    primitive_renderer->setProjection(projection);

    // Set primitive renderer camera
    primitive_renderer->setCamera(camera);

    // Draw background texture
    primitive_renderer->begin();
    {
        primitive_renderer->drawQuad(
            *mBackTex,
            rio::PrimitiveRenderer::QuadArg()
                .setSize({ f32(window->getWidth()), f32(window->getHeight()) })
        );
    }
    primitive_renderer->end();
}
