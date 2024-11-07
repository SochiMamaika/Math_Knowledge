#include <iostream>
#include "window.h"
#include "image.h"
#include "GUI.h"

#define WIN_WIDTH   800
#define WIN_HEIGHT  500
#define GAP         5

class MainWindow : public Window
{
public:
    MainWindow()
    {
        m_pImage = nullptr;
    }
    ~MainWindow()
    {
    }

    void OnCreate();
    bool OnKeyPress(uint64_t value);
    void OnSizeChanged();

public:
    const char *m_filename;

private:
    IMAGEINFO m_ii;
    Image *m_pImage;
};

void MainWindow::OnCreate()
{
    CaptureKeyboard(this);
    if(!m_filename)
    {
        m_filename = "test.png";
    }

    m_ii = theGUI->LoadPNG(m_filename);

    m_pImage = new Image(m_ii);
    AddChild(m_pImage, Point(0,0), Rect(100,100));
    m_pImage->SetBackColor(RGB(1.0,1.0,0.5));
    m_pImage->SetStyle(IMAGE_SCALE_STRETCH);
}

bool MainWindow::OnKeyPress(uint64_t keyval)
{
    switch(keyval)
    {
    case 'q':
        if(m_pImage)
        {
            theGUI->DeletePNG(m_ii);
        }
        DeleteMe();
        return true;
    case '0':
        m_pImage->SetStyle(IMAGE_SCALE_1_1);
        m_pImage->ReDraw();
        return true;
    case '1':
        m_pImage->SetStyle(IMAGE_SCALE_FIT);
        m_pImage->ReDraw();
        return true;
    case '2':
        m_pImage->SetStyle(IMAGE_SCALE_STRETCH);
        m_pImage->ReDraw();
        return true;
    case '3':
        m_pImage->SetStyle(IMAGE_SCALE_XY);
        m_pImage->ReDraw();
        return true;
    case 'p':
    case 'P':
        theGUI->Print();
        return true;
    default:
        ;
    }
    return true;
}

void MainWindow::OnSizeChanged()
{
	Rect mysize = GetInteriorSize();
	m_pImage->SetSize(mysize);
}

int main(int argc, char **argv)
{
    MainWindow *pWindow = new MainWindow;
    pWindow->m_filename = argv[1];

    int res = Run(argc, argv, pWindow, WIN_WIDTH, WIN_HEIGHT);

    delete pWindow;

    return res;
}
