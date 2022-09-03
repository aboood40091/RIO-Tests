#include "roottask.h"

#include <gfx/rio_PrimitiveRenderer.h>
#include <gfx/rio_Projection.h>
#include <gfx/rio_Window.h>
#include <gpu/rio_Drawer.h>
#include <gpu/rio_RenderState.h>
#include <misc/rio_MemUtil.h>

__attribute__((aligned(rio::Drawer::cUniformBlockAlignment))) RootTask::ViewBlock   RootTask::sViewBlock;
__attribute__((aligned(rio::Drawer::cUniformBlockAlignment))) RootTask::ModelBlock  RootTask::sModelBlock[RootTask::cCubeNum];
__attribute__((aligned(rio::Drawer::cUniformBlockAlignment))) RootTask::LightBlock  RootTask::sLightBlock;

__attribute__((aligned(rio::Drawer::cVtxAlignment))) const RootTask::Vertex RootTask::cVBData[] = {
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
    { { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, {  0.0f,  0.0f, -1.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f }, {  0.0f,  0.0f, -1.0f } },

    { { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f }, {  0.0f,  0.0f,  1.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, {  0.0f,  0.0f,  1.0f } },

    { { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f } },

    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f } },

    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
    { {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
    { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
    { { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f }, {  0.0f, -1.0f,  0.0f } },
    { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f }, {  0.0f, -1.0f,  0.0f } },

    { { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
    { {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },
    { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f }, {  0.0f,  1.0f,  0.0f } },
    { { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f }, {  0.0f,  1.0f,  0.0f } }
};

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
    // Load Shader
    mShader.load("test_shader");

    // Set the vertex buffer instance's stride (must be done before setting the instance's data)
    mVertexBuffer.setStride(cVtxElemSize);
    // Set the vertex buffer instance's data and flush/invalidate its cache (now, as it won't be modified later)
    mVertexBuffer.setDataInvalidate(cVBData, cVBDataSize);

    // Set the vertex attribute streams' layouts
    mPosStream.setLayout(0, rio::VertexStream::FORMAT_32_32_32_FLOAT, offsetof(Vertex, pos));
    mTexStream.setLayout(1, rio::VertexStream::FORMAT_32_32_FLOAT,    offsetof(Vertex, tex));
    mNmlStream.setLayout(2, rio::VertexStream::FORMAT_32_32_32_FLOAT, offsetof(Vertex, nml));

    // Clear vertex array
    // (Not needed, it's already empty)
  //mVertexArray.initialize();

    // Add vertex attributes and vertex buffer containing them to the vertex array
    mVertexArray.addAttribute(mPosStream, mVertexBuffer);
    mVertexArray.addAttribute(mTexStream, mVertexBuffer);
    mVertexArray.addAttribute(mNmlStream, mVertexBuffer);

    // Process added vertex attributes
    mVertexArray.process();

    // Load textures
    mTextures[0].ptr = new rio::Texture2D("container");
    mTextures[1].ptr = new rio::Texture2D("awesomeface");

    // Find the samplers' locations
    mTextures[0].loc = mShader.getFragmentSamplerLocation("texture0");
    mTextures[1].loc = mShader.getFragmentSamplerLocation("texture1");

    // Disable mip filter as our textures don't have mipmaps
    // (Not doing this results in a black screen on my PC with OpenGL, however GX2 seems to not care)
    mTextureSampler.setMipFilter(rio::TEX_MIP_FILTER_MODE_NONE);

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
    mpViewUniformBlock = new rio::UniformBlock(rio::UniformBlock::STAGE_ALL, 1);
    // Set base data for view uniform block
    mpViewUniformBlock->setData(&sViewBlock, sizeof(ViewBlock));

    for (s32 i = 0; i < cCubeNum; i++)
    {
        f32 angle = rio::Mathf::deg2rad(20) * i;

        // Model (local-to-world) matrix
        sModelBlock[i].model_mtx.makeRT(
            { angle, angle * 1.f/3.f, angle * 2.f/3.f },
            cCubePos[i]
        );

        // Normal matrix
        sModelBlock[i].normal_mtx.setInverseTranspose(sModelBlock[i].model_mtx);

        // Create model uniform block instance
        mpModelUniformBlock[i] = new rio::UniformBlock(rio::UniformBlock::STAGE_VERTEX_SHADER, 2);
        // Set model uniform block data and invalidate cache now as it won't be modified later
        mpModelUniformBlock[i]->setDataInvalidate(&sModelBlock[i], sizeof(ModelBlock));
    }

    // Set light color
    mLightColor.set(0.625f, 1.0f, 1.0f);
    // Set light position
    mLightPos.set(0.0f, 0.0f, 2.0f);

    // Create light uniform block instance
    mpLightUniformBlock = new rio::UniformBlock(rio::UniformBlock::STAGE_FRAGMENT_SHADER, 3);
    // Set light uniform block data and invalidate cache now as it won't be modified later
    sLightBlock.light_color = mLightColor;
    sLightBlock.light_pos = mLightPos;
    mpLightUniformBlock->setDataInvalidate(&sLightBlock, sizeof(LightBlock));

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
    mpViewUniformBlock->setSubDataInvalidate(&sViewBlock, 0, sizeof(ViewBlock));

    // Our front faces are not counter-clockwise, so, we disable culling
    rio::RenderState render_state;
    render_state.setCullingMode(rio::Graphics::CULLING_MODE_NONE);
    // Apply the specified render state
    render_state.apply();

    // Bind shader (must be done before binding vertex array)
    mShader.bind();
    // Bind vertex array
    mVertexArray.bind();

    // Bind view uniform block
    mpViewUniformBlock->bind();

    // Bind light uniform block
    mpLightUniformBlock->bind();

    // Bind texture 0
    mTextureSampler.linkTexture2D(mTextures[0].ptr);
    mTextureSampler.bindFS(mTextures[0].loc, 0);

    // Bind texture 1 (reusing the same texture sampler instance, but a new one can be created instead)
    mTextureSampler.linkTexture2D(mTextures[1].ptr);
    mTextureSampler.bindFS(mTextures[1].loc, 1);

    for (s32 i = 0; i < cCubeNum; i++)
    {
        // Bind model uniform block
        mpModelUniformBlock[i]->bind();

        // Draw
        rio::Drawer::DrawArrays(rio::Drawer::TRIANGLES, cVtxNum);
    }

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

    for (s32 i = 0; i < cCubeNum; i++)
        delete mpModelUniformBlock[i];

    delete mpViewUniformBlock;

    // Unload textures
    delete mTextures[0].ptr;
    delete mTextures[1].ptr;

    // Unload shader
    // (Not needed, shader is unloaded by its destructor as well)
  //mShader.unload();
}
