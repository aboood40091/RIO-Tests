#include "roottask.h"

#include <gfx/rio_Window.h>
#include <gpu/rio_Drawer.h>
#include <gpu/rio_RenderState.h>
#include <misc/rio_MemUtil.h>

RootTask::RootTask()
    : ITask("RootTask")
{
}

void RootTask::prepare_()
{
    // Load Shader
    mShader.load("test_shader");

    // Allocate vertex buffer data
    mpVBData = static_cast<Vertex*>(rio::MemUtil::alloc(cVBDataSize, rio::Drawer::cVtxAlignment));

    // Set the rectangle positions and texture coordinates
    // top right
    mpVBData[0].pos.set(0.5f, 0.5f, 0.0f);
    mpVBData[0].tex.set(1.0f, 0.0f);
    // bottom right
    mpVBData[1].pos.set(0.5f, -0.5f, 0.0f);
    mpVBData[1].tex.set(1.0f, 1.0f);
    // bottom left
    mpVBData[2].pos.set(-0.5f, -0.5f, 0.0f);
    mpVBData[2].tex.set(0.0f, 1.0f);
    // top left
    mpVBData[3].pos.set(-0.5f, 0.5f, 0.0f);
    mpVBData[3].tex.set(0.0f, 0.0f);

    // Allocate index buffer data
    mpIBData = static_cast<u16*>(rio::MemUtil::alloc(cIBDataSize, rio::Drawer::cIdxAlignment));

    // Set the rectangle indices
    mpIBData[0] = 0; mpIBData[1] = 1; mpIBData[2] = 3;  // first triangle
    mpIBData[3] = 1; mpIBData[4] = 2; mpIBData[5] = 3;  // second triangle

    // Flush/invalidate index buffer cache (now, as it won't be modified later)
    rio::VertexBuffer::invalidateCache(mpIBData, cIBDataSize);

    // Set the vertex buffer instance's stride (must be done before setting the instance's data)
    mVertexBuffer.setStride(cVtxElemSize);
    // Set the vertex buffer instance's data and flush/invalidate its cache (now, as it won't be modified later)
    mVertexBuffer.setDataInvalidate(mpVBData, cVBDataSize);

    // Set the vertex attribute streams' layouts
    mPosStream.setLayout(0, rio::VertexStream::FORMAT_32_32_32_FLOAT, offsetof(Vertex, pos));
    mTexStream.setLayout(1, rio::VertexStream::FORMAT_32_32_FLOAT,    offsetof(Vertex, tex));

    // Clear vertex array
    // (Not needed, it's already empty)
  //mVertexArray.initialize();

    // Add vertex attributes and vertex buffer containing them to the vertex array
    mVertexArray.addAttribute(mPosStream, mVertexBuffer);
    mVertexArray.addAttribute(mTexStream, mVertexBuffer);

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
  //mTextureSampler.setMipFilter(rio::TEX_MIP_FILTER_MODE_NONE);
    // (Note: Ignore the above, it was fixed as of 19/09/2022.)
}

void RootTask::calc_()
{
    /*
        Since the rectangle is static, all of the following can instead be done
        in prepare_() while leaving calc_() empty.
    */

    rio::Window::instance()->clearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Our rectangle's front faces are not counter-clockwise, so, we disable culling
    rio::RenderState render_state;
    render_state.setCullingMode(rio::Graphics::CULLING_MODE_NONE);
    // Apply the specified render state
    render_state.apply();

    // Bind shader (must be done before binding vertex array)
    mShader.bind();
    // Bind vertex array
    mVertexArray.bind();

    // Bind texture 0
    mTextureSampler.linkTexture2D(mTextures[0].ptr);
    mTextureSampler.bindFS(mTextures[0].loc, 0);

    // Bind texture 1 (reusing the same texture sampler instance, but a new one can be created instead)
    mTextureSampler.linkTexture2D(mTextures[1].ptr);
    mTextureSampler.bindFS(mTextures[1].loc, 1);

    // Draw
    rio::Drawer::DrawElements(rio::Drawer::TRIANGLES, cIdxNum, mpIBData);
}

void RootTask::exit_()
{
    // Unload textures
    delete mTextures[0].ptr;
    delete mTextures[1].ptr;

    // Free allocated vertex buffer data
    rio::MemUtil::free(mpVBData);

    // Unload shader
    // (Not needed, shader is unloaded by its destructor as well)
  //mShader.unload();
}
