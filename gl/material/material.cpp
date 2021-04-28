#include "material.h"
#include "str.h"

Material::Material(string name)
{
	name = str::lower(name);
	if(name == "emerald") // 翡翠
	{
		ambient_color = vec3(0.0215, 0.1745, 0.0215);
		diffuse_color = vec3(0.07568, 0.61424, 0.07568);
		specular_color = vec3(0.633, 0.727811, 0.633);
		shininess = 0.6;
	}
	else if(name == "jade") // 玉
	{
		ambient_color = vec3(0.135, 0.2225, 0.1575);
		diffuse_color = vec3(0.54, 0.89, 0.63);
		specular_color = vec3(0.316228, 0.316228, 0.316228);
		shininess = 0.1;
	}
	else if(name == "obsidian") // 黑曜石
	{
		ambient_color = vec3(0.05375, 0.05, 0.06625);
		diffuse_color = vec3(0.18275, 0.17, 0.22525);
		specular_color = vec3(0.332741, 0.328634, 0.346435);
		shininess = 0.3;
	}
	else if(name == "pearl") // 珍珠
	{
		ambient_color = vec3(0.25, 0.20725, 0.20725);
		diffuse_color = vec3(1, 0.829, 0.829);
		specular_color = vec3(0.296648, 0.296648, 0.296648);
		shininess = 0.088;
	}
	else if(name == "ruby") // 红宝石
	{
		ambient_color = vec3(0.1745, 0.01175, 0.01175);
		diffuse_color = vec3(0.61424, 0.04136, 0.04136);
		specular_color = vec3(0.727811, 0.626959, 0.626959);
		shininess = 0.6;
	}
	else if(name == "turquoise") // 绿松石
	{
		ambient_color = vec3(0.1, 0.18725, 0.1745);
		diffuse_color = vec3(0.396, 0.74151, 0.69102);
		specular_color = vec3(0.297254, 0.30829, 0.306678);
		shininess = 0.1;
	}
	else if(name == "brass") // 黄铜
	{
		ambient_color = vec3(0.329412, 0.223529, 0.027451);
		diffuse_color = vec3(0.780392, 0.568627, 0.113725);
		specular_color = vec3(0.992157, 0.941176, 0.807843);
		shininess = 0.21794872;
	}
	else if(name == "bronze") // 青铜
	{
		ambient_color = vec3(0.2125, 0.1275, 0.054);
		diffuse_color = vec3(0.714, 0.4284, 0.18144);
		specular_color = vec3(0.393548, 0.271906, 0.166721);
		shininess = 0.2;
	}
	else if(name == "chrome") // 铬
	{
		ambient_color = vec3(0.25, 0.25, 0.25);
		diffuse_color = vec3(0.4, 0.4, 0.4);
		specular_color = vec3(0.774597, 0.774597, 0.774597);
		shininess = 0.6;
	}
	else if(name == "copper") // 铜
	{
		ambient_color = vec3(0.19125, 0.0735, 0.0225);
		diffuse_color = vec3(0.7038, 0.27048, 0.0828);
		specular_color = vec3(0.256777, 0.137622, 0.086014);
		shininess = 0.1;
	}
	else if(name == "gold") // 金
	{
		ambient_color = vec3(0.24725, 0.1995, 0.0745);
		diffuse_color = vec3(0.75164, 0.60648, 0.22648);
		specular_color = vec3(0.628281, 0.555802, 0.366065);
		shininess = 0.4;
	}
	else if(name == "silver") // 银
	{
		ambient_color = vec3(0.19225, 0.19225, 0.19225);
		diffuse_color = vec3(0.50754, 0.50754, 0.50754);
		specular_color = vec3(0.508273, 0.508273, 0.508273);
		shininess = 0.4;
	}
	else if(name == "black_plastic") // 黑色塑料
	{
		ambient_color = vec3(0.0, 0.0, 0.0);
		diffuse_color = vec3(0.01, 0.01, 0.01);
		specular_color = vec3(0.50, 0.50, 0.50);
		shininess = 0.25;
	}
	else if(name == "cyan_plastic") // 青色塑料
	{
		ambient_color = vec3(0.0, 0.1, 0.06);
		diffuse_color = vec3(0.0, 0.50980392, 0.50980392);
		specular_color = vec3(0.50196078, 0.50196078, 0.50196078);
		shininess = 0.25;
	}
	else if(name == "green_plastic") // 绿色塑料
	{
		ambient_color = vec3(0.0, 0.0, 0.0);
		diffuse_color = vec3(0.1, 0.35, 0.1);
		specular_color = vec3(0.45, 0.55, 0.45);
		shininess = 0.25;
	}
	else if(name == "red_plastic") // 红色塑料
	{
		ambient_color = vec3(0.0, 0.0, 0.0);
		diffuse_color = vec3(0.5, 0.0, 0.0);
		specular_color = vec3(0.7, 0.6, 0.6);
		shininess = 0.25;
	}
	else if(name == "white_plastic") // 白色塑料
	{
		ambient_color = vec3(0.0, 0.0, 0.0);
		diffuse_color = vec3(0.55, 0.55, 0.55);
		specular_color = vec3(0.70, 0.70, 0.70);
		shininess = 0.25;
	}
	else if(name == "yellow_plastic") // 黄色塑料
	{
		ambient_color = vec3(0.0, 0.0, 0.0);
		diffuse_color = vec3(0.5, 0.5, 0.0);
		specular_color = vec3(0.60, 0.60, 0.50);
		shininess = 0.25;
	}
	else if(name == "black_rubber") // 黑色橡胶
	{
		ambient_color = vec3(0.02, 0.02, 0.02);
		diffuse_color = vec3(0.01, 0.01, 0.01);
		specular_color = vec3(0.4, 0.4, 0.4);
		shininess = 0.078125;
	}
	else if(name == "cyan_rubber") // 青色橡胶
	{
		ambient_color = vec3(0.0, 0.05, 0.05);
		diffuse_color = vec3(0.4, 0.5, 0.5);
		specular_color = vec3(0.04, 0.7, 0.7);
		shininess = 0.078125;
	}
	else if(name == "green_rubber") // 绿色橡胶
	{
		ambient_color = vec3(0.0, 0.05, 0.0);
		diffuse_color = vec3(0.4, 0.5, 0.4);
		specular_color = vec3(0.04, 0.7, 0.04);
		shininess = 0.078125;
	}
	else if(name == "red_rubber") // 红色橡胶
	{
		ambient_color = vec3(0.05, 0.0, 0.0);
		diffuse_color = vec3(0.5, 0.4, 0.4);
		specular_color = vec3(0.7, 0.04, 0.04);
		shininess = 0.078125;
	}
	else if(name == "white_rubber") // 白色橡胶
	{
		ambient_color = vec3(0.05, 0.05, 0.05);
		diffuse_color = vec3(0.5, 0.5, 0.5);
		specular_color = vec3(0.7, 0.7, 0.7);
		shininess = 0.078125;
	}
	else if(name == "yellow_rubber") // 黄色橡胶
	{
		ambient_color = vec3(0.05, 0.05, 0.0);
		diffuse_color = vec3(0.5, 0.5, 0.4);
		specular_color = vec3(0.7, 0.7, 0.04);
		shininess = 0.078125;
	}
	else // default: 铬
	{
		ambient_color = vec3(0.25, 0.25, 0.25);
		diffuse_color = vec3(0.4, 0.4, 0.4);
		specular_color = vec3(0.774597, 0.774597, 0.774597);
		shininess = 0.2;
	}
}

