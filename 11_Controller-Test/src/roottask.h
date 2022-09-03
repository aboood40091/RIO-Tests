#include <controller/rio_Controller.h>
#include <gfx/lyr/rio_Renderer.h>
#include <gpu/rio_Texture.h>
#include <task/rio_Task.h>

class RootTask : public rio::ITask, public rio::lyr::IDrawable
{
public:
    RootTask();

    void render(const rio::lyr::DrawInfo&);
    void drawController(const rio::Controller& controller) const;

private:
    void prepare_() override;
    void exit_() override;

private:
    struct
    {
        rio::lyr::Layer::iterator   it;
        rio::lyr::Layer*            ptr;
    } mLayer;

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
};
