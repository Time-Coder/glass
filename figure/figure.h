#ifndef __FIGURE_H__
#define __FIGURE_H__

#include <iostream>
#include <color.h>
#include <gl_objects.h>
#include <tictoc.hpp>

using namespace std;

#ifndef TYPE_UINT
typedef unsigned int uint;
#define TYPE_UINT
#endif

class GLFigure
{
protected:
	string __name = "Figure";
	int __width = 800;
	int __height = 600;
	bool __auto_clear = true;
	Color __color = Color(0.5, 0.5, 0.5);
	GLFWwindow* __window = NULL;
	static GLFigure* parent;

private:
	static Timer timer;
	static Scheduler scheduler;
	static double __current_x;
	static double __current_y;
	static double __current_t;
	static double __last_x;
	static double __last_y;
	static double __last_t;
	static double __last_left_press_x;
	static double __last_left_press_y;
	static double __last_left_release_x;
	static double __last_left_release_y;
	static double __last_right_press_x;
	static double __last_right_press_y;
	static double __last_right_release_x;
	static double __last_right_release_y;
	static double __last_middle_press_x;
	static double __last_middle_press_y;
	static double __last_middle_release_x;
	static double __last_middle_release_y;
	static bool left_pressed;
	static bool middle_pressed;
	static bool right_pressed;
	static double left_press_time;
	static double right_press_time;
	static double middle_press_time;
	static double left_click_time;
	static double right_click_time;
	static double middle_click_time;
	static bool mouse_stoped;
	static map<uint, string> key_map;
	static bool __is_key_map_init;

private:
	static void init_key_map();
	static void mouse_position_callback(GLFWwindow* _window, double x, double y);
	static void mouse_button_callback(GLFWwindow* _window, int button, int action, int mods);
	static void __scroll_callback(GLFWwindow* _window, double xoffset, double yoffset);
	static void __mouse_stop_move_callback();
	static void __change_size_callback(GLFWwindow* _window, int width, int height);
	static void mouse_enter_leave_callback(GLFWwindow* _window, int flag);
	static void __keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

protected:
	static int current_x();
	static int current_y();
	static double current_t();
	static int last_x();
	static int last_y();
	static double last_t();
	static int dx();
	static int dy();
	static double dt();
	static int last_left_press_x();
	static int last_left_press_y();
	static int last_left_release_x();
	static int last_left_release_y();
	static int last_right_press_x();
	static int last_right_press_y();
	static int last_right_release_x();
	static int last_right_release_y();
	static int last_middle_press_x();
	static int last_middle_press_y();
	static int last_middle_release_x();
	static int last_middle_release_y();
	static bool is_left_pressed();
	static bool is_middle_pressed();
	static bool is_right_pressed();

	virtual void key_press_callback(const string& key);
	virtual void key_release_callback(const string& key);
	virtual void key_repeat_callback(const string& key);
	virtual void drag_callback();
	virtual void right_drag_callback();
	virtual void middle_drag_callback();
	virtual void mouse_leave_callback();
	virtual void mouse_enter_callback();
	virtual void mouse_start_move_callback();
	virtual void mouse_move_callback();
	virtual void mouse_stop_move_callback();
	virtual void double_click_callback();
	virtual void right_double_click_callback();
	virtual void middle_double_click_callback();
	virtual void click_callback();
	virtual void right_click_callback();
	virtual void middle_click_callback();
	virtual void left_press_callback();
	virtual void right_press_callback();
	virtual void middle_press_callback();
	virtual void left_release_callback();
	virtual void right_release_callback();
	virtual void middle_release_callback();
	virtual void scroll_callback(int scroll_step);
	virtual void change_size_callback(int width, int height);

public:
	GLFigure(int width = 800, int height = 600, const string& name = "Figure", const Color& color = Color(0.8, 0.8, 0.8));
	GLFigure(GLFigure&& figure);
	~GLFigure();
	void auto_clear(bool flag);
	bool auto_clear();
	GLFWwindow* window();
	uint width()const;
	uint height()const;
	virtual void prepare();
	virtual void draw();
	void show();
};

#endif