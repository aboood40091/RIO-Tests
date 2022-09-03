#include <gfx/rio_Camera.h>
#include <gpu/rio_Shader.h>
#include <gpu/rio_TextureSampler.h>
#include <gpu/rio_VertexArray.h>
#include <math/rio_Matrix.h>
#include <math/rio_Vector.h>
#include <task/rio_Task.h>

class RootTask : public rio::ITask
{
public:
    RootTask();

private:
    void prepare_() override;
    void calc_() override;
    void exit_() override;

private:
    struct Vertex
    {
        rio::Vector3f pos;
        rio::Vector2f tex;
    };

    rio::Shader         mShader;

    rio::VertexBuffer   mVertexBuffer;
    rio::VertexStream   mPosStream;
    rio::VertexStream   mTexStream;
    rio::VertexArray    mVertexArray;

    struct
    {
        rio::Texture2D* ptr;
        u32             loc;
    } mTextures[2];

    rio::TextureSampler2D mTextureSampler;

    rio::Matrix44f      mProjMtx;
    rio::LookAtCamera   mCamera;
    u32                 mMvpUniformLoc;
    f32                 mCounter;

    // Size of a single vertex (also the stride of the vertex buffer)
    static constexpr size_t cVtxElemSize = sizeof(Vertex);
    // Number of vertices
    static constexpr s32 cVtxNum = 6*6;
    // Vertex buffer data
    static const Vertex cVBData[cVtxNum];
    // Size of vertex buffer
    static constexpr size_t cVBDataSize = cVtxElemSize * cVtxNum;

    // Number of cubes
    static constexpr s32 cCubeNum = 10;
    // Cube positions
    static const rio::Vector3f cCubePos[cCubeNum];
};
