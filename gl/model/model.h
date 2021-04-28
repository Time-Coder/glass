#ifndef __MODEL_H__
#define __MODEL_H__

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "vec.h"
#include "camera.h"
#include "material.h"

struct Model
{
	// for glsl
	mat4 mat;
	mat3 normal_mat;
	Material material;
	// end

	Layout layout;

private:
	vec3 __position;
	vec3 __origin; // 旋转中心在上级模型坐标系的坐标
	vec3 __center; // 旋转中心在当前模型坐标系的坐标
	vec3 __zoom = vec3(1, 1, 1);

	double __yaw  = 0.0f, __pitch = 0.0f, __roll = 0.0f;

	Camera* camera = NULL;
	double __relative_pitch = 0.0f;

	Model* parent = NULL;
	map<string, Model> models;
	string base_dir = ".";

	mat4 __mat();
	mat4 _mat();
	mat3 _normal_mat();
	void update_mat();

	void processMesh(aiMesh *mesh, const aiScene *scene);
	void processNode(aiNode *node, const aiScene *scene);
	void load_ambient_map(aiMaterial* ptr_material);
	void load_reflection_map(aiMaterial* ptr_material);
	void load_diffuse_map(aiMaterial* ptr_material);
	void load_specular_map(aiMaterial* ptr_material);
	void load_depth_map(aiMaterial* ptr_material);
	void load_normal_map(aiMaterial* ptr_material);

public:
	void reset();
	void setCamera(Camera& _camera);
	
	void yaw(double dpsi); // 偏航
	void pitch(double dtheta); // 俯仰
	void roll(double dphi); // 滚转
	double yaw()const; // 偏航
	double pitch()const; // 俯仰
	double roll()const; // 滚转
	void yaw_to(double _psi); // 偏航
	void pitch_to(double _theta); // 俯仰
	void roll_to(double _phi); // 滚转

	void move(double dx, double dy, double dz);
	void move(const vec3& v);
	void move_to(double x, double y, double z);
	void move_to(const vec3& v);
	void move_x(double dx);
	void move_x_to(double x);
	void move_y(double dy);
	void move_y_to(double y);
	void move_z(double dz);
	void move_z_to(double z);
	vec3 position()const;
	void position(double x, double y, double z);
	void position(const vec3& v);
	vec3 origin()const;
	void origin(const vec3& v);
	void origin(double x, double y, double z);
	vec3 center()const;
	void center(const vec3& v);
	void center(double x, double y, double z);
	vec3 zoom()const;
	void zoom(const vec3& v);
	void zoom_to(const vec3& v);
	void zoom_to(double sx, double sy, double sz);
	void zoom_x(double sx);
	void zoom_y(double sy);
	void zoom_z(double sz);
	void zoom_x_to(double sx);
	void zoom_y_to(double sy);
	void zoom_z_to(double sz);

	double relative_pitch()const;
	void relative_pitch(double _pitch);
	void relative_pitch_to(double _pitch);
	
	Model();
	Model(Model* parent);
	Model(const string& filename);
	Model(const string& filename, Model* parent);
	void load(const string& filename);
	
	Model& operator [](const string& name);
	vector<string> keys();
};

#endif