#include "model.h"

#include <gpu/rio_Drawer.h>
#include <misc/rio_MemUtil.h>

Model::Model(rio::mdl::res::Model* res_mdl, const char* view_block_name, const char* light_block_name, const char* model_block_name)
    : rio::mdl::Model(res_mdl)
{
    u32 num_meshes = numMeshes();

    mModelUniformBlock = (rio::UniformBlock*)rio::MemUtil::alloc(num_meshes * sizeof(rio::UniformBlock), 4);
    mModelBlock = (ModelBlock*)rio::MemUtil::alloc(num_meshes * sizeof(ModelBlock), rio::Drawer::cUniformBlockAlignment);
    mUniformBlocks = (UniformBlocks*)rio::MemUtil::alloc(num_meshes * sizeof(UniformBlocks), 4);

    for (u32 i = 0; i < num_meshes; i++)
    {
        const rio::mdl::Mesh* p_mesh = &(meshes()[i]);
        const rio::mdl::Material* p_material = p_mesh->material();

        ShaderLocation view_block_idx;
        ShaderLocation light_block_idx;
        ShaderLocation model_block_idx;

        if (p_material)
        {
            const rio::Shader* p_shader = p_material->shader();
            RIO_ASSERT(p_shader);

            view_block_idx.vs = p_shader->getVertexUniformBlockIndex(view_block_name);
            view_block_idx.fs = p_shader->getFragmentUniformBlockIndex(view_block_name);
            view_block_idx.findStage();

            light_block_idx.vs = p_shader->getVertexUniformBlockIndex(light_block_name);
            light_block_idx.fs = p_shader->getFragmentUniformBlockIndex(light_block_name);
            light_block_idx.findStage();

            model_block_idx.vs = p_shader->getVertexUniformBlockIndex(model_block_name);
            model_block_idx.fs = p_shader->getFragmentUniformBlockIndex(model_block_name);
            model_block_idx.findStage();
        }

        new (&mModelUniformBlock[i]) rio::UniformBlock(model_block_idx.stage, model_block_idx.vs, model_block_idx.fs);
        mModelUniformBlock[i].setData(&mModelBlock[i], sizeof(ModelBlock));

        new (&mUniformBlocks[i]) UniformBlocks(view_block_idx, light_block_idx);

    }
}

Model::~Model()
{
    rio::MemUtil::free(mModelBlock);

    for (u32 i = 0; i < numMeshes(); i++)
        mModelUniformBlock[i].~UniformBlock();

    rio::MemUtil::free(mModelUniformBlock);
}

void Model::draw(rio::UniformBlock& view_uniform_block, rio::UniformBlock& light_uniform_block) const
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

        const UniformBlocks& uniform_block_idx = mUniformBlocks[i];

        // Set the ViewBlock index and stage
        view_uniform_block.setIndex(uniform_block_idx.view_block_idx.vs, uniform_block_idx.view_block_idx.fs);
        view_uniform_block.setStage(uniform_block_idx.view_block_idx.stage);

        // Bind the ViewBlock uniform
        view_uniform_block.bind();

        // Set the LightBlock index and stage
        light_uniform_block.setIndex(uniform_block_idx.light_block_idx.vs, uniform_block_idx.light_block_idx.fs);
        light_uniform_block.setStage(uniform_block_idx.light_block_idx.stage);

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
