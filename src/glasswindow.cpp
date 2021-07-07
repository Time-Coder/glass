#ifndef USE_QT
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#else
#include <Qt>
#include <QMouseEvent>
#include <QApplication>
#include <QIcon>
#endif

#include "glass/GlassWindow"
#include "glass/utils/image.h"
#include "glass/utils/exceptions.h"

using namespace std;

GlassWindow* GlassWindow::current = nullptr;
bool GlassWindow::is_glad_init = false;

map<uint, string> GlassWindow::key_map =
{
#ifndef USE_QT
    {GLFW_KEY_SPACE, " "},
    {GLFW_KEY_APOSTROPHE, "'"},
    {GLFW_KEY_COMMA, ","},
    {GLFW_KEY_MINUS, "-"},
    {GLFW_KEY_PERIOD, "."},
    {GLFW_KEY_SLASH, "/"},
    {GLFW_KEY_0, "0"},
    {GLFW_KEY_1, "1"},
    {GLFW_KEY_2, "2"},
    {GLFW_KEY_3, "3"},
    {GLFW_KEY_4, "4"},
    {GLFW_KEY_5, "5"},
    {GLFW_KEY_6, "6"},
    {GLFW_KEY_7, "7"},
    {GLFW_KEY_8, "8"},
    {GLFW_KEY_9, "9"},
    {GLFW_KEY_SEMICOLON, "}"},
    {GLFW_KEY_EQUAL, "="},
    {GLFW_KEY_A, "A"},
    {GLFW_KEY_B, "B"},
    {GLFW_KEY_C, "C"},
    {GLFW_KEY_D, "D"},
    {GLFW_KEY_E, "E"},
    {GLFW_KEY_F, "F"},
    {GLFW_KEY_G, "G"},
    {GLFW_KEY_H, "H"},
    {GLFW_KEY_I, "I"},
    {GLFW_KEY_J, "J"},
    {GLFW_KEY_K, "K"},
    {GLFW_KEY_L, "L"},
    {GLFW_KEY_M, "M"},
    {GLFW_KEY_N, "N"},
    {GLFW_KEY_O, "O"},
    {GLFW_KEY_P, "P"},
    {GLFW_KEY_Q, "Q"},
    {GLFW_KEY_R, "R"},
    {GLFW_KEY_S, "S"},
    {GLFW_KEY_T, "T"},
    {GLFW_KEY_U, "U"},
    {GLFW_KEY_V, "V"},
    {GLFW_KEY_W, "W"},
    {GLFW_KEY_X, "X"},
    {GLFW_KEY_Y, "Y"},
    {GLFW_KEY_Z, "Z"},
    {GLFW_KEY_LEFT_BRACKET, "["},
    {GLFW_KEY_BACKSLASH, "\\"},
    {GLFW_KEY_RIGHT_BRACKET, "]"},
    {GLFW_KEY_GRAVE_ACCENT, "`"},
    {GLFW_KEY_ESCAPE, "ESC"},
    {GLFW_KEY_ENTER, "ENTER"},
    {GLFW_KEY_TAB, "TAB"},
    {GLFW_KEY_BACKSPACE, "BACKSPACE"},
    {GLFW_KEY_INSERT, "INSERT"},
    {GLFW_KEY_DELETE, "DELETE"},
    {GLFW_KEY_RIGHT, "RIGHT"},
    {GLFW_KEY_LEFT, "LEFT"},
    {GLFW_KEY_DOWN, "DOWN"},
    {GLFW_KEY_UP, "UP"},
    {GLFW_KEY_PAGE_UP, "PAGE_UP"},
    {GLFW_KEY_PAGE_DOWN, "PAGE_DOWN"},
    {GLFW_KEY_HOME, "HOME"},
    {GLFW_KEY_END, "END"},
    {GLFW_KEY_CAPS_LOCK, "CAPS_LOCK"},
    {GLFW_KEY_SCROLL_LOCK, "SCROLL_LOCK"},
    {GLFW_KEY_NUM_LOCK, "NUM_LOCK"},
    {GLFW_KEY_PRINT_SCREEN, "PRINT_SCREEN"},
    {GLFW_KEY_PAUSE, "PAUSE"},
    {GLFW_KEY_F1, "F1"},
    {GLFW_KEY_F2, "F2"},
    {GLFW_KEY_F3, "F3"},
    {GLFW_KEY_F4, "F4"},
    {GLFW_KEY_F5, "F5"},
    {GLFW_KEY_F6, "F6"},
    {GLFW_KEY_F7, "F7"},
    {GLFW_KEY_F8, "F8"},
    {GLFW_KEY_F9, "F9"},
    {GLFW_KEY_F10, "F10"},
    {GLFW_KEY_F11, "F11"},
    {GLFW_KEY_F12, "F12"},
    {GLFW_KEY_F13, "F13"},
    {GLFW_KEY_F14, "F14"},
    {GLFW_KEY_F15, "F15"},
    {GLFW_KEY_F16, "F16"},
    {GLFW_KEY_F17, "F17"},
    {GLFW_KEY_F18, "F18"},
    {GLFW_KEY_F19, "F19"},
    {GLFW_KEY_F20, "F20"},
    {GLFW_KEY_F21, "F21"},
    {GLFW_KEY_F22, "F22"},
    {GLFW_KEY_F23, "F23"},
    {GLFW_KEY_F24, "F24"},
    {GLFW_KEY_F25, "F25"},
    {GLFW_KEY_KP_0, "KP_0"},
    {GLFW_KEY_KP_1, "KP_1"},
    {GLFW_KEY_KP_2, "KP_2"},
    {GLFW_KEY_KP_3, "KP_3"},
    {GLFW_KEY_KP_4, "KP_4"},
    {GLFW_KEY_KP_5, "KP_5"},
    {GLFW_KEY_KP_6, "KP_6"},
    {GLFW_KEY_KP_7, "KP_7"},
    {GLFW_KEY_KP_8, "KP_8"},
    {GLFW_KEY_KP_9, "KP_9"},
    {GLFW_KEY_KP_DECIMAL, "KP_."},
    {GLFW_KEY_KP_DIVIDE, "KP_/"},
    {GLFW_KEY_KP_MULTIPLY, "*"},
    {GLFW_KEY_KP_SUBTRACT, "KP_-"},
    {GLFW_KEY_KP_ADD, "+"},
    {GLFW_KEY_KP_ENTER, "KP_ENTER"},
    {GLFW_KEY_KP_EQUAL, "KP_="},
    {GLFW_KEY_LEFT_SHIFT, "LEFT_SHIFT"},
    {GLFW_KEY_LEFT_CONTROL, "LEFT_CONTROL"},
    {GLFW_KEY_LEFT_ALT, "LEFT_ALT"},
    {GLFW_KEY_LEFT_SUPER, "LEFT_SUPER"},
    {GLFW_KEY_RIGHT_SHIFT, "RIGHT_SHIFT"},
    {GLFW_KEY_RIGHT_CONTROL, "RIGHT_CONTROL"},
    {GLFW_KEY_RIGHT_ALT, "RIGHT_ALT"},
    {GLFW_KEY_RIGHT_SUPER, "RIGHT_SUPER"},
    {GLFW_KEY_MENU, "MENU"}
#else
    {Qt::Key_Space, " "},
    {Qt::Key_Apostrophe, "'"},
    {Qt::Key_Comma, ","},
    {Qt::Key_Minus, "-"},
    {Qt::Key_Period, "."},
    {Qt::Key_Slash, "/"},
    {Qt::Key_0, "0"},
    {Qt::Key_1, "1"},
    {Qt::Key_2, "2"},
    {Qt::Key_3, "3"},
    {Qt::Key_4, "4"},
    {Qt::Key_5, "5"},
    {Qt::Key_6, "6"},
    {Qt::Key_7, "7"},
    {Qt::Key_8, "8"},
    {Qt::Key_9, "9"},
    {Qt::Key_Semicolon, "}"},
    {Qt::Key_Equal, "="},
    {Qt::Key_A, "A"},
    {Qt::Key_B, "B"},
    {Qt::Key_C, "C"},
    {Qt::Key_D, "D"},
    {Qt::Key_E, "E"},
    {Qt::Key_F, "F"},
    {Qt::Key_G, "G"},
    {Qt::Key_H, "H"},
    {Qt::Key_I, "I"},
    {Qt::Key_J, "J"},
    {Qt::Key_K, "K"},
    {Qt::Key_L, "L"},
    {Qt::Key_M, "M"},
    {Qt::Key_N, "N"},
    {Qt::Key_O, "O"},
    {Qt::Key_P, "P"},
    {Qt::Key_Q, "Q"},
    {Qt::Key_R, "R"},
    {Qt::Key_S, "S"},
    {Qt::Key_T, "T"},
    {Qt::Key_U, "U"},
    {Qt::Key_V, "V"},
    {Qt::Key_W, "W"},
    {Qt::Key_X, "X"},
    {Qt::Key_Y, "Y"},
    {Qt::Key_Z, "Z"},
    {Qt::Key_BracketLeft, "["},
    {Qt::Key_Backslash, "\\"},
    {Qt::Key_BracketRight, "]"},
    {Qt::Key_Agrave, "`"},
    {Qt::Key_Escape, "ESC"},
    {Qt::Key_Return, "ENTER"},
    {Qt::Key_Tab, "TAB"},
    {Qt::Key_Backspace, "BACKSPACE"},
    {Qt::Key_Insert, "INSERT"},
    {Qt::Key_Delete, "DELETE"},
    {Qt::Key_Right, "RIGHT"},
    {Qt::Key_Left, "LEFT"},
    {Qt::Key_Down, "DOWN"},
    {Qt::Key_Up, "UP"},
    {Qt::Key_PageUp, "PAGE_UP"},
    {Qt::Key_PageDown, "PAGE_DOWN"},
    {Qt::Key_Home, "HOME"},
    {Qt::Key_End, "END"},
    {Qt::Key_CapsLock, "CAPS_LOCK"},
    {Qt::Key_ScrollLock, "SCROLL_LOCK"},
    {Qt::Key_NumLock, "NUM_LOCK"},
    {Qt::Key_Print, "PRINT_SCREEN"},
    {Qt::Key_Pause, "PAUSE"},
    {Qt::Key_F1, "F1"},
    {Qt::Key_F2, "F2"},
    {Qt::Key_F3, "F3"},
    {Qt::Key_F4, "F4"},
    {Qt::Key_F5, "F5"},
    {Qt::Key_F6, "F6"},
    {Qt::Key_F7, "F7"},
    {Qt::Key_F8, "F8"},
    {Qt::Key_F9, "F9"},
    {Qt::Key_F10, "F10"},
    {Qt::Key_F11, "F11"},
    {Qt::Key_F12, "F12"},
    {Qt::Key_F13, "F13"},
    {Qt::Key_F14, "F14"},
    {Qt::Key_F15, "F15"},
    {Qt::Key_F16, "F16"},
    {Qt::Key_F17, "F17"},
    {Qt::Key_F18, "F18"},
    {Qt::Key_F19, "F19"},
    {Qt::Key_F20, "F20"},
    {Qt::Key_F21, "F21"},
    {Qt::Key_F22, "F22"},
    {Qt::Key_F23, "F23"},
    {Qt::Key_F24, "F24"},
    {Qt::Key_F25, "F25"},
    {Qt::Key_Shift, "SHIFT"},
    {Qt::Key_Control, "CONTROL"},
    {Qt::Key_Alt, "ALT"},
    {Qt::Key_Super_L, "LEFT_SUPER"},
    {Qt::Key_Super_R, "Right_SUPER"},
    {Qt::Key_Menu, "MENU"}
#endif
};