void Material::set_ambient_map(const string& filename)
{
	ambient_map.addImage(filename);
	using_ambient_map = true;
}

void Material::set_reflection_map(const string& filename)
{
	reflection_map.addImage(filename);
	using_reflection_map = true;
}

void Material::set_diffuse_map(const string& filename)
{
	diffuse_map.addImage(filename);
	using_diffuse_map = true;
}

void Material::set_specular_map(const string& filename)
{
	specular_map.addImage(filename);
	using_specular_map = true;
}

void Material::set_depth_map(const string& filename, float factor)
{
	depth_map.addImage(filename);
	using_depth_map = true;
	height_factor = factor;
}

void Material::set_normal_map(const string& filename)
{
	normal_map.addImage(filename);
	using_normal_map = true;
}

void Material::enable_ambient_map()
{
	using_ambient_map = true;
}

void Material::enable_diffuse_map()
{
	using_diffuse_map = true;
}

void Material::enable_specular_map()
{
	using_specular_map = true;
}

void Material::enable_reflection_map()
{
	using_reflection_map = true;
}

void Material::enable_depth_map()
{
	using_depth_map = true;
}

void Material::enable_normal_map()
{
	using_normal_map = true;
}

void Material::disable_ambient_map()
{
	using_ambient_map = false;
}

void Material::disable_diffuse_map()
{
	using_diffuse_map = false;
}

void Material::disable_specular_map()
{
	using_specular_map = false;
}

void Material::disable_reflection_map()
{
	using_reflection_map = false;
}

void Material::disable_depth_map()
{
	using_depth_map = false;
}

void Material::disable_normal_map()
{
	using_normal_map = false;
}

void Material::ls()
{
	cout << "emerald" << endl
	     << "jade" << endl
	     << "obsidian" << endl
	     << "pearl" << endl
	     << "ruby" << endl
	     << "turquoise" << endl
	     << "brass" << endl
	     << "bronze" << endl
	     << "chrome" << endl
	     << "copper" << endl
	     << "gold" << endl
	     << "silver" << endl
	     << "black_plastic" << endl
	     << "cyan_plastic" << endl
	     << "green_plastic" << endl
	     << "red_plastic" << endl
	     << "white_plastic" << endl
	     << "yellow_plastic" << endl
	     << "black_rubber" << endl
	     << "cyan_rubber" << endl
	     << "green_rubber" << endl
	     << "red_rubber" << endl
	     << "white_rubber" << endl
	     << "yellow_rubber" << endl;
}

vector<string> Material::list()
{
	return vector<string>(
		{
			string("emerald"),
			string("jade"),
			string("obsidian"),
			string("pearl"),
			string("ruby"),
			string("turquoise"),
			string("brass"),
			string("bronze"),
			string("chrome"),
			string("copper"),
			string("gold"),
			string("silver"),
			string("black_plastic"),
			string("cyan_plastic"),
			string("green_plastic"),
			string("red_plastic"),
			string("white_plastic"),
			string("yellow_plastic"),
			string("black_rubber"),
			string("cyan_rubber"),
			string("green_rubber"),
			string("red_rubber"),
			string("white_rubber"),
			string("yellow_rubber")
		}
	);
}