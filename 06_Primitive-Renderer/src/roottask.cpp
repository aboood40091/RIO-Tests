#include "roottask.h"

#include <gfx/rio_PrimitiveRenderer.h>
#include <gfx/rio_Projection.h>
#include <gfx/rio_Window.h>
#include <gpu/rio_Drawer.h>
#include <gpu/rio_RenderState.h>
#include <misc/rio_MemUtil.h>

const rio::Vector3f RootTask::cCubePos[] = {
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
    // Load texture
    mpTexture = new rio::Texture2D("container");

    // Set primitive renderer projection
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

        // Set primitive renderer projection
        rio::PrimitiveRenderer::instance()->setProjection(proj);
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

    // Set primitive renderer camera
    rio::PrimitiveRenderer::instance()->setCamera(mCamera);

    // Disable culling
    rio::RenderState render_state;
    render_state.setCullingMode(rio::Graphics::CULLING_MODE_NONE);
    // Apply the specified render state
    render_state.apply();

    // Begin rendering using primitive renderer
    rio::PrimitiveRenderer::instance()->begin();

    // Draw textured quad at center
    {
        rio::PrimitiveRenderer::instance()->setModelMatrix(rio::Matrix34f::ident);

        rio::PrimitiveRenderer::instance()->drawQuad(
            *mpTexture,
            rio::PrimitiveRenderer::QuadArg()
        );
    }

    // Draw cubes
    for (s32 i = 1; i < cCubeNum; i++)  // Skip first cube
    {
        f32 angle = rio::Mathf::deg2rad(20) * i;

        // Model (local-to-world) matrix
        rio::Matrix34f model_mtx;
        model_mtx.makeRT(
            { angle, angle * 1.f/3.f, angle * 2.f/3.f },
            cCubePos[i]
        );

        rio::PrimitiveRenderer::instance()->setModelMatrix(model_mtx);

        rio::PrimitiveRenderer::instance()->drawCube(
            rio::PrimitiveRenderer::CubeArg()
                .setColor(rio::Color4f::cRed, rio::Color4f::cBlue)
        );
    }

    // End rendering using primitive renderer
    rio::PrimitiveRenderer::instance()->end();
}

void RootTask::exit_()
{
    // Unload texture
    delete mpTexture;
}