GlassWindow::Constructor::Constructor()
{
#ifndef USE_QT
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAC, GL_TRUE);
#endif
#endif
}

GlassWindow::Constructor::~Constructor()
{
#ifndef USE_QT
	glfwTerminate();
#endif
}
GlassWindow::Constructor GlassWindow::constructor;

void GlassWindow::glad_init()
{
	if(!GlassWindow::is_glad_init)
	{
	#ifndef USE_QT
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	#else
		if(!gladLoadGL())
	#endif
		{
	        throw glass::RuntimeError("Failed to initialize GLAD");
	    }
		GlassWindow::is_glad_init = true;
	}
}

#ifndef USE_QT
void GlassWindow::mouse_position_callback(GLFWwindow* _window, double x, double y)
#else
void GlassWindow::mouseMoveEvent(QMouseEvent* event)
#endif
{
#ifndef USE_QT
	glfwGetCursorPos(_window, &(current->__current_x), &(current->__current_y));
#else
    QPoint point = event->pos();
    current->__current_x = point.x();
    current->__current_y = point.y();
#endif

	if(current->left_pressed)
	{
		current->onDrag();
	}
	else if(current->middle_pressed)
	{
		current->onMiddleDrag();
	}
	else if(current->right_pressed)
	{
		current->onRightDrag();
	}

    if(current->is_move_bound_detecting && current->mouse_stoped)
	{
		current->onMouseStartMove();
    }

	current->onMouseMove();
	
    if(current->is_move_bound_detecting)
    {
        current->mouse_stoped = false;
        current->scheduler_mouse.cancle();
        current->scheduler_mouse.schedule(&GlassWindow::__mouse_stop_move_callback, 0.2);
    }

    current->__last_x = current->__current_x;
    current->__last_y = current->__current_y;
}

