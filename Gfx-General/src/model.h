#include <gfx/mdl/rio_Model.h>
#include <gpu/rio_Drawer.h>

class Model : public rio::mdl::Model
{
public:
    Model(rio::mdl::res::Model* res_mdl);
    ~Model();

    void draw(const rio::UniformBlock& viewUniformBlock, const rio::UniformBlock& lightUniformBlock) const;

private:
    struct ModelBlock
    {
        rio::Matrix34f model;
        rio::Matrix34f normalMtx;
        rio::BaseVec4u _padding[10];
    };
    static_assert(sizeof(ModelBlock) % rio::Drawer::cUniformBlockAlignment == 0);

    rio::UniformBlock* mModelUniformBlock;
    ModelBlock* mModelBlock;
};
