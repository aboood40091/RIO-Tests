#include <controller/rio_Controller.h>
#include <gfx/lyr/rio_Renderer.h>
#include <gfx/rio_Projection.h>
#include <gpu/rio_Texture.h>
#include <math/rio_Matrix.h>
#include <task/rio_TaskMgr.h>

class RootTask : public rio::ITask, public rio::lyr::IDrawable
{
    RootTask();

    void prepare_() override;
    void exit_() override;

    void render(const rio::lyr::DrawInfo& draw_info);
    void drawController(const rio::Controller& controller) const;

    rio::lyr::Layer::iterator mLayerIt;
    rio::lyr::Layer* mLayer;

    rio::Matrix34f mModelMtx;

    rio::Texture2D* mControllerTexture;

    rio::Texture2D* mStickTextures[2];
    rio::Texture2D* mABXYTextures[2];
    rio::Texture2D* mHomeTextures[2];
    rio::Texture2D* mStartSelectTextures[2];
    rio::Texture2D* mUpTextures[2];
    rio::Texture2D* mDownTextures[2];
    rio::Texture2D* mLeftTextures[2];
    rio::Texture2D* mRightTextures[2];
    rio::Texture2D* mLTextures[2];
    rio::Texture2D* mRTextures[2];
    rio::Texture2D* mZLTextures[2];
    rio::Texture2D* mZRTextures[2];

    friend class rio::TaskMgr;
};
