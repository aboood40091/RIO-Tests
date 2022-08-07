#include "roottask.h"

#include <controller/rio_ControllerMgr.h>
#include <gfx/rio_Camera.h>
#include <gfx/rio_PrimitiveRenderer.h>
#include <gfx/rio_Projection.h>
#include <gfx/rio_Window.h>
#include <gpu/rio_RenderState.h>

RootTask::RootTask()
    : ITask("RootTask")
{
}

void RootTask::prepare_()
{
    mControllerTexture = new rio::Texture2D("controller_no_buttons");

    mStickTextures[0] = new rio::Texture2D("stick_0");
    mStickTextures[1] = new rio::Texture2D("stick_1");

    mABXYTextures[0] = new rio::Texture2D("abxy_0");
    mABXYTextures[1] = new rio::Texture2D("abxy_1");

    mHomeTextures[0] = new rio::Texture2D("home_0");
    mHomeTextures[1] = new rio::Texture2D("home_1");

    mStartSelectTextures[0] = new rio::Texture2D("start_select_0");
    mStartSelectTextures[1] = new rio::Texture2D("start_select_1");

    mUpTextures[0] = new rio::Texture2D("up_0");
    mUpTextures[1] = new rio::Texture2D("up_1");

    mDownTextures[0] = new rio::Texture2D("down_0");
    mDownTextures[1] = new rio::Texture2D("down_1");

    mLeftTextures[0] = new rio::Texture2D("left_0");
    mLeftTextures[1] = new rio::Texture2D("left_1");

    mRightTextures[0] = new rio::Texture2D("right_0");
    mRightTextures[1] = new rio::Texture2D("right_1");

    mLTextures[0] = new rio::Texture2D("l_0");
    mLTextures[1] = new rio::Texture2D("l_1");

    mRTextures[0] = new rio::Texture2D("r_0");
    mRTextures[1] = new rio::Texture2D("r_1");

    mZLTextures[0] = new rio::Texture2D("zl_0");
    mZLTextures[1] = new rio::Texture2D("zl_1");

    mZRTextures[0] = new rio::Texture2D("zr_0");
    mZRTextures[1] = new rio::Texture2D("zr_1");

    mLayerIt = rio::lyr::Renderer::instance()->addLayer("Layer");
    mLayer = &const_cast<rio::lyr::Layer&>(*mLayerIt);

    mLayer->setClearColor({ 0.25f, 0.5f, 0.5f, 1.0f });
    mLayer->setClearDepth();

    mLayer->addRenderStep("Layer");
    mLayer->addDrawMethod(0, { this, &RootTask::render });

    mModelMtx.makeT({ -406.0f, 320.0f, 0.0f });
}

void RootTask::exit_()
{
    delete mControllerTexture;

    delete mStickTextures[0];
    delete mStickTextures[1];

    delete mABXYTextures[0];
    delete mABXYTextures[1];

    delete mHomeTextures[0];
    delete mHomeTextures[1];

    delete mStartSelectTextures[0];
    delete mStartSelectTextures[1];

    delete mUpTextures[0];
    delete mUpTextures[1];

    delete mDownTextures[0];
    delete mDownTextures[1];

    delete mLeftTextures[0];
    delete mLeftTextures[1];

    delete mRightTextures[0];
    delete mRightTextures[1];

    delete mLTextures[0];
    delete mLTextures[1];

    delete mRTextures[0];
    delete mRTextures[1];

    delete mZLTextures[0];
    delete mZLTextures[1];

    delete mZRTextures[0];
    delete mZRTextures[1];
}

void RootTask::render(const rio::lyr::DrawInfo& draw_info)
{
    rio::RenderState render_state;
    render_state.apply();

    const rio::lyr::Layer& layer = draw_info.parent_layer;
    rio::PrimitiveRenderer* const primitive_renderer = rio::PrimitiveRenderer::instance();

    primitive_renderer->setCamera(*layer.camera());
    primitive_renderer->setProjection(*layer.projection());
    primitive_renderer->setModelMatrix(mModelMtx);

    primitive_renderer->begin();

    rio::Controller* controller = rio::ControllerMgr::instance()->getGamepad(0);
    if (!controller || !controller->isConnected())
    {
        controller = rio::ControllerMgr::instance()->getMainGamepad();
        RIO_ASSERT(controller);
    }

    drawController(*controller);

    primitive_renderer->end();
}

