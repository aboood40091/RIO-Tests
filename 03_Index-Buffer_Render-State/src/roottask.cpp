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

    // Set the rectangle positions
    mpVBData[0].pos.set( 0.5f,  0.5f, 0.0f);    // top right
    mpVBData[1].pos.set( 0.5f, -0.5f, 0.0f);    // bottom right
    mpVBData[2].pos.set(-0.5f, -0.5f, 0.0f);    // bottom left
    mpVBData[3].pos.set(-0.5f,  0.5f, 0.0f);    // top left

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

    // Set the vertex attribute stream's layout
    mPosStream.setLayout(0, rio::VertexStream::FORMAT_32_32_32_FLOAT, 0);

    // Clear vertex array
    // (Not needed, it's already empty)
  //mVertexArray.initialize();

    // Add vertex attribute and vertex buffer containing it to the vertex array
    mVertexArray.addAttribute(mPosStream, mVertexBuffer);

    // Process added vertex attributes
    mVertexArray.process();

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
    // Enable wire-frame for fun
    render_state.setPolygonMode(rio::Graphics::POLYGON_MODE_LINE);
    // Apply the specified render state
    render_state.apply();

    // Bind shader (must be done before binding vertex array)
    mShader.bind();
    // Bind vertex array
    mVertexArray.bind();

    // Draw
    rio::Drawer::DrawElements(rio::Drawer::TRIANGLES, cIdxNum, mpIBData);
}

void RootTask::exit_()
{
    // Free allocated vertex buffer data
    rio::MemUtil::free(mpVBData);

    // Unload shader
    // (Not needed, shader is unloaded by its destructor as well)
  //mShader.unload();
}
