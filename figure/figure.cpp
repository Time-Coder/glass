#include <figure.h>
#include <image.h>

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
bool GLFigure::__is_key_map_init = false;
map<uint, string> GLFigure::key_map;

void GLFigure::init_key_map()
{
	if(__is_key_map_init)
	{
		return;
	}

	key_map[GLFW_KEY_SPACE] = " ";
	key_map[GLFW_KEY_APOSTROPHE] = "'";
	key_map[GLFW_KEY_COMMA] = ",";
	key_map[GLFW_KEY_MINUS] = "-";
	key_map[GLFW_KEY_PERIOD] = ".";
	key_map[GLFW_KEY_SLASH] = "/";
	key_map[GLFW_KEY_0] = "0";
	key_map[GLFW_KEY_1] = "1";
	key_map[GLFW_KEY_2] = "2";
	key_map[GLFW_KEY_3] = "3";
	key_map[GLFW_KEY_4] = "4";
	key_map[GLFW_KEY_5] = "5";
	key_map[GLFW_KEY_6] = "6";
	key_map[GLFW_KEY_7] = "7";
	key_map[GLFW_KEY_8] = "8";
	key_map[GLFW_KEY_9] = "9";
	key_map[GLFW_KEY_SEMICOLON] = ";";
	key_map[GLFW_KEY_EQUAL] = "=";
	key_map[GLFW_KEY_A] = "A";
	key_map[GLFW_KEY_B] = "B";
	key_map[GLFW_KEY_C] = "C";
	key_map[GLFW_KEY_D] = "D";
	key_map[GLFW_KEY_E] = "E";
	key_map[GLFW_KEY_F] = "F";
	key_map[GLFW_KEY_G] = "G";
	key_map[GLFW_KEY_H] = "H";
	key_map[GLFW_KEY_I] = "I";
	key_map[GLFW_KEY_J] = "J";
	key_map[GLFW_KEY_K] = "K";
	key_map[GLFW_KEY_L] = "L";
	key_map[GLFW_KEY_M] = "M";
	key_map[GLFW_KEY_N] = "N";
	key_map[GLFW_KEY_O] = "O";
	key_map[GLFW_KEY_P] = "P";
	key_map[GLFW_KEY_Q] = "Q";
	key_map[GLFW_KEY_R] = "R";
	key_map[GLFW_KEY_S] = "S";
	key_map[GLFW_KEY_T] = "T";
	key_map[GLFW_KEY_U] = "U";
	key_map[GLFW_KEY_V] = "V";
	key_map[GLFW_KEY_W] = "W";
	key_map[GLFW_KEY_X] = "X";
	key_map[GLFW_KEY_Y] = "Y";
	key_map[GLFW_KEY_Z] = "Z";
	key_map[GLFW_KEY_LEFT_BRACKET] = "[";
	key_map[GLFW_KEY_BACKSLASH] = "\\";
	key_map[GLFW_KEY_RIGHT_BRACKET] = "]";
	key_map[GLFW_KEY_GRAVE_ACCENT] = "`";
	key_map[GLFW_KEY_ESCAPE] = "ESC";
	key_map[GLFW_KEY_ENTER] = "ENTER";
	key_map[GLFW_KEY_TAB] = "TAB";
	key_map[GLFW_KEY_BACKSPACE] = "BACKSPACE";
	key_map[GLFW_KEY_INSERT] = "INSERT";
	key_map[GLFW_KEY_DELETE] = "DELETE";
	key_map[GLFW_KEY_RIGHT] = "RIGHT";
	key_map[GLFW_KEY_LEFT] = "LEFT";
	key_map[GLFW_KEY_DOWN] = "DOWN";
	key_map[GLFW_KEY_UP] = "UP";
	key_map[GLFW_KEY_PAGE_UP] = "PAGE_UP";
	key_map[GLFW_KEY_PAGE_DOWN] = "PAGE_DOWN";
	key_map[GLFW_KEY_HOME] = "HOME";
	key_map[GLFW_KEY_END] = "END";
	key_map[GLFW_KEY_CAPS_LOCK] = "CAPS_LOCK";
	key_map[GLFW_KEY_SCROLL_LOCK] = "SCROLL_LOCK";
	key_map[GLFW_KEY_NUM_LOCK] = "NUM_LOCK";
	key_map[GLFW_KEY_PRINT_SCREEN] = "PRINT_SCREEN";
	key_map[GLFW_KEY_PAUSE] = "PAUSE";
	key_map[GLFW_KEY_F1] = "F1";
	key_map[GLFW_KEY_F2] = "F2";
	key_map[GLFW_KEY_F3] = "F3";
	key_map[GLFW_KEY_F4] = "F4";
	key_map[GLFW_KEY_F5] = "F5";
	key_map[GLFW_KEY_F6] = "F6";
	key_map[GLFW_KEY_F7] = "F7";
	key_map[GLFW_KEY_F8] = "F8";
	key_map[GLFW_KEY_F9] = "F9";
	key_map[GLFW_KEY_F10] = "F10";
	key_map[GLFW_KEY_F11] = "F11";
	key_map[GLFW_KEY_F12] = "F12";
	key_map[GLFW_KEY_F13] = "F13";
	key_map[GLFW_KEY_F14] = "F14";
	key_map[GLFW_KEY_F15] = "F15";
	key_map[GLFW_KEY_F16] = "F16";
	key_map[GLFW_KEY_F17] = "F17";
	key_map[GLFW_KEY_F18] = "F18";
	key_map[GLFW_KEY_F19] = "F19";
	key_map[GLFW_KEY_F20] = "F20";
	key_map[GLFW_KEY_F21] = "F21";
	key_map[GLFW_KEY_F22] = "F22";
	key_map[GLFW_KEY_F23] = "F23";
	key_map[GLFW_KEY_F24] = "F24";
	key_map[GLFW_KEY_F25] = "F25";
	key_map[GLFW_KEY_KP_0] = "KP_0";
	key_map[GLFW_KEY_KP_1] = "KP_1";
	key_map[GLFW_KEY_KP_2] = "KP_2";
	key_map[GLFW_KEY_KP_3] = "KP_3";
	key_map[GLFW_KEY_KP_4] = "KP_4";
	key_map[GLFW_KEY_KP_5] = "KP_5";
	key_map[GLFW_KEY_KP_6] = "KP_6";
	key_map[GLFW_KEY_KP_7] = "KP_7";
	key_map[GLFW_KEY_KP_8] = "KP_8";
	key_map[GLFW_KEY_KP_9] = "KP_9";
	key_map[GLFW_KEY_KP_DECIMAL] = "KP_.";
	key_map[GLFW_KEY_KP_DIVIDE] = "KP_/";
	key_map[GLFW_KEY_KP_MULTIPLY] = "*";
	key_map[GLFW_KEY_KP_SUBTRACT] = "KP_-";
	key_map[GLFW_KEY_KP_ADD] = "+";
	key_map[GLFW_KEY_KP_ENTER] = "KP_ENTER";
	key_map[GLFW_KEY_KP_EQUAL] = "KP_=";
	key_map[GLFW_KEY_LEFT_SHIFT] = "LEFT_SHIFT";
	key_map[GLFW_KEY_LEFT_CONTROL] = "LEFT_CONTROL";
	key_map[GLFW_KEY_LEFT_ALT] = "LEFT_ALT";
	key_map[GLFW_KEY_LEFT_SUPER] = "LEFT_SUPER";
	key_map[GLFW_KEY_RIGHT_SHIFT] = "RIGHT_SHIFT";
	key_map[GLFW_KEY_RIGHT_CONTROL] = "RIGHT_CONTROL";
	key_map[GLFW_KEY_RIGHT_ALT] = "RIGHT_ALT";
	key_map[GLFW_KEY_RIGHT_SUPER] = "RIGHT_SUPER";
	key_map[GLFW_KEY_MENU] = "MENU";

	__is_key_map_init = true;
}

