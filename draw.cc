#include <iostream>

#include "window.h"
#include "text.h"
#include "image.h"
#include "button.h"
#include "GUI.h"

// типы событий
enum UserEventType
{
    EVENT_CMD1 = 1,
    EVENT_CMD2,
    EVENT_CMD3,
    EVENT_CMD4,
    EVENT_CMD5,
    EVENT_CMD6,
};

#define BUTTON_WIDTH    120
#define BUTTON_HEIGHT   70

class DrawWindow : public Window
{
public:
    DrawWindow();
    ~DrawWindow();

    void OnDraw(Context *cr);
    bool OnLeftMouseButtonClick(const Point &position);
};

DrawWindow::DrawWindow()
{
}

DrawWindow::~DrawWindow()
{
}

void DrawWindow::OnDraw(Context *cr)
{
}

bool DrawWindow::OnLeftMouseButtonClick(const Point &position)
{
    return true;
}


class MainWindow : public Window
{
public:
    MainWindow() {}
    ~MainWindow() {}

    void OnCreate();
    bool OnKeyPress(uint64_t value);
    void OnNotify(Window *child, uint32_t type, const Point &position);

private:
    DrawWindow *m_pDraw;
};

void MainWindow::OnCreate()
{
    IMAGEINFO ii;
    ImageButton *ib;

    ii = theGUI->LoadPNG("icons48/computer.png");
    ib = new ImageButton(ii, EVENT_CMD1);
    AddChild(ib, Point(0,0), Rect(BUTTON_WIDTH,BUTTON_HEIGHT));
    ib->SetStyle(IMAGE_SCALE_1_1 | IMAGE_ALIGNH_CENTER | IMAGE_ALIGNV_CENTER);

    ii = theGUI->LoadPNG("icons48/document-new.png");
    ib = new ImageButton(ii, EVENT_CMD2);
    AddChild(ib, Point(0,BUTTON_HEIGHT*1), Rect(BUTTON_WIDTH,BUTTON_HEIGHT));
    ib->SetStyle(IMAGE_SCALE_STRETCH | IMAGE_ALIGNH_RIGHT | IMAGE_ALIGNV_BOTTOM);
    ib->SetToggle(true);

    ii = theGUI->LoadPNG("icons48/face-cool.png");
    ib = new ImageButton(ii, EVENT_CMD3);
    AddChild(ib, Point(0,BUTTON_HEIGHT*2), Rect(BUTTON_WIDTH,BUTTON_HEIGHT));
    ib->SetStyle(IMAGE_SCALE_1_1 | IMAGE_ALIGNH_LEFT | IMAGE_ALIGNV_TOP);

    ii = theGUI->LoadPNG("icons48/format-text-bold.png");
    ib = new ImageButton(ii, EVENT_CMD4);
    AddChild(ib, Point(0,BUTTON_HEIGHT*3), Rect(BUTTON_WIDTH,BUTTON_HEIGHT));
    ib->SetStyle(IMAGE_SCALE_FIT | IMAGE_ALIGNH_RIGHT | IMAGE_ALIGNV_BOTTOM);

    ii = theGUI->LoadPNG("icons48/mail-mark-important.png");
    ib = new ImageButton(ii, EVENT_CMD5);
    AddChild(ib, Point(0,BUTTON_HEIGHT*4), Rect(BUTTON_WIDTH,BUTTON_HEIGHT));

    ii = theGUI->LoadPNG("icons48/process-stop.png");
    ib = new ImageButton(ii, EVENT_CMD6);
    AddChild(ib, Point(0,BUTTON_HEIGHT*5), Rect(BUTTON_WIDTH,BUTTON_HEIGHT));
    ib->SetToggle(true);

    CaptureKeyboard(this);
}

bool MainWindow::OnKeyPress(uint64_t keyval)
{
    if(keyval == 'q')
    {
        DeleteMe();
    }
    else if(keyval == 'p' || keyval == 'P')
    {
        theGUI->Print();
    }
    return true;
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position)
{
    std::cout << "MainWindow::OnNotify() type=" << type << std::endl;
}

// функция main
int main(int argc, char **argv)
{
    MainWindow *pWindow = new MainWindow;

    int res = Run(argc, argv, pWindow, 800, 500);

    delete pWindow;

    return res;
}


