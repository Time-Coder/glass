#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glass/common.h"
#include "glass/Model"
#include "glass/Camera"
#include "glass/utils/transform.h"
#include "glass/utils/geometry.h"
#include "glass/utils/helper.h"

using namespace std;
using namespace glass;

map<string, Layout> Model::layout_map;
map<string, map<string, set<Model*> > > Model::material_group;

uint Model::n_models = 0;

void Model::setCamera(Camera& _camera)
{
	camera = &_camera;
}

void Model::move(float dx, float dy, float dz)
{
	__position.x += dx;
	__position.y += dy;
	__position.z += dz;
	update_mat();
}

void Model::move(const vec3& v)
{
	__position += v;
	update_mat();
}

void Model::moveTo(float x, float y, float z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void Model::moveTo(const vec3& v)
{
	__position = v;
	update_mat();
}

void Model::moveX(float dx)
{
	__position.x += dx;
	update_mat();
}

void Model::moveXTo(float x)
{
	__position.x = x;
	update_mat();
}

void Model::moveY(float dy)
{
	__position.y += dy;
	update_mat();
}

void Model::moveYTo(float y)
{
	__position.y = y;
	update_mat();
}

void Model::moveZ(float dz)
{
	__position.z += dz;
	update_mat();
}

void Model::moveZTo(float z)
{
	__position.z = z;
	update_mat();
}

vec3 Model::position()const
{
	return __position;
}

void Model::setPosition(float x, float y, float z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void Model::setPosition(const vec3& v)
{
	__position = v;
	update_mat();
}

vec3 Model::origin()const
{
	return __origin;
}

void Model::setOrigin(const vec3& _origin)
{
	__origin = _origin;
	update_mat();
}

void Model::setOrigin(float x, float y, float z)
{
	__origin = vec3(x, y, z);
	update_mat();
}

vec3 Model::center()const
{
	return __center;
}

void Model::setCenter(const vec3& _center)
{
	__center = _center;
	update_mat();
}

void Model::setCenter(float x, float y, float z)
{
	__center = vec3(x, y, z);
	update_mat();
}

vec3 Model::zoom()const
{
	return __zoom;
}

void Model::zoom(const vec3& v)
{
	__zoom *= v;
	update_mat();
}

void Model::zoomTo(const vec3& v)
{
	__zoom = v;
	update_mat();
}

void Model::zoomTo(float sx, float sy, float sz)
{
	__zoom = vec3(sx, sy, sz);
	update_mat();
}

void Model::zoomX(float sx)
{
	__zoom.x *= sx;
	update_mat();
}

void Model::zoomY(float sy)
{
	__zoom.y *= sy;
	update_mat();
}

void Model::zoomZ(float sz)
{
	__zoom.z *= sz;
	update_mat();
}

void Model::zoomXTo(float sx)
{
	__zoom.x = sx;
	update_mat();
}

void Model::zoomYTo(float sy)
{
	__zoom.y = sy;
	update_mat();
}

void Model::zoomZTo(float sz)
{
	__zoom.z = sz;
	update_mat();
}

void Model::yaw(float dyaw)
{
	__yaw += dyaw;
	update_mat();
}

void Model::roll(float droll)
{
	__roll += droll;
	update_mat();
}

float Model::yaw()const
{
	return __yaw;
}

float Model::pitch()const
{
	return __pitch;
}

float Model::roll()const
{
	return __roll;
}

void Model::yawTo(float _yaw)
{
	__yaw = _yaw;

	update_mat();
}

void Model::rollTo(float _roll)
{
	__roll = _roll;
	update_mat();
}

mat4 Model::self_mat()
{
	if(!camera)
	{
		return translate(-__origin) * translate(-__position) * Ry(-__yaw)*Rx(-__pitch)*Rz(__roll) * glass::zoom(1.0f/__zoom) * translate(__center);
	}
	else
	{
		return translate(-__origin) * translate(-__position) * axis_angle(camera->right(), __relative_pitch) * Ry(-__yaw)*Rx(-__pitch)*Rz(__roll) * glass::zoom(1.0f/__zoom) * translate(__center);
	}
}

void Model::reset()
{
	__position = vec3(0);
	__zoom = vec3(1);
	__yaw  = 0.0f; __pitch = 0.0f; __roll = 0.0f;
	__relative_pitch = 0.0f;

	update_mat();
}

float Model::relativePitch()const
{
	return __relative_pitch;
}

Model::Model(const Model& model):
__mat(model.__mat),
__normal_mat(model.__normal_mat),
__material(model.__material),
layout_key(model.layout_key),
model_name(model.model_name),
file_name(model.file_name),
__position(model.__position),
__origin(model.__origin),
__center(model.__center),
__zoom(model.__zoom),
__yaw(model.__yaw),
__pitch(model.__pitch),
__roll(model.__roll),
camera(model.camera),
__relative_pitch(model.__relative_pitch),
parent(model.parent),
children(model.children),
base_dir(model.base_dir)
{
	n_models++;
}

Model::Model(Model* _parent):
parent(_parent)
{
	n_models++;
	if(_parent == NULL)
	{
		model_name = "model0";
		return;
	}

	int i = 0;
	while(parent->children.count("model" + str::str(i)) > 0)
	{
		i++;
	}
	model_name = "model" + str::str(i);
}

Model::Model(const string& name, Model* _parent):
model_name(name), parent(_parent)
{
	if(parent == NULL)
	{
		n_models++;
		return;
	}

	if(parent->children.count(name) > 0)
	{
		throw glass::KeyError("model name '" + name + "' is exist in current model path.");
	}

	n_models++;
}

Model::~Model()
{
	n_models--;
	if(n_models == 0)
	{
		layout_map.clear();
	}
}

void Model::load_diffuse_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_DIFFUSE, 0, &filename);
        __material.setTexture(base_dir + "/" + path::basename(filename.C_Str()), Material::DIFFUSE);
    }
}