void GlassWindow::__mouse_stop_move_callback()
{
	current->onMouseStopMove();
    current->mouse_stoped = true;
}

void GlassWindow::__wheel_stop_scroll_callback()
{
    current->onWheelStopScroll();
    current->wheel_stoped = true;
}

#ifndef USE_QT
void GlassWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS)
		{
			current->onLeftPress();
            current->left_press_time = current->timer.toc();
            current->__last_left_press_x = current->__current_x;
            current->__last_left_press_y = current->__current_y;
            current->left_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			current->onLeftRelease();
			double current_time = current->timer.toc();
			if(current_time - current->left_press_time <= 0.2)
			{
				current->onClick();
				if(current_time - current->left_click_time <= 0.4)
				{
					current->onDoubleClick();
				}
                current->left_click_time = current_time;
			}
            current->__last_left_release_x = current->__current_x;
            current->__last_left_release_y = current->__current_y;
            current->left_pressed = false;
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if(action == GLFW_PRESS)
		{
			current->onRightPress();
            current->right_press_time = current->timer.toc();
            current->__last_right_press_x = current->__current_x;
            current->__last_right_press_y = current->__current_y;
            current->right_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			current->onRightRelease();
			double current_time = current->timer.toc();
			if(current_time - current->right_press_time <= 0.2)
			{
				current->onRightClick();
				if(current_time - current->right_click_time <= 0.4)
				{
					current->onRightDoubleClick();
				}
                current->right_click_time = current_time;
			}
            current->__last_right_release_x = current->__current_x;
            current->__last_right_release_y = current->__current_y;
            current->right_pressed = false;
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if(action == GLFW_PRESS)
		{
			current->onMiddlePress();
            current->middle_press_time = current->timer.toc();
            current->__last_middle_press_x = current->__current_x;
            current->__last_middle_press_y = current->__current_y;
            current->middle_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			current->onMiddleRelease();
			double current_time = current->timer.toc();
			if(current_time - current->middle_press_time <= 0.2)
			{
				current->onMiddleClick();
				if(current_time - current->middle_click_time <= 0.4)
				{
					current->onMiddleDoubleClick();
				}
                current->middle_click_time = current_time;
			}
            current->__last_middle_release_x = current->__current_x;
            current->__last_middle_release_y = current->__current_y;
            current->middle_pressed = false;
		}
	}
}
#else
void GlassWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    __current_x = point.x();
    __current_y = point.y();

    switch(event->button())
    {
    case Qt::LeftButton:
    {
        onLeftPress();
        left_press_time = timer.toc();
        __last_left_press_x = __current_x;
        __last_left_press_y = __current_y;
        left_pressed = true;
        break;
    }
    case Qt::RightButton:
    {
        onRightPress();
        right_press_time = timer.toc();
        __last_right_press_x = __current_x;
        __last_right_press_y = __current_y;
        right_pressed = true;
        break;
    }
    case Qt::MiddleButton:
    {
        onMiddlePress();
        middle_press_time = timer.toc();
        __last_middle_press_x = __current_x;
        __last_middle_press_y = __current_y;
        middle_pressed = true;
        break;
    }
    default: break;
    }

    __last_x = __current_x;
    __last_y = __current_y;
}

void GlassWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    __current_x = point.x();
    __current_y = point.y();

    switch(event->button())
    {
    case Qt::LeftButton:
    {
        onLeftRelease();
        double current_time = timer.toc();
        if(current_time - left_press_time <= 0.2)
        {
            onClick();
            if(current_time - left_click_time <= 0.4)
            {
                onDoubleClick();
            }
            left_click_time = current_time;
        }
        __last_left_release_x = __current_x;
        __last_left_release_y = __current_y;
        left_pressed = false;
        break;
    }
    case Qt::RightButton:
    {
        onRightRelease();
        double current_time = timer.toc();
        if(current_time - right_press_time <= 0.2)
        {
            onRightClick();
            if(current_time - right_click_time <= 0.4)
            {
                onRightDoubleClick();
            }
            right_click_time = current_time;
        }
        __last_right_release_x = __current_x;
        __last_right_release_y = __current_y;
        right_pressed = false;
        break;
    }
    case Qt::MiddleButton:
    {
        onMiddleRelease();
        double current_time = timer.toc();
        if(current_time - middle_press_time <= 0.2)
        {
            onMiddleClick();
            if(current_time - middle_click_time <= 0.4)
            {
                onMiddleDoubleClick();
            }
            middle_click_time = current_time;
        }
        __last_middle_release_x = __current_x;
        __last_middle_release_y = __current_y;
        middle_pressed = false;
        break;
    }
    default:
        break;
    }

    __last_x = __current_x;
    __last_y = __current_y;
}
#endif

