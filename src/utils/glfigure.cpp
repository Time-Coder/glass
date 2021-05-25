#include "glass/GLFigure"
#include "glass/utils/image.h"
#include "glass/utils/exceptions.h"

using namespace std;

GLFigure* GLFigure::parent = NULL;
Timer GLFigure::timer;
Scheduler GLFigure::scheduler;
double GLFigure::__current_x = 0.0;
double GLFigure::__current_y = 0.0;
double GLFigure::__current_t = 0.0;
double GLFigure::__last_x = -1.0;
double GLFigure::__last_y = -1.0;
double GLFigure::__last_t = -1.0;
double GLFigure::__last_left_press_x = -1.0;
double GLFigure::__last_left_press_y = -1.0;
double GLFigure::__last_left_release_x = -1.0;
double GLFigure::__last_left_release_y = -1.0;
double GLFigure::__last_right_press_x = -1.0;
double GLFigure::__last_right_press_y = -1.0;
double GLFigure::__last_right_release_x = -1.0;
double GLFigure::__last_right_release_y = -1.0;
double GLFigure::__last_middle_press_x = -1.0;
double GLFigure::__last_middle_press_y = -1.0;
double GLFigure::__last_middle_release_x = -1.0;
double GLFigure::__last_middle_release_y = -1.0;
bool GLFigure::left_pressed = false;
bool GLFigure::middle_pressed = false;
bool GLFigure::right_pressed = false;
double GLFigure::left_press_time = 0.0;
double GLFigure::right_press_time = 0.0;
double GLFigure::middle_press_time = 0.0;
double GLFigure::left_click_time = 0.0;
double GLFigure::right_click_time = 0.0;
double GLFigure::middle_click_time = 0.0;
bool GLFigure::mouse_stoped = true;
bool GLFigure::is_glad_init = false;

map<uint, string> GLFigure::key_map;

GLFigure::Constructor::Constructor()
{
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAC, GL_TRUE);
#endif

	GLFigure::key_map[GLFW_KEY_SPACE] = " ";
	GLFigure::key_map[GLFW_KEY_APOSTROPHE] = "'";
	GLFigure::key_map[GLFW_KEY_COMMA] = ",";
	GLFigure::key_map[GLFW_KEY_MINUS] = "-";
	GLFigure::key_map[GLFW_KEY_PERIOD] = ".";
	GLFigure::key_map[GLFW_KEY_SLASH] = "/";
	GLFigure::key_map[GLFW_KEY_0] = "0";
	GLFigure::key_map[GLFW_KEY_1] = "1";
	GLFigure::key_map[GLFW_KEY_2] = "2";
	GLFigure::key_map[GLFW_KEY_3] = "3";
	GLFigure::key_map[GLFW_KEY_4] = "4";
	GLFigure::key_map[GLFW_KEY_5] = "5";
	GLFigure::key_map[GLFW_KEY_6] = "6";
	GLFigure::key_map[GLFW_KEY_7] = "7";
	GLFigure::key_map[GLFW_KEY_8] = "8";
	GLFigure::key_map[GLFW_KEY_9] = "9";
	GLFigure::key_map[GLFW_KEY_SEMICOLON] = ";";
	GLFigure::key_map[GLFW_KEY_EQUAL] = "=";
	GLFigure::key_map[GLFW_KEY_A] = "A";
	GLFigure::key_map[GLFW_KEY_B] = "B";
	GLFigure::key_map[GLFW_KEY_C] = "C";
	GLFigure::key_map[GLFW_KEY_D] = "D";
	GLFigure::key_map[GLFW_KEY_E] = "E";
	GLFigure::key_map[GLFW_KEY_F] = "F";
	GLFigure::key_map[GLFW_KEY_G] = "G";
	GLFigure::key_map[GLFW_KEY_H] = "H";
	GLFigure::key_map[GLFW_KEY_I] = "I";
	GLFigure::key_map[GLFW_KEY_J] = "J";
	GLFigure::key_map[GLFW_KEY_K] = "K";
	GLFigure::key_map[GLFW_KEY_L] = "L";
	GLFigure::key_map[GLFW_KEY_M] = "M";
	GLFigure::key_map[GLFW_KEY_N] = "N";
	GLFigure::key_map[GLFW_KEY_O] = "O";
	GLFigure::key_map[GLFW_KEY_P] = "P";
	GLFigure::key_map[GLFW_KEY_Q] = "Q";
	GLFigure::key_map[GLFW_KEY_R] = "R";
	GLFigure::key_map[GLFW_KEY_S] = "S";
	GLFigure::key_map[GLFW_KEY_T] = "T";
	GLFigure::key_map[GLFW_KEY_U] = "U";
	GLFigure::key_map[GLFW_KEY_V] = "V";
	GLFigure::key_map[GLFW_KEY_W] = "W";
	GLFigure::key_map[GLFW_KEY_X] = "X";
	GLFigure::key_map[GLFW_KEY_Y] = "Y";
	GLFigure::key_map[GLFW_KEY_Z] = "Z";
	GLFigure::key_map[GLFW_KEY_LEFT_BRACKET] = "[";
	GLFigure::key_map[GLFW_KEY_BACKSLASH] = "\\";
	GLFigure::key_map[GLFW_KEY_RIGHT_BRACKET] = "]";
	GLFigure::key_map[GLFW_KEY_GRAVE_ACCENT] = "`";
	GLFigure::key_map[GLFW_KEY_ESCAPE] = "ESC";
	GLFigure::key_map[GLFW_KEY_ENTER] = "ENTER";
	GLFigure::key_map[GLFW_KEY_TAB] = "TAB";
	GLFigure::key_map[GLFW_KEY_BACKSPACE] = "BACKSPACE";
	GLFigure::key_map[GLFW_KEY_INSERT] = "INSERT";
	GLFigure::key_map[GLFW_KEY_DELETE] = "DELETE";
	GLFigure::key_map[GLFW_KEY_RIGHT] = "RIGHT";
	GLFigure::key_map[GLFW_KEY_LEFT] = "LEFT";
	GLFigure::key_map[GLFW_KEY_DOWN] = "DOWN";
	GLFigure::key_map[GLFW_KEY_UP] = "UP";
	GLFigure::key_map[GLFW_KEY_PAGE_UP] = "PAGE_UP";
	GLFigure::key_map[GLFW_KEY_PAGE_DOWN] = "PAGE_DOWN";
	GLFigure::key_map[GLFW_KEY_HOME] = "HOME";
	GLFigure::key_map[GLFW_KEY_END] = "END";
	GLFigure::key_map[GLFW_KEY_CAPS_LOCK] = "CAPS_LOCK";
	GLFigure::key_map[GLFW_KEY_SCROLL_LOCK] = "SCROLL_LOCK";
	GLFigure::key_map[GLFW_KEY_NUM_LOCK] = "NUM_LOCK";
	GLFigure::key_map[GLFW_KEY_PRINT_SCREEN] = "PRINT_SCREEN";
	GLFigure::key_map[GLFW_KEY_PAUSE] = "PAUSE";
	GLFigure::key_map[GLFW_KEY_F1] = "F1";
	GLFigure::key_map[GLFW_KEY_F2] = "F2";
	GLFigure::key_map[GLFW_KEY_F3] = "F3";
	GLFigure::key_map[GLFW_KEY_F4] = "F4";
	GLFigure::key_map[GLFW_KEY_F5] = "F5";
	GLFigure::key_map[GLFW_KEY_F6] = "F6";
	GLFigure::key_map[GLFW_KEY_F7] = "F7";
	GLFigure::key_map[GLFW_KEY_F8] = "F8";
	GLFigure::key_map[GLFW_KEY_F9] = "F9";
	GLFigure::key_map[GLFW_KEY_F10] = "F10";
	GLFigure::key_map[GLFW_KEY_F11] = "F11";
	GLFigure::key_map[GLFW_KEY_F12] = "F12";
	GLFigure::key_map[GLFW_KEY_F13] = "F13";
	GLFigure::key_map[GLFW_KEY_F14] = "F14";
	GLFigure::key_map[GLFW_KEY_F15] = "F15";
	GLFigure::key_map[GLFW_KEY_F16] = "F16";
	GLFigure::key_map[GLFW_KEY_F17] = "F17";
	GLFigure::key_map[GLFW_KEY_F18] = "F18";
	GLFigure::key_map[GLFW_KEY_F19] = "F19";
	GLFigure::key_map[GLFW_KEY_F20] = "F20";
	GLFigure::key_map[GLFW_KEY_F21] = "F21";
	GLFigure::key_map[GLFW_KEY_F22] = "F22";
	GLFigure::key_map[GLFW_KEY_F23] = "F23";
	GLFigure::key_map[GLFW_KEY_F24] = "F24";
	GLFigure::key_map[GLFW_KEY_F25] = "F25";
	GLFigure::key_map[GLFW_KEY_KP_0] = "KP_0";
	GLFigure::key_map[GLFW_KEY_KP_1] = "KP_1";
	GLFigure::key_map[GLFW_KEY_KP_2] = "KP_2";
	GLFigure::key_map[GLFW_KEY_KP_3] = "KP_3";
	GLFigure::key_map[GLFW_KEY_KP_4] = "KP_4";
	GLFigure::key_map[GLFW_KEY_KP_5] = "KP_5";
	GLFigure::key_map[GLFW_KEY_KP_6] = "KP_6";
	GLFigure::key_map[GLFW_KEY_KP_7] = "KP_7";
	GLFigure::key_map[GLFW_KEY_KP_8] = "KP_8";
	GLFigure::key_map[GLFW_KEY_KP_9] = "KP_9";
	GLFigure::key_map[GLFW_KEY_KP_DECIMAL] = "KP_.";
	GLFigure::key_map[GLFW_KEY_KP_DIVIDE] = "KP_/";
	GLFigure::key_map[GLFW_KEY_KP_MULTIPLY] = "*";
	GLFigure::key_map[GLFW_KEY_KP_SUBTRACT] = "KP_-";
	GLFigure::key_map[GLFW_KEY_KP_ADD] = "+";
	GLFigure::key_map[GLFW_KEY_KP_ENTER] = "KP_ENTER";
	GLFigure::key_map[GLFW_KEY_KP_EQUAL] = "KP_=";
	GLFigure::key_map[GLFW_KEY_LEFT_SHIFT] = "LEFT_SHIFT";
	GLFigure::key_map[GLFW_KEY_LEFT_CONTROL] = "LEFT_CONTROL";
	GLFigure::key_map[GLFW_KEY_LEFT_ALT] = "LEFT_ALT";
	GLFigure::key_map[GLFW_KEY_LEFT_SUPER] = "LEFT_SUPER";
	GLFigure::key_map[GLFW_KEY_RIGHT_SHIFT] = "RIGHT_SHIFT";
	GLFigure::key_map[GLFW_KEY_RIGHT_CONTROL] = "RIGHT_CONTROL";
	GLFigure::key_map[GLFW_KEY_RIGHT_ALT] = "RIGHT_ALT";
	GLFigure::key_map[GLFW_KEY_RIGHT_SUPER] = "RIGHT_SUPER";
	GLFigure::key_map[GLFW_KEY_MENU] = "MENU";
}

GLFigure::Constructor::~Constructor()
{
	glfwTerminate();
}
GLFigure::Constructor GLFigure::constructor;

void GLFigure::glad_init()
{
	if(!GLFigure::is_glad_init)
	{
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw glass::RuntimeError("Failed to initialize GLAD");
		}
		GLFigure::is_glad_init = true;
	}
}

void GLFigure::mouse_position_callback(GLFWwindow* _window, double x, double y)
{
	glfwGetCursorPos(_window, &__current_x, &__current_y);

	if(left_pressed)
	{
		parent->onDrag();
	}
	else if(middle_pressed)
	{
		parent->onMiddleDrag();
	}
	else if(right_pressed)
	{
		parent->onRightDrag();
	}

	if(mouse_stoped)
	{
		parent->onMouseStartMove();
	}
	parent->onMouseMove();
	mouse_stoped = false;
	
	scheduler.cancle();
	scheduler.schedule(&GLFigure::__mouse_stop_move_callback, 0.2);

	__last_x = x;
	__last_y = y;
}

void GLFigure::__mouse_stop_move_callback()
{
	parent->onMouseStopMove();
	mouse_stoped = true;
}

void GLFigure::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS)
		{
			parent->onLeftPress();
			left_press_time = timer.toc();
			__last_left_press_x = __current_x;
			__last_left_press_y = __current_y;
			left_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			parent->onLeftRelease();
			double current_time = timer.toc();
			if(current_time - left_press_time <= 0.2)
			{
				parent->onClick();
				if(current_time - left_click_time <= 0.4)
				{
					parent->onDoubleClick();
				}
				left_click_time = current_time;
			}
			__last_left_release_x = __current_x;
			__last_left_release_y = __current_y;
			left_pressed = false;
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if(action == GLFW_PRESS)
		{
			parent->onRightPress();
			right_press_time = timer.toc();
			__last_right_press_x = __current_x;
			__last_right_press_y = __current_y;
			right_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			parent->onRightRelease();
			double current_time = timer.toc();
			if(current_time - right_press_time <= 0.2)
			{
				parent->onRightClick();
				if(current_time - right_click_time <= 0.4)
				{
					parent->onRightDoubleClick();
				}
				right_click_time = current_time;
			}
			__last_right_release_x = __current_x;
			__last_right_release_y = __current_y;
			right_pressed = false;
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if(action == GLFW_PRESS)
		{
			parent->onMiddlePress();
			middle_press_time = timer.toc();
			__last_middle_press_x = __current_x;
			__last_middle_press_y = __current_y;
			middle_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			parent->onMiddleRelease();
			double current_time = timer.toc();
			if(current_time - middle_press_time <= 0.2)
			{
				parent->onMiddleClick();
				if(current_time - middle_click_time <= 0.4)
				{
					parent->onMiddleDoubleClick();
				}
				middle_click_time = current_time;
			}
			__last_middle_release_x = __current_x;
			__last_middle_release_y = __current_y;
			middle_pressed = false;
		}
	}
}