void Model::load_specular_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_SPECULAR) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_SPECULAR, 0, &filename);
        __material.setTexture(base_dir + "/" + path::basename(filename.C_Str()), Material::SPECULAR);
    }
}

void Model::load_bump_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_HEIGHT) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_HEIGHT, 0, &filename);
        __material.setTexture(base_dir + "/" + path::basename(filename.C_Str()), Material::BUMP);
    }
}

void Model::load_normal_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_NORMALS) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_NORMALS, 0, &filename);
        __material.setTexture(base_dir + "/" + path::basename(filename.C_Str()), Material::NORMAL);
    }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	if(mesh->mMaterialIndex >= 0)
	{
	    aiMaterial *ptr_material = scene->mMaterials[mesh->mMaterialIndex];
	    load_diffuse_map(ptr_material);
	    load_specular_map(ptr_material);
	    // load_bump_map(ptr_material);
	    load_normal_map(ptr_material);

	    update_material_group();
	}

	layout_key = file_name + "::" + path();
	if(layout_map.count(layout_key) && !layout_map[layout_key].empty())
	{
		return;
	}

	vector<vec3> positions;
	vector<vec3> normals;
	vector<vec2> tex_coords;
	vector<vec3> tangents;
	vector<vec3> bitangents;
	for(uint i = 0; i < mesh->mNumVertices; i++)
    {
        positions.push_back(vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
        normals.push_back(normalize(vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)));
        if(mesh->mTextureCoords[0])
        {
        	tex_coords.push_back(vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }
        else
        {
        	tex_coords.push_back(vec2(0, 0));
        }

        if(mesh->mTangents)
        {
        	tangents.push_back(vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
        }
        else
        {
        	tangents.push_back(vec3(0));
        }

        if(mesh->mBitangents)
        {
        	bitangents.push_back(vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z));
        }
        else
        {
        	bitangents.push_back(vec3(0));
        }
    }

    vector<uvec3> indices;
    for(uint i = 0; i < mesh->mNumFaces; i++)
	{
	    aiFace face = mesh->mFaces[i];
	   	indices.push_back(uvec3(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
	}

	layout_map[layout_key](0) = positions;
	layout_map[layout_key](1) = normals;
	layout_map[layout_key](2) = tex_coords;
	layout_map[layout_key](3) = tangents;
	layout_map[layout_key](4) = bitangents;
	layout_map[layout_key].order(indices); 
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for(uint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        string name = "mesh" + str::str(i);

        (*this)[name] = Model(name, this);
        (*this)[name].base_dir = base_dir;
        (*this)[name].file_name = file_name;
        (*this)[name].processMesh(mesh, scene);
    }
    for(uint i = 0; i < node->mNumChildren; i++)
    {
    	string name = "model" + str::str(i);

    	(*this)[name] = Model(name, this);
    	(*this)[name].base_dir = base_dir;
    	(*this)[name].file_name = file_name;
    	(*this)[name].processNode(node->mChildren[i], scene);
    }
}

void Model::load(const string& filename)
{
	if(!path::isfile(filename))
	{
		throw glass::IOError("File " + filename + " is not exists.");
	}
	
	base_dir = path::dirname(filename);
	file_name = path::abspath(filename);

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
    	throw glass::RuntimeError("Error when loading model " + filename + ": " + importer.GetErrorString());
    }
    processNode(scene->mRootNode, scene);
}

mat4 Model::_mat()
{
	if(parent)
	{
		return parent->__mat * self_mat();
	}
	else
	{
		return self_mat();
	}
}

mat3 Model::_normal_mat()
{
	return transpose(inverse(mat3(__mat)));
}

mat4 Model::mat()const
{
	return __mat;
}

mat3 Model::normal_mat()const
{
	return __normal_mat;
}

void Model::update_mat()
{
	__mat = _mat();
	__normal_mat = _normal_mat();
	for(auto it = children.begin(); it != children.end(); it++)
	{
		it->second.update_mat();
	}
	sync();
}

Model& Model::operator [](const string& _model_name)
{
	if(children.count(_model_name) == 0)
	{
		children.insert({_model_name, Model(_model_name, this)});
	}
	return children[_model_name];
}

vector<string> Model::keys()
{
	vector<string> result;
	for(auto it = children.begin(); it != children.end(); it++)
	{
		result.push_back(it->first);
	}
	return result;
}

Layout& Model::layout()
{
	return layout_map[layout_key];
}

GLVector& Model::layout(uint location)
{
	return layout_map[layout_key](location);
}

string Model::name()const
{
	return model_name;
}

void Model::rename(const string& _name)
{
	if(parent == NULL ||
	   parent->children.count(model_name) == 0 ||
	   &(parent->children[model_name]) != this)
	{
		model_name = _name;
		return;
	}

	if(parent->children.count(_name) > 0)
	{
		throw glass::KeyError(_name + " is already exists in current model path.");
	}

	parent->children.erase(model_name);
	model_name = _name;
	parent->children.insert({_name, *this});
}

string Model::path()const
{
	string result = model_name;
	const Model* model = this;
	while(model->parent)
	{
		model = model->parent;
		result = model->model_name + "/" + result;
	}

	return result;
}

uint Model::depth()const
{
	uint n = 0;
	const Model* model = this;
	while(model->parent)
	{
		n++;
		model = model->parent;
	}

	return n;
}

string Model::key()const
{
	return layout_key;
}

void Model::setKey(const string& _key)
{
	layout_key = _key;
}

bool Model::exists()const
{
	return (layout_map.count(layout_key) > 0);
}

ostream& operator <<(ostream& out, const Model& model)
{
	int n = model.depth();
	for(int i = 0; i < n; i++)
	{
		out << "    ";
	}
	out << model.name();
	for(auto it = model.children.begin(); it != model.children.end(); it++)
	{
		out << endl << it->second;
	}
	return out;
}

void Model::update_material_group()
{
	if(layout_key.empty())
	{
		return;
	}

	string temp_material_key = __material.key();
	if(temp_material_key == material_key)
	{
		return;
	}

	if(material_group.count(layout_key) &&
	   material_group[layout_key].count(material_key) &&
	   material_group[layout_key][material_key].count(this))
	{
		material_group[layout_key][material_key].erase(this);
	}

	material_key = temp_material_key;

	material_group[layout_key][material_key].insert(this);
}

void Model::setTexture(const string& filename, Material::TextureType type, float height_factor, bool _inverse)
{
	__material.setTexture(filename, type, height_factor, _inverse);
	update_material_group();
}

void Model::setTexture(const sampler2D& sampler, Material::TextureType type, float height_factor, bool _inverse)
{
	__material.setTexture(sampler, type, height_factor, _inverse);
	update_material_group();
}

void Model::useTexture(Material::TextureType type, bool flag)
{
	__material.useTexture(type, flag);
	update_material_group();
}

void Model::setAsLight(bool flag)
{
	is_light = flag;
}

bool Model::isLight()const
{
	return is_light;
}

void Model::setColor(const vec3& _color)
{
	__material.diffuse_color = _color;
	update_material_group();
}

void Model::setColor(float r, float g, float b)
{
	__material.diffuse_color = vec3(r, g, b);
	update_material_group();
}

vec3 Model::color()const
{
	return __material.diffuse_color;
}

void Model::setMaskColor(const vec3& _color)
{
	__material.mask_color = _color;
	update_material_group();
}

vec3 Model::maskColor()const
{
	return __material.mask_color;
}

Material Model::material()const
{
	return __material;
}

void Model::setMaterial(const Material& _material)
{
	__material = _material;
	update_material_group();
}

void Model::pitch(float dpitch)
{
	__pitch += dpitch;
	if(__pitch > PI/2-1E-6)
	{
		__pitch = PI/2-1E-6;
	}
	else if(__pitch < -PI/2+1E-6)
	{
		__pitch = -PI/2+1E-6;
	}
	update_mat();
}

void Model::pitchTo(float _pitch)
{
	__pitch = _pitch;
	if(__pitch > PI/2-1E-6)
	{
		__pitch = PI/2-1E-6;
	}
	else if(__pitch < -PI/2+1E-6)
	{
		__pitch = -PI/2+1E-6;
	}

	update_mat();
}

void Model::relativePitchTo(float _pitch)
{
	__relative_pitch = _pitch;
	if(__relative_pitch > camera->pitch()+PI/2)
	{
		__relative_pitch = camera->pitch()+PI/2;
	}
	else if(__relative_pitch < camera->pitch()-PI/2)
	{
		__relative_pitch = camera->pitch()-PI/2;
	}

	update_mat();
}

void Model::relativePitch(float _pitch)
{
	__relative_pitch += _pitch;
	if(__relative_pitch > camera->pitch()+PI/2)
	{
		__relative_pitch = camera->pitch()+PI/2;
	}
	else if(__relative_pitch < camera->pitch()-PI/2)
	{
		__relative_pitch = camera->pitch()-PI/2;
	}

	update_mat();
}

bool Model::empty()const
{
	return layout_key.empty();
}

void Model::hide()
{
	is_visible = false;
}

void Model::show()
{
	is_visible = true;
}

bool Model::isVisible()const
{
	return is_visible;
}

bool Model::isHidden()const
{
	return !is_visible;
}