#ifndef USE_QT
void GlassWindow::__keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		current->onKeyPress(key_map[key]);
		current->onKeyRepeat(key_map[key]);
	}
	else if(action == GLFW_REPEAT)
	{
		current->onKeyRepeat(key_map[key]);
	}
	else if(action == GLFW_RELEASE)
	{
		current->onKeyRelease(key_map[key]);
	}
}
#else
void GlassWindow::keyPressEvent(QKeyEvent *event)
{
    if(!(event->isAutoRepeat()))
    {
        current->onKeyPress(key_map[event->key()]);
    }
    current->onKeyRepeat(key_map[event->key()]);
}

void GlassWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(!(event->isAutoRepeat()))
    {
        current->onKeyRelease(key_map[event->key()]);
    }
}
#endif

#ifndef USE_QT
void GlassWindow::mouse_enter_leave_callback(GLFWwindow* window, int flag)
{
	if(flag)
	{
		current->onMouseEnter();
	}
	else
	{
		current->onMouseLeave();
        current->__last_x = -1;
        current->__last_y = -1;
	}
}
#else
//void GlassWindow::enterEvent(QEvent* event)
//{
//    Q_UNUSED(event);
//    onMouseEnter();
//}

void GlassWindow::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);

    QPoint point = QCursor::pos();
    point = mapFromGlobal(point);

    __current_x = point.x();
    __current_y = point.y();

    if(!is_cursor_hiden)
    {
        onMouseLeave();
    }
    else
    {
        double new_x = __current_x, new_y = __current_y;
        if(__current_x <= 0)
        {
            __base_x -= width();
            new_x = width();
        }
        else if(__current_x >= width())
        {
            __base_x += width();
            new_x = 0;
        }

        if(__current_y <= 0)
        {
            __base_y -= height();
            new_y = height();
        }
        else if(__current_y >= height())
        {
            __base_y += height();
            new_y = 0;
        }

        QCursor::setPos(mapToGlobal(QPoint(new_x, new_y)));
    }

    __last_x = -1;
    __last_y = -1;
}
#endif