void GLFigure::__keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		parent->onKeyPress(key_map[key]);
		parent->onKeyRepeat(key_map[key]);
	}
	else if(action == GLFW_REPEAT)
	{
		parent->onKeyRepeat(key_map[key]);
	}
	else if(action == GLFW_RELEASE)
	{
		parent->onKeyRelease(key_map[key]);
	}
}

void GLFigure::mouse_enter_leave_callback(GLFWwindow* window, int flag)
{
	if(flag)
	{
		parent->onMouseEnter();
	}
	else
	{
		parent->onMouseLeave();
		__last_x = -1;
		__last_y = -1;
	}
}

void GLFigure::__scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	parent->onScroll(yoffset);
}

int GLFigure::current_x()
{
	return __current_x;
}

int GLFigure::current_y()
{
	return __current_y;
}

double GLFigure::current_t()
{
	return __current_t;
}

int GLFigure::last_x()
{
	return __last_x != -1 ? __last_x : __current_x;
}

int GLFigure::last_y()
{
	return __last_y != -1 ? __last_y : __current_y;
}

double GLFigure::last_t()
{
	return __last_t != -1 ? __last_t : __current_t;
}

int GLFigure::dx()
{
	return __current_x - last_x();
}

int GLFigure::dy()
{
	return __current_y - last_y();
}

double GLFigure::dt()
{
	return __current_t - last_t();
}

double GLFigure::fps()
{
	return 1.0 / (__current_t - last_t());
}

int GLFigure::last_left_press_x()
{
	return __last_left_press_x;
}

int GLFigure::last_left_press_y()
{
	return __last_left_press_y;
}

int GLFigure::last_left_release_x()
{
	return __last_left_release_x;
}

int GLFigure::last_left_release_y()
{
	return __last_left_release_y;
}

int GLFigure::last_right_press_x()
{
	return __last_right_press_x;
}

int GLFigure::last_right_press_y()
{
	return __last_right_press_y;
}

int GLFigure::last_right_release_x()
{
	return __last_right_release_x;
}

int GLFigure::last_right_release_y()
{
	return __last_right_release_y;
}

int GLFigure::last_middle_press_x()
{
	return __last_middle_press_x;
}

int GLFigure::last_middle_press_y()
{
	return __last_middle_press_y;
}

int GLFigure::last_middle_release_x()
{
	return __last_middle_release_x;
}

int GLFigure::last_middle_release_y()
{
	return __last_middle_release_y;
}

bool GLFigure::is_left_pressed()
{
	return left_pressed;
}

bool GLFigure::is_middle_pressed()
{
	return middle_pressed;
}

bool GLFigure::is_right_pressed()
{
	return right_pressed;
}

