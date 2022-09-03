#include <gfx/rio_Camera.h>
#include <gpu/rio_Shader.h>
#include <gpu/rio_TextureSampler.h>
#include <gpu/rio_UniformBlock.h>
#include <gpu/rio_VertexArray.h>
#include <math/rio_Matrix.h>
#include <math/rio_Vector.h>
#include <task/rio_Task.h>

#include <vector>

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
        rio::Vector3f nml;
    };

    struct ViewBlock
    {
        rio::Vector3f  view_pos;       u32 _padding;
        rio::Matrix44f view_proj_mtx;
    };

    struct LightBlock
    {
        rio::Vector3f  light_color;     u32 _padding_0;
        rio::Vector3f  light_pos;       u32 _padding_1;
    };

    struct ModelBlock
    {
        rio::Matrix34f model_mtx;
        rio::Matrix34f normal_mtx;
        rio::BaseVec4u _padding[10];
    };

    rio::Shader         mShader;

    rio::VertexBuffer   mVertexBuffer;
    rio::VertexStream   mPosStream;
    rio::VertexStream   mTexStream;
    rio::VertexStream   mNmlStream;
    rio::VertexArray    mVertexArray;

    struct
    {
        rio::Texture2D* ptr;
        u32             loc;
    } mTextures[2];

    rio::TextureSampler2D mTextureSampler;

    rio::Matrix44f      mProjMtx;
    rio::LookAtCamera   mCamera;
    f32                 mCounter;

    rio::Vector3f       mLightColor;
    rio::Vector3f       mLightPos;

    rio::UniformBlock*  mpViewUniformBlock;
    rio::UniformBlock*  mpLightUniformBlock;

    static ViewBlock    sViewBlock;
    static LightBlock   sLightBlock;

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

    rio::UniformBlock*              mpModelUniformBlock[cCubeNum];
    static ModelBlock               sModelBlock[cCubeNum];
};