#ifndef USE_QT
void GlassWindow::__scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
#else
void GlassWindow::wheelEvent(QWheelEvent *event)
#endif
{
    if(current->is_scroll_bound_detecting && current->wheel_stoped)
    {
        current->onWheelStartScroll();
    }
#ifndef USE_QT
    current->onWheelScroll(yoffset);
#else
    current->onWheelScroll(event->angleDelta().y());
#endif
    if(current->is_scroll_bound_detecting)
    {
        current->wheel_stoped = false;
        current->scheduler_wheel.cancle();
        current->scheduler_wheel.schedule(&GlassWindow::__wheel_stop_scroll_callback, 0.2);
    }
}

int GlassWindow::current_x()
{
    return __base_x + __current_x;
}

int GlassWindow::current_y()
{
    return __base_y + __current_y;
}

double GlassWindow::current_t()
{
    return __current_t;
}

int GlassWindow::last_x()
{
    return __base_x + (__last_x != -1 ? __last_x : __current_x);
}

int GlassWindow::last_y()
{
    return __base_y + (__last_y != -1 ? __last_y : __current_y);
}

double GlassWindow::last_t()
{
    return __last_t != -1 ? __last_t : __current_t;
}

int GlassWindow::dx()
{
    return current_x() - last_x();
}

int GlassWindow::dy()
{
    return current_y() - last_y();
}

double GlassWindow::dt()
{
    return current_t() - last_t();
}

double GlassWindow::fps()
{
    return 1.0 / dt();
}

int GlassWindow::last_left_press_x()
{
    return __base_x + __last_left_press_x;
}

int GlassWindow::last_left_press_y()
{
    return __base_y + __last_left_press_y;
}

int GlassWindow::last_left_release_x()
{
    return __base_x + __last_left_release_x;
}

int GlassWindow::last_left_release_y()
{
    return __base_y + __last_left_release_y;
}

int GlassWindow::last_right_press_x()
{
    return __base_x + __last_right_press_x;
}

int GlassWindow::last_right_press_y()
{
    return __base_y + __last_right_press_y;
}

int GlassWindow::last_right_release_x()
{
    return __base_x + __last_right_release_x;
}

int GlassWindow::last_right_release_y()
{
    return __base_y + __last_right_release_y;
}

int GlassWindow::last_middle_press_x()
{
    return __base_x + __last_middle_press_x;
}

int GlassWindow::last_middle_press_y()
{
    return __base_y + __last_middle_press_y;
}

int GlassWindow::last_middle_release_x()
{
    return __base_x + __last_middle_release_x;
}

int GlassWindow::last_middle_release_y()
{
    return __base_y + __last_middle_release_y;
}

bool GlassWindow::is_left_pressed()
{
	return left_pressed;
}

bool GlassWindow::is_middle_pressed()
{
	return middle_pressed;
}

bool GlassWindow::is_right_pressed()
{
	return right_pressed;
}

void GlassWindow::onDrag() {}
void GlassWindow::onRightDrag() {}
void GlassWindow::onMiddleDrag() {}
void GlassWindow::onMouseStartMove() {}
void GlassWindow::onMouseMove() {}
void GlassWindow::onMouseStopMove() {}
void GlassWindow::onDoubleClick() {}
void GlassWindow::onRightDoubleClick() {}
void GlassWindow::onMiddleDoubleClick() {}
void GlassWindow::onClick() {}
void GlassWindow::onRightClick() {}
void GlassWindow::onMiddleClick() {}
void GlassWindow::onLeftPress() {}
void GlassWindow::onRightPress() {}
void GlassWindow::onMiddlePress() {}
void GlassWindow::onLeftRelease() {}
void GlassWindow::onRightRelease() {}
void GlassWindow::onMiddleRelease() {}
void GlassWindow::onWheelStartScroll() {}
void GlassWindow::onWheelScroll(int scroll_step)
{
#ifdef USE_QT
    Q_UNUSED(scroll_step);
#endif
}
void GlassWindow::onWheelStopScroll() {}
void GlassWindow::onMouseEnter() {}
void GlassWindow::onMouseLeave() {}

