#include <cmath>
#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <string.h>
#include <iostream>
#include <string>
#include <cstdlib> // Для rand()

#include "window.h"
#include "text.h"
#include "image.h"
#include "button.h"
#include "GUI.h"

// типы событий
enum UserEventType
{
    EVENT_NEXT,
    EVENT_RESTART
};

// родительское окно
class MainWindow : public Window
{
public:
    MainWindow() {     m_ClassName = __FUNCTION__; m_string[0]=0; }

    void OnCreate();
    void Ask();
    void Lose();
    void OnDraw(Context *cr);
    bool OnTimeout();
    void OnSizeChanged();
    void OnNotify(Window *child, uint32_t type, const Point &position);
    bool OnLeftMouseButtonClick(const Point &Position);
    bool OnKeyPress(uint64_t value);
    void ProcessDigit(uint16_t digit);
    void PrintString();
    void OnResize(); // Метод для обработки изменения размера окна

private:
    int32_t     m_value1 =0;
    int32_t     result;//правильный ответ
    int32_t     m_previousResult;  // Для сохранения правильного ответа текущего шага
    int32_t     n = -1;
    int32_t     n1;
    int32_t     b = 0;
    char        m_string[80];
    char        m_lose[80];
    Text        *m_pText;
    Text        *m_pText1;
    TextButton  *m_button;
     bool        m_isNegative = false; // Флаг для отслеживания отрицательного числа
};

void MainWindow::OnDraw(Context *cr)
{
    std::cout << "MainWindow::OnDraw()" << std::endl;
	Point wsize = GetInteriorSize();

    // зальем прямоугольник окна серым цветом
    cr->SetColor(m_backColor);
    cr->FillRectangle(Point(0,0), wsize);
}

void MainWindow::OnCreate() {
    std::cout << "MainWindow::OnCreate()" << std::endl;
    CreateTimeout(this,1000);
    uint16_t width_window= GetSize().GetWidth();
    uint16_t height_window= GetSize().GetHeight();
    // текст
    m_pText = new Text;
    m_pText->SetFrameWidth(1);
    m_pText->SetFont(NULL, 20, -1, -1);
    m_pText->SetAlignment(TEXT_ALIGNH_LEFT | TEXT_ALIGNV_CENTER);
    m_pText->SetBackColor(RGB(1.0, 1.0, 1.0)); 
    m_pText->SetWrap(true);
    m_pText1 = new Text;
    m_pText1->SetFrameWidth(1);
    m_pText1->SetFont(NULL, 20, -1, -1);
    m_pText1->SetAlignment(TEXT_ALIGNH_LEFT | TEXT_ALIGNV_CENTER);
    m_pText1->SetBackColor(RGB(1.0, 1.0, 1.0)); 
    m_pText1->SetWrap(true);
    AddChild(m_pText, Point(25, 220), Rect(190, 100));
    AddChild(m_pText1, Point(25, 33), Rect(320, 100));
    m_pText->SetBackColor(RGB(0.9, 0.9, 0.9)); 
    m_pText1->SetBackColor(RGB(0.9, 0.9, 0.9)); 
    
    // добавляем кнопку
    m_button = new TextButton("Start", EVENT_RESTART);
    AddChild(m_button, Point(width_window*0.7, height_window*0.8), Rect(80, 40));

    // фокус ввода
    CaptureKeyboard(this);
}

   void MainWindow::Ask() //метод для формирования примера
{
    std::cout << "MainWindow::Ask()" << std::endl;

    // Инициализируем генератор случайных чисел
    srand(time(0)); 

    // Генерируем случайные числа в заданном диапазоне
    int num1 = rand() % 20 + 1;
    int num2 = rand() % 20 + 1; // Числа от 1 до 20

    // Выбираем случайное действие (+, -, *, /)
    int action = rand() % 4; // 4 варианта действий
    std::string operation;
    switch (action) {
        case 0:
            operation = "+";
            result = num1 + num2;
            break;
        case 1:
            operation = "-";
            result = num1 - num2;
            break;
        case 2:
            operation = "*";
            result = num1 * num2;
            break;
        case 3:
            operation = "/";
            // Чтобы деление всегда давало целое, 
            // делаем num1 кратным num2
            num1 = num2 * (rand() % 5 + 1); // num1 кратно num2 от 1 до 5 раз
            result = num1 / num2;
            break;
    }

    // Формируем строку с примером
    std::string example = std::to_string(num1) + " " + operation + " " + std::to_string(num2) + " = ";

    // Выводим строку в Text
    m_pText1->SetText(example.c_str()); 
}