void GLFigure::mouse_position_callback(GLFWwindow* _window, double x, double y)
{
	glfwGetCursorPos(_window, &__current_x, &__current_y);

	if(left_pressed)
	{
		parent->drag_callback();
	}
	else if(middle_pressed)
	{
		parent->middle_drag_callback();
	}
	else if(right_pressed)
	{
		parent->right_drag_callback();
	}

	if(mouse_stoped)
	{
		parent->mouse_start_move_callback();
	}
	parent->mouse_move_callback();
	mouse_stoped = false;
	
	scheduler.cancle();
	scheduler.schedule(&GLFigure::__mouse_stop_move_callback, 0.2);

	__last_x = x;
	__last_y = y;
}

void GLFigure::__mouse_stop_move_callback()
{
	parent->mouse_stop_move_callback();
	mouse_stoped = true;
}

void GLFigure::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS)
		{
			parent->left_press_callback();
			left_press_time = timer.toc();
			__last_left_press_x = __current_x;
			__last_left_press_y = __current_y;
			left_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			parent->left_release_callback();
			double current_time = timer.toc();
			if(current_time - left_press_time <= 0.2)
			{
				parent->click_callback();
				if(current_time - left_click_time <= 0.4)
				{
					parent->double_click_callback();
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
			parent->right_press_callback();
			right_press_time = timer.toc();
			__last_right_press_x = __current_x;
			__last_right_press_y = __current_y;
			right_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			parent->right_release_callback();
			double current_time = timer.toc();
			if(current_time - right_press_time <= 0.2)
			{
				parent->right_click_callback();
				if(current_time - right_click_time <= 0.4)
				{
					parent->right_double_click_callback();
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
			parent->middle_press_callback();
			middle_press_time = timer.toc();
			__last_middle_press_x = __current_x;
			__last_middle_press_y = __current_y;
			middle_pressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			parent->middle_release_callback();
			double current_time = timer.toc();
			if(current_time - middle_press_time <= 0.2)
			{
				parent->middle_click_callback();
				if(current_time - middle_click_time <= 0.4)
				{
					parent->middle_double_click_callback();
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
		parent->key_press_callback(key_map[key]);
		parent->key_repeat_callback(key_map[key]);
	}
	else if(action == GLFW_REPEAT)
	{
		parent->key_repeat_callback(key_map[key]);
	}
	else if(action == GLFW_RELEASE)
	{
		parent->key_release_callback(key_map[key]);
	}
}

void GLFigure::mouse_enter_leave_callback(GLFWwindow* window, int flag)
{
	if(flag)
	{
		parent->mouse_enter_callback();
	}
	else
	{
		parent->mouse_leave_callback();
		__last_x = -1;
		__last_y = -1;
	}
}

void GLFigure::__scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	parent->scroll_callback(yoffset);
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

void GLFigure::drag_callback() {}
void GLFigure::right_drag_callback() {}
void GLFigure::middle_drag_callback() {}
void GLFigure::mouse_start_move_callback() {}
void GLFigure::mouse_move_callback() {}
void GLFigure::mouse_stop_move_callback() {}
void GLFigure::double_click_callback() {}
void GLFigure::right_double_click_callback() {}
void GLFigure::middle_double_click_callback() {}
void GLFigure::click_callback() {}
void GLFigure::right_click_callback() {}
void GLFigure::middle_click_callback() {}
void GLFigure::left_press_callback() {}
void GLFigure::right_press_callback() {}
void GLFigure::middle_press_callback() {}
void GLFigure::left_release_callback() {}
void GLFigure::right_release_callback() {}
void GLFigure::middle_release_callback() {}
void GLFigure::scroll_callback(int scroll_step) {}
void GLFigure::mouse_enter_callback() {}
void GLFigure::mouse_leave_callback() {}
void GLFigure::key_press_callback(const string& key) {}
void GLFigure::key_release_callback(const string& key) {}
void GLFigure::key_repeat_callback(const string& key) {}

void GLFigure::__change_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	parent->__width = width;
	parent->__height = height;
	parent->change_size_callback(width, height);
}

void GLFigure::change_size_callback(int width, int height) {}

void GLFigure::prepare() {}
void GLFigure::draw() {}

void GLFigure::auto_clear(bool flag)
{
	__auto_clear = flag;
}

bool GLFigure::auto_clear()
{
	return __auto_clear;
}

void GLFigure::show()
{
	glfwWindowHint(GLFW_SAMPLES, 8);
	__window = glfwCreateWindow(__width, __height, __name.c_str(), NULL, NULL);
	if(__window == NULL)
	{
		throw RuntimeError("Failed to create GLFW window!");
	}
	glfwMakeContextCurrent(__window);

	// Callbacks
	glfwSetFramebufferSizeCallback(__window, GLFigure::__change_size_callback);
	glfwSetCursorEnterCallback(__window, GLFigure::mouse_enter_leave_callback);
	glfwSetCursorPosCallback(__window, GLFigure::mouse_position_callback);
	glfwSetMouseButtonCallback(__window, GLFigure::mouse_button_callback);
	glfwSetScrollCallback(__window, GLFigure::__scroll_callback);
	glfwSetKeyCallback(__window, GLFigure::__keyboard_callback);
	
	GLFWimage icons[2];
	icons[0] = Image("resources/images/icons/icon.png");
	icons[1] = Image("resources/images/icons/icon_small.png");
	glfwSetWindowIcon(__window, 2, icons);

	GLAD::init();
	glViewport(0, 0, __width, __height);

	prepare();
	glfwShowWindow(__window);

	glEnable(GL_MULTISAMPLE); // 抗锯齿
	glEnable(GL_DEPTH_TEST); // 深度测试
	// glEnable(GL_FRAMEBUFFER_SRGB); // Gamma 校正

	glClearColor(__color.R, __color.G, __color.B, __color.A);
	while(!glfwWindowShouldClose(__window))
	{
		__current_t = timer.toc();

		if(__auto_clear)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		draw();
		
		glfwSwapBuffers(__window);
    	glfwPollEvents();

    	__last_t = __current_t;
	}
}

GLFigure::GLFigure(int width, int height, const string& name, const Color& color) :
__width(width), __height(height), __color(color), __name(name)
{
	init_key_map();
	GLFW::init();
	GLFigure::parent = this;
}

GLFigure::GLFigure(GLFigure&& figure)
{
	init_key_map();
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

GLFigure::~GLFigure()
{
    glfwTerminate();
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