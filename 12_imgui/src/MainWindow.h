#include <task/rio_Task.h>

#include <imgui.h>

class MainWindow : public rio::ITask
{
public:
    MainWindow();

private:
    void prepare_() override;
    void calc_() override;
    void exit_() override;

#if RIO_IS_WIN
    void resize_(s32 width, s32 height);
    static void onResizeCallback_(s32 width, s32 height);
#endif // RIO_IS_WIN

private:
    ImGuiIO* p_io;
    bool show_demo_window;
    bool show_another_window;
    ImVec4 clear_color;
};
