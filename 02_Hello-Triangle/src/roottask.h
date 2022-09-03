#include <gpu/rio_Shader.h>
#include <gpu/rio_VertexArray.h>
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
    };

    rio::Shader         mShader;

    Vertex*             mpVBData;

    rio::VertexBuffer   mVertexBuffer;
    rio::VertexStream   mPosStream;
    rio::VertexArray    mVertexArray;

    // Size of a single vertex (also the stride of the vertex buffer)
    static constexpr size_t cVtxElemSize = sizeof(Vertex);
    // Number of vertices to draw
    static constexpr s32 cVtxNum = 3;
    // Size of vertex buffer
    static constexpr size_t cVBDataSize = cVtxElemSize * cVtxNum;
};