void GlassWindow::onKeyPress(const string& key)
{
#ifdef USE_QT
    Q_UNUSED(key);
#endif
}

void GlassWindow::onKeyRelease(const string& key)
{
#ifdef USE_QT
    Q_UNUSED(key);
#endif
}

void GlassWindow::onKeyRepeat(const string& key)
{
#ifdef USE_QT
    Q_UNUSED(key);
#endif
}

#ifndef USE_QT
void GlassWindow::__change_size_callback(GLFWwindow* window, int width, int height)
#else
void GlassWindow::resizeGL(int width, int height)
#endif
{
#ifndef USE_QT
    glViewport(0, 0, width, height);
	current->__width = width;
	current->__height = height;
#endif
	current->onChangeSize(width, height);
}

void GlassWindow::onChangeSize(int width, int height)
{
#ifdef USE_QT
    Q_UNUSED(width);
    Q_UNUSED(height);
#endif
}

void GlassWindow::prepare() {}
void GlassWindow::draw() {}

void GlassWindow::autoClear(bool flag)
{
	__auto_clear = flag;
}

bool GlassWindow::autoClear()
{
	return __auto_clear;
}

#ifndef USE_QT
void GlassWindow::show()
{
	prepare();

	glfwShowWindow(__window);

	glEnable(GL_MULTISAMPLE); // 抗锯齿
	glEnable(GL_DEPTH_TEST); // 深度测试
	// glEnable(GL_CULL_FACE); // 面剔除
	// glEnable(GL_FRAMEBUFFER_SRGB); // Gamma 校正

    is_move_bound_detecting = ((void*)(this->*(&GlassWindow::onMouseStartMove)) != (void*)(&GlassWindow::onMouseStartMove) ||
                               (void*)(this->*(&GlassWindow::onMouseStopMove)) != (void*)(&GlassWindow::onMouseStopMove));
    is_scroll_bound_detecting = ((void*)(this->*(&GlassWindow::onWheelStartScroll)) != (void*)(&GlassWindow::onWheelStartScroll) ||
                                 (void*)(this->*(&GlassWindow::onWheelStopScroll)) != (void*)(&GlassWindow::onWheelStopScroll));

	while(!glfwWindowShouldClose(__window))
    {
        __last_t = __current_t;
		__current_t = timer.toc();

		if(__auto_clear)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		draw();
		
		glfwSwapBuffers(__window);
    	glfwPollEvents();
	}
}
#else
void GlassWindow::initializeGL()
{
    makeCurrent();
    glad_init();
    glClearColor(__color.x, __color.y, __color.z, __color.w);

    prepare();

    glEnable(GL_MULTISAMPLE); // 抗锯齿
    glEnable(GL_DEPTH_TEST); // 深度测试
    // glEnable(GL_CULL_FACE); // 面剔除
    // glEnable(GL_FRAMEBUFFER_SRGB); // Gamma 校正

    is_move_bound_detecting = ((void*)(this->*(&GlassWindow::onMouseStartMove)) != (void*)(&GlassWindow::onMouseStartMove) ||
                               (void*)(this->*(&GlassWindow::onMouseStopMove)) != (void*)(&GlassWindow::onMouseStopMove));
    is_scroll_bound_detecting = ((void*)(this->*(&GlassWindow::onWheelStartScroll)) != (void*)(&GlassWindow::onWheelStartScroll) ||
                                 (void*)(this->*(&GlassWindow::onWheelStopScroll)) != (void*)(&GlassWindow::onWheelStopScroll));

#ifdef USE_QT
    emit initialized();
#endif
}

void GlassWindow::paintGL()
{
    __last_t = __current_t;
    __current_t = timer.toc();

    if(__auto_clear)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    draw();
}
#endif

