#include <gfx/mdl/rio_Model.h>

class Model : public rio::mdl::Model
{
public:
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

public:
    Model(rio::mdl::res::Model* res_mdl, rio::UniformBlock::ShaderStage uniform_block_stage, u32 uniform_block_location);
    ~Model();

    void draw(const rio::UniformBlock& view_uniform_block, const rio::UniformBlock& light_uniform_block) const;

private:
    rio::UniformBlock*  mModelUniformBlock;
    ModelBlock*         mModelBlock;
};
