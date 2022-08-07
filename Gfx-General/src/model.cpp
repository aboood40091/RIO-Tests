#include "model.h"

#include <misc/rio_MemUtil.h>

Model::Model(rio::mdl::res::Model* res_mdl)
    : rio::mdl::Model(res_mdl)
{
    u32 num_meshes = numMeshes();

    mModelUniformBlock = (rio::UniformBlock*)rio::MemUtil::alloc(num_meshes * sizeof(rio::UniformBlock), 4);
    mModelBlock = (ModelBlock*)rio::MemUtil::alloc(num_meshes * sizeof(ModelBlock), rio::Drawer::cUniformBlockAlignment);

    for (u32 i = 0; i < num_meshes; i++)
    {
        new (&mModelUniformBlock[i]) rio::UniformBlock(rio::UniformBlock::STAGE_VERTEX_SHADER, 2);
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

void Model::draw(const rio::UniformBlock& viewUniformBlock, const rio::UniformBlock& lightUniformBlock) const
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
        viewUniformBlock.bind();
        // Bind the LightBlock uniform
        lightUniformBlock.bind();

        // Get mesh world matrix
        mModelBlock[i].model = mesh.worldMtx();
        mModelBlock[i].normalMtx.setInverseTranspose(mModelBlock[i].model);

        // Update the ModelBlock uniform
        mModelUniformBlock[i].setSubDataInvalidate(&mModelBlock[i], 0, 2 * sizeof(rio::Matrix34f));
        // Bind the ModelBlock uniform
        mModelUniformBlock[i].bind();

        // Draw
        mesh.draw();
    }
}