#ifndef USE_QT
void GlassWindow::setIcon(const string& filename)
{
	Image image(filename);

	GLFWimage icon;
	icon.width = image.cols();
	icon.height = image.rows();
	icon.pixels = new unsigned char[image.size()];
	memcpy((void*)icon.pixels, (void*)(image.data()), image.size() * sizeof(unsigned char));

	glfwSetWindowIcon(__window, 1, &icon);
}

void GlassWindow::setIcon(const Image& image)
{
    GLFWimage icon;
    icon.width = image.cols();
    icon.height = image.rows();
    icon.pixels = new unsigned char[image.size()];
    memcpy((void*)icon.pixels, (void*)(image.data()), image.size() * sizeof(unsigned char));

    glfwSetWindowIcon(__window, 1, &icon);
}
#else
void GlassWindow::setIcon(const string& filename)
{
    setWindowIcon(QIcon(QString::fromStdString(filename)));
}

void GlassWindow::setIcon(const QIcon& icon)
{
    setWindowIcon(icon);
}
#endif

#ifndef USE_QT
GlassWindow::GlassWindow(int width, int height, const string& name, const vec4& color) :
__width(width), __height(height),
#else
GlassWindow::GlassWindow(QWidget* parent_widget, int width, int height, const string& name, const vec4& color) :
QOpenGLWidget(parent_widget),
#endif
__name(name), __color(color)
{
    GlassWindow::current = this;
#ifndef USE_QT
	glfwWindowHint(GLFW_SAMPLES, 8);
	__window = glfwCreateWindow(__width, __height, __name.c_str(), NULL, NULL);
	if(__window == NULL)
	{
		throw glass::RuntimeError("Failed to create GLFW window!");
	}
	glfwMakeContextCurrent(__window);
#else
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(8);
    setFormat(surfaceFormat);

    if(width > 0 && height > 0)
    {
        resize(width, height);
    }
    setMouseTracking(true);
#endif

#ifndef USE_QT
	// Callbacks
	glfwSetFramebufferSizeCallback(__window, GlassWindow::__change_size_callback);
	glfwSetCursorEnterCallback(__window, GlassWindow::mouse_enter_leave_callback);
	glfwSetCursorPosCallback(__window, GlassWindow::mouse_position_callback);
	glfwSetMouseButtonCallback(__window, GlassWindow::mouse_button_callback);
	glfwSetScrollCallback(__window, GlassWindow::__scroll_callback);
	glfwSetKeyCallback(__window, GlassWindow::__keyboard_callback);

	// GLFWimage icons[2];
	// icons[0] = getGLFWimage(Image("static/resources/images/icons/icon.png"));
	// icons[1] = getGLFWimage(Image("static/resources/images/icons/icon_small.png"));
	// glfwSetWindowIcon(__window, 2, icons);

    glad_init();
    glViewport(0, 0, __width, __height);
    glClearColor(__color.x, __color.y, __color.z, __color.w);
#endif
}

#ifndef USE_QT
GlassWindow::GlassWindow(GlassWindow&& figure)
{
	if(this == &figure)
	{
		return;
	}

    __name = std::move(figure.__name);
    __width = std::move(figure.__width);
    __height = std::move(figure.__height);
    __color = std::move(figure.__color);
	__window = move(figure.__window);

	current = this;
	figure.__name = "";
	figure.__width = 0;
	figure.__height = 0;
	figure.__window = NULL;
}

GLFWwindow* GlassWindow::window()
{
	return __window;
}

uint GlassWindow::width()const
{
    return __width;
}

uint GlassWindow::height()const
{
    return __height;
}

GlassWindow::~GlassWindow()
{
	if(__window != NULL)
	{
		glfwDestroyWindow(__window);
	}
}
#endif

void GlassWindow::hideCursor()
{
#ifndef USE_QT
	glfwSetInputMode(__window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#else
    QApplication::setOverrideCursor(Qt::BlankCursor);
#endif
    is_cursor_hiden = true;
}

void GlassWindow::showCursor()
{
#ifndef USE_QT
	glfwSetInputMode(__window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#else
    QApplication::restoreOverrideCursor();
#endif
    is_cursor_hiden = false;
}
