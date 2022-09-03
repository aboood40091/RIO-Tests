#include "model.h"

#include <gpu/rio_Drawer.h>
#include <misc/rio_MemUtil.h>

Model::Model(rio::mdl::res::Model* res_mdl, rio::UniformBlock::ShaderStage uniform_block_stage, u32 uniform_block_location)
    : rio::mdl::Model(res_mdl)
{
    u32 num_meshes = numMeshes();

    mModelUniformBlock = (rio::UniformBlock*)rio::MemUtil::alloc(num_meshes * sizeof(rio::UniformBlock), 4);
    mModelBlock = (ModelBlock*)rio::MemUtil::alloc(num_meshes * sizeof(ModelBlock), rio::Drawer::cUniformBlockAlignment);

    for (u32 i = 0; i < num_meshes; i++)
    {
        new (&mModelUniformBlock[i]) rio::UniformBlock(uniform_block_stage, uniform_block_location);
        mModelUniformBlock[i].setData(&mModelBlock[i], sizeof(ModelBlock));
    }
}

Model::~Model()
{
    rio::MemUtil::free(mModelBlock);

    for (u32 i = 0; i < numMeshes(); i++)
        mModelUniformBlock[i].~UniformBlock();

    rio::MemUtil::free(mModelUniformBlock);
}

void Model::draw(const rio::UniformBlock& view_uniform_block, const rio::UniformBlock& light_uniform_block) const
{
    const rio::mdl::Mesh* const meshes = this->meshes();

    // Render each mesh in order
    for (u32 i = 0; i < numMeshes(); i++)
    {
        // Get the mesh
        const rio::mdl::Mesh& mesh = meshes[i];
        if (!mesh.material())
            continue;

        // Get the material
        const rio::mdl::Material& material = *mesh.material();
        if (!material.shader() || !material.resMaterial().isVisible())
            continue;

        // Bind the material
        material.bind();

        // Bind the ViewBlock uniform
        view_uniform_block.bind();
        // Bind the LightBlock uniform
        light_uniform_block.bind();

        // Get mesh world matrix
        mModelBlock[i].model_mtx = mesh.worldMtx();
        mModelBlock[i].normal_mtx.setInverseTranspose(mModelBlock[i].model_mtx);

        // Update the ModelBlock uniform
        mModelUniformBlock[i].setSubDataInvalidate(&mModelBlock[i], 0, 2 * sizeof(rio::Matrix34f));
        // Bind the ModelBlock uniform
        mModelUniformBlock[i].bind();

        // Draw
        mesh.draw();
    }
}