void GLFigure::onDrag() {}
void GLFigure::onRightDrag() {}
void GLFigure::onMiddleDrag() {}
void GLFigure::onMouseStartMove() {}
void GLFigure::onMouseMove() {}
void GLFigure::onMouseStopMove() {}
void GLFigure::onDoubleClick() {}
void GLFigure::onRightDoubleClick() {}
void GLFigure::onMiddleDoubleClick() {}
void GLFigure::onClick() {}
void GLFigure::onRightClick() {}
void GLFigure::onMiddleClick() {}
void GLFigure::onLeftPress() {}
void GLFigure::onRightPress() {}
void GLFigure::onMiddlePress() {}
void GLFigure::onLeftRelease() {}
void GLFigure::onRightRelease() {}
void GLFigure::onMiddleRelease() {}
void GLFigure::onScroll(int scroll_step) {}
void GLFigure::onMouseEnter() {}
void GLFigure::onMouseLeave() {}
void GLFigure::onKeyPress(const string& key) {}
void GLFigure::onKeyRelease(const string& key) {}
void GLFigure::onKeyRepeat(const string& key) {}

void GLFigure::__change_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	parent->__width = width;
	parent->__height = height;
	parent->onChangeSize(width, height);
}

void GLFigure::onChangeSize(int width, int height) {}

void GLFigure::prepare() {}
void GLFigure::draw() {}
void GLFigure::update() {}

void GLFigure::autoClear(bool flag)
{
	__auto_clear = flag;
}

bool GLFigure::autoClear()
{
	return __auto_clear;
}

void GLFigure::show()
{
	prepare();
	glfwShowWindow(__window);

	glEnable(GL_MULTISAMPLE); // 抗锯齿
	glEnable(GL_DEPTH_TEST); // 深度测试
	// glEnable(GL_FRAMEBUFFER_SRGB); // Gamma 校正

	while(!glfwWindowShouldClose(__window))
	{
		__current_t = timer.toc();

		if(__auto_clear)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		update();
		draw();
		
		glfwSwapBuffers(__window);
    	glfwPollEvents();

    	__last_t = __current_t;
	}
}

void GLFigure::setIcon(const Image& image)
{
	GLFWimage icon;
	icon.width = image.cols();
	icon.height = image.rows();
	icon.pixels = new unsigned char[image.size()];
	memcpy((void*)icon.pixels, (void*)(image.data()), image.size() * sizeof(unsigned char));

	glfwSetWindowIcon(__window, 1, &icon);
}

void GLFigure::setIcon(const string& filename)
{
	Image image(filename);

	GLFWimage icon;
	icon.width = image.cols();
	icon.height = image.rows();
	icon.pixels = new unsigned char[image.size()];
	memcpy((void*)icon.pixels, (void*)(image.data()), image.size() * sizeof(unsigned char));

	glfwSetWindowIcon(__window, 1, &icon);
}

GLFigure::GLFigure(int width, int height, const string& name, const vec4& color) :
__width(width), __height(height), __color(color), __name(name)
{
	GLFigure::parent = this;

	glfwWindowHint(GLFW_SAMPLES, 8);
	__window = glfwCreateWindow(__width, __height, __name.c_str(), NULL, NULL);
	if(__window == NULL)
	{
		throw glass::RuntimeError("Failed to create GLFW window!");
	}
	glfwMakeContextCurrent(__window);

	// Callbacks
	glfwSetFramebufferSizeCallback(__window, GLFigure::__change_size_callback);
	glfwSetCursorEnterCallback(__window, GLFigure::mouse_enter_leave_callback);
	glfwSetCursorPosCallback(__window, GLFigure::mouse_position_callback);
	glfwSetMouseButtonCallback(__window, GLFigure::mouse_button_callback);
	glfwSetScrollCallback(__window, GLFigure::__scroll_callback);
	glfwSetKeyCallback(__window, GLFigure::__keyboard_callback);

	// GLFWimage icons[2];
	// icons[0] = getGLFWimage(Image("static/resources/images/icons/icon.png"));
	// icons[1] = getGLFWimage(Image("static/resources/images/icons/icon_small.png"));
	// glfwSetWindowIcon(__window, 2, icons);

	glad_init();
	glViewport(0, 0, __width, __height);
	glClearColor(__color.x, __color.y, __color.z, __color.w);
}

GLFigure::GLFigure(GLFigure&& figure)
{
	if(this == &figure)
	{
		return;
	}

	__name = move(figure.__name);
	__width = move(figure.__width);
	__height = move(figure.__height);
	__color = move(figure.__color);
	__window = move(figure.__window);

	parent = this;
	figure.__name = "";
	figure.__width = 0;
	figure.__height = 0;
	figure.__window = NULL;
}

GLFWwindow* GLFigure::window()
{
	return __window;
}

uint GLFigure::width()const
{
	return __width;
}

uint GLFigure::height()const
{
	return __height;
}

GLFigure::~GLFigure() {}