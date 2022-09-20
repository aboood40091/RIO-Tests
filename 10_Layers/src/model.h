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
    Model(rio::mdl::res::Model* res_mdl, const char* view_block_name, const char* light_block_name, const char* model_block_name);
    ~Model();

    void draw(rio::UniformBlock& view_uniform_block, rio::UniformBlock& light_uniform_block) const;

private:
    struct ShaderLocation
    {
        ShaderLocation(u32 in_vs = u32(-1), u32 in_fs = u32(-1), rio::UniformBlock::ShaderStage in_stage = rio::UniformBlock::STAGE_NONE)
            : vs(in_vs)
            , fs(in_fs)
            , stage(in_stage)
        {
        }

        void findStage()
        {
            if (vs != u32(-1))
            {
                if (fs != u32(-1))
                    stage = rio::UniformBlock::STAGE_ALL;

                else
                    stage = rio::UniformBlock::STAGE_VERTEX_SHADER;
            }
            else if (fs != u32(-1))
            {
                stage = rio::UniformBlock::STAGE_FRAGMENT_SHADER;
            }
            else
            {
                stage = rio::UniformBlock::STAGE_NONE;
            }
        }

        u32 vs;
        u32 fs;
        rio::UniformBlock::ShaderStage stage;
    };

    struct UniformBlocks
    {
        UniformBlocks()
            : view_block_idx()
            , light_block_idx()
        {
        }

        UniformBlocks(const ShaderLocation& in_view_block_idx, const ShaderLocation& in_light_block_idx)
            : view_block_idx{in_view_block_idx}
            , light_block_idx{in_light_block_idx}
        {
        }

        ShaderLocation   view_block_idx;
        ShaderLocation   light_block_idx;
    };

    rio::UniformBlock*  mModelUniformBlock;
    ModelBlock*         mModelBlock;
    UniformBlocks*      mUniformBlocks;
};