void RootTask::drawController(const rio::Controller& controller) const
{
    rio::PrimitiveRenderer* const primitive_renderer = rio::PrimitiveRenderer::instance();

    primitive_renderer->drawQuad(
        *mControllerTexture,
        rio::PrimitiveRenderer::QuadArg()
            .setCornerAndSize( { 0.0f, -640.0f, 0.0f }, { 812.0f, 640.0f })
    );

    primitive_renderer->drawQuad(
        *mStickTextures[controller.isHold(1 << rio::Controller::PAD_IDX_1)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({   75.0f + controller.getLeftStick().x * 20,
                         -100.0f + controller.getLeftStick().y * 20 })
            .setSize({ 61.0f, 61.0f })
    );

    primitive_renderer->drawQuad(
        *mStickTextures[controller.isHold(1 << rio::Controller::PAD_IDX_2)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({  737.0f + controller.getRightStick().x * 20,
                         -100.0f + controller.getRightStick().y * 20 })
            .setSize({ 61.0f, 61.0f })
    );

    primitive_renderer->drawQuad(
        *mABXYTextures[controller.isHold(1 << rio::Controller::PAD_IDX_A)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 740.0f, -190.0f })
            .setSize({ 33.0f, 33.0f })
    );

    primitive_renderer->drawQuad(
        *mABXYTextures[controller.isHold(1 << rio::Controller::PAD_IDX_B)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 707.0f, -220.0f })
            .setSize({ 33.0f, 33.0f })
    );

    primitive_renderer->drawQuad(
        *mABXYTextures[controller.isHold(1 << rio::Controller::PAD_IDX_X)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 707.0f, -160.0f })
            .setSize({ 33.0f, 33.0f })
    );

    primitive_renderer->drawQuad(
        *mABXYTextures[controller.isHold(1 << rio::Controller::PAD_IDX_Y)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 674.0f, -190.0f })
            .setSize({ 33.0f, 33.0f })
    );

    primitive_renderer->drawQuad(
        *mHomeTextures[controller.isHold(1 << rio::Controller::PAD_IDX_HOME)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 406.0f, -380.0f })
            .setSize({ 38.0f, 38.0f })
    );

    primitive_renderer->drawQuad(
        *mStartSelectTextures[controller.isHold((1 << rio::Controller::PAD_IDX_PLUS) |
                                                (1 << rio::Controller::PAD_IDX_START))],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 672.0f, -272.0f })
            .setSize({ 24.0f, 24.0f })
    );

    primitive_renderer->drawQuad(
        *mStartSelectTextures[controller.isHold((1 << rio::Controller::PAD_IDX_MINUS) |
                                                (1 << rio::Controller::PAD_IDX_SELECT))],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 672.0f, -314.0f })
            .setSize({ 24.0f, 24.0f })
    );

    primitive_renderer->drawQuad(
        *mUpTextures[controller.isHold(1 << rio::Controller::PAD_IDX_UP)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 111.0f, -167.0f })
            .setSize({ 26.0f, 26.0f })
    );

    primitive_renderer->drawQuad(
        *mDownTextures[controller.isHold(1 << rio::Controller::PAD_IDX_DOWN)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 111.0f, -215.0f })
            .setSize({ 26.0f, 26.0f })
    );

    primitive_renderer->drawQuad(
        *mLeftTextures[controller.isHold(1 << rio::Controller::PAD_IDX_LEFT)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 87.0f, -191.0f })
            .setSize({ 26.0f, 26.0f })
    );

    primitive_renderer->drawQuad(
        *mRightTextures[controller.isHold(1 << rio::Controller::PAD_IDX_RIGHT)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 135.0f, -191.0f })
            .setSize({ 26.0f, 26.0f })
    );

    primitive_renderer->drawQuad(
        *mZLTextures[controller.isHold(1 << rio::Controller::PAD_IDX_ZL)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 80.0f, -502.0f })
            .setSize({ 66.0f, 52.0f })
    );

    primitive_renderer->drawQuad(
        *mZRTextures[controller.isHold(1 << rio::Controller::PAD_IDX_ZR)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 732.0f, -502.0f })
            .setSize({ 66.0f, 52.0f })
    );

    primitive_renderer->drawQuad(
        *mLTextures[controller.isHold(1 << rio::Controller::PAD_IDX_L)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 42.0f, -557.0f })
            .setSize({ 82.0f, 40.0f })
    );

    primitive_renderer->drawQuad(
        *mRTextures[controller.isHold(1 << rio::Controller::PAD_IDX_R)],
        rio::PrimitiveRenderer::QuadArg()
            .setCenter({ 770.0f, -557.0f })
            .setSize({ 82.0f, 40.0f })
    );
}