void MainWindow::Lose() {
    uint16_t width_window = GetSize().GetWidth();
    uint16_t height_window = GetSize().GetHeight();
    std::cout << "MainWindow::Lose()" << std::endl;
    if(n > -1)
    {
        n1 = n;
    }

    // Отображаем сообщение о поражении
    std::snprintf(m_lose, sizeof(m_lose), "Вы проиграли!\nПравильных ответов: %d", n1);
    m_pText1->SetText(m_lose);

    // Создаём кнопку "Start" при поражении
    DeleteChild(m_button);
    m_previousResult = 0;
    result = 0;
    n= -1;
    m_button = new TextButton("Start", EVENT_RESTART);
    AddChild(m_button, Point(width_window * 0.7, height_window * 0.8), Rect(80, 40));
    m_button->SetSize(Rect(width_window * 0.2f, height_window * 0.1f));
    m_button->SetPosition(Point(width_window*0.7f, height_window*0.7f));
    CaptureKeyboard(this);
    PrintString();
}

void MainWindow::OnSizeChanged()
{
    std::cout << "MainWindow::OnSizeChanged()" << std::endl;
    uint16_t width_window= GetSize().GetWidth();
    uint16_t height_window= GetSize().GetHeight();
    m_button->SetPosition(Point(width_window*0.7f, height_window*0.7f));
     m_pText->SetPosition(Point(width_window*0.1f, height_window*0.6f));
    m_pText1->SetPosition(Point(width_window*0.1f, height_window*0.12f));
    m_button->SetSize(Rect(width_window * 0.2f, height_window * 0.1f));
     m_pText->SetSize(Rect(width_window * 0.5f, height_window * 0.3f));
    m_pText1->SetSize(Rect(width_window * 0.8f, height_window * 0.3f));
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position) {
    std::cout << "MainWindow::OnNotify()" << std::endl;
    uint16_t width_window= GetSize().GetWidth();
    uint16_t height_window= GetSize().GetHeight();

    if (type == EVENT_RESTART) {
        b = 0;
        // Сохраняем текущий результат перед генерацией нового примера
        m_previousResult = result;
        // Мы создаём кнопку "Next"
        DeleteChild(m_button);
        m_button = new TextButton("Next", EVENT_RESTART);  // Меняем текст на "Next"
        AddChild(m_button, Point(width_window*0.7, height_window*0.8), Rect(80, 40));
        m_button->SetSize(Rect(width_window * 0.2f, height_window * 0.1f));
        m_button->SetPosition(Point(width_window*0.7f, height_window*0.7f));
        CaptureKeyboard(this);
        // Сбрасываем состояние игры, чтобы продолжить
        m_isNegative = false;
        strcpy(m_string, "");
        Ask();
        if (m_value1 != m_previousResult)
        {
            Lose();
        }
        else
        {
            n= n+1;
        }
        m_value1 = 0;
        PrintString();
    }
}

bool MainWindow::OnTimeout()
{   
	b = b + 1;
    if (b % 5 ==0)
    {
        Lose();
    }
    return true;
}

bool MainWindow::OnLeftMouseButtonClick(const Point &Position)
{
//    CaptureKeyboard(this);
    return true;
}

bool MainWindow::OnKeyPress(uint64_t keyval)
{
    std::cout << "MainWindow::OnKeyPress(" << keyval << ")" << std::endl;

    switch(keyval)
    {
    case '-':
        // Устанавливаем флаг для отрицательного числа только если m_value1 еще не введено
        if (m_value1 == 0 && !m_isNegative) 
        {
            m_isNegative = true; // Следующее введенное число будет отрицательным
        }
        PrintString(); // Обновляем вывод
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        ProcessDigit(keyval - '0');
        PrintString(); // Обновляем вывод
        break;

    case KEY_Return:
        OnNotify(m_button, EVENT_NEXT, Point(GetSize().GetWidth() * 0.7, GetSize().GetHeight() * 0.8));
        OnNotify(m_button, EVENT_RESTART, Point(GetSize().GetWidth() * 0.7, GetSize().GetHeight() * 0.8));
        break;
        break;

    case 'q':
        DeleteMe();
        return true;
    case KEY_Esc:
        m_value1 = 0;
        PrintString(); // Обновляем вывод

    default:
        break;
    }
    return true;
}

void MainWindow::ProcessDigit(uint16_t digit)
{
    if (m_isNegative) // Если установлен флаг отрицательности
    {
        m_value1 = m_value1 * 10 - digit; //digit - введеное число
    }
    else
    {
        m_value1 = m_value1 * 10 + digit; // Сбор положительного числа
    }

    PrintString(); // Обновляем вывод
}

void MainWindow::PrintString() {
    strcpy(m_string,"");
    char tmp[40];
    sprintf(tmp, "%d", m_value1); // Форматируем число

    strcpy(m_string, tmp); // Копируем отформатированное число в строку
    m_pText->SetText(m_string); // Устанавливаем текст для отображения
    ReDraw();
}
// функция main
int main(int argc, char **argv)
{
    MainWindow *pWindow = new MainWindow;
    int res = Run(argc, argv, pWindow, 370, 340);
    delete pWindow;

    return res;
}
