#include "model.h"
#include "transform.hpp"
#include "geometry.h"

using namespace tran;

void Model::setCamera(Camera& _camera)
{
	camera = &_camera;
}

void Model::move(double dx, double dy, double dz)
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

void Model::move_to(double x, double y, double z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void Model::move_to(const vec3& v)
{
	__position = v;
	update_mat();
}

void Model::move_x(double dx)
{
	__position.x += dx;
	update_mat();
}

void Model::move_x_to(double x)
{
	__position.x = x;
	update_mat();
}

void Model::move_y(double dy)
{
	__position.y += dy;
	update_mat();
}

void Model::move_y_to(double y)
{
	__position.y = y;
	update_mat();
}

void Model::move_z(double dz)
{
	__position.z += dz;
	update_mat();
}

void Model::move_z_to(double z)
{
	__position.z = z;
	update_mat();
}

vec3 Model::position()const
{
	return __position;
}

void Model::position(double x, double y, double z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void Model::position(const vec3& v)
{
	__position = v;
	update_mat();
}

vec3 Model::origin()const
{
	return __origin;
}

void Model::origin(const vec3& _origin)
{
	__origin = _origin;
	update_mat();
}

void Model::origin(double x, double y, double z)
{
	__origin = vec3(x, y, z);
	update_mat();
}

vec3 Model::center()const
{
	return __center;
}

void Model::center(const vec3& _center)
{
	__center = _center;
	update_mat();
}

void Model::center(double x, double y, double z)
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

void Model::zoom_to(const vec3& v)
{
	__zoom = v;
	update_mat();
}

void Model::zoom_to(double sx, double sy, double sz)
{
	__zoom = vec3(sx, sy, sz);
	update_mat();
}

void Model::zoom_x(double sx)
{
	__zoom.x *= sx;
	update_mat();
}

void Model::zoom_y(double sy)
{
	__zoom.y *= sy;
	update_mat();
}

void Model::zoom_z(double sz)
{
	__zoom.z *= sz;
	update_mat();
}

void Model::zoom_x_to(double sx)
{
	__zoom.x = sx;
	update_mat();
}

void Model::zoom_y_to(double sy)
{
	__zoom.y = sy;
	update_mat();
}

void Model::zoom_z_to(double sz)
{
	__zoom.z = sz;
	update_mat();
}

void Model::yaw(double dyaw)
{
	__yaw += dyaw;
	update_mat();
}

void Model::roll(double droll)
{
	__roll += droll;
	update_mat();
}

void Model::pitch(double dpitch)
{
	__pitch += dpitch;
	if(__pitch > pi/2-1E-6)
	{
		__pitch = pi/2-1E-6;
	}
	else if(__pitch < -pi/2+1E-6)
	{
		__pitch = -pi/2+1E-6;
	}
	update_mat();
}

double Model::yaw()const
{
	return __yaw;
}

double Model::pitch()const
{
	return __pitch;
}

double Model::roll()const
{
	return __roll;
}

void Model::yaw_to(double _yaw)
{
	__yaw = _yaw;

	update_mat();
}

void Model::roll_to(double _roll)
{
	__roll = _roll;
	update_mat();
}

void Model::pitch_to(double _pitch)
{
	__pitch = _pitch;
	if(__pitch > pi/2-1E-6)
	{
		__pitch = pi/2-1E-6;
	}
	else if(__pitch < -pi/2+1E-6)
	{
		__pitch = -pi/2+1E-6;
	}

	update_mat();
}

mat4 Model::__mat()
{
	if(!camera)
	{
		return translate(-__origin) * translate(-__position) * Ry(-__yaw)*Rx(-__pitch)*Rz(__roll) * tran::zoom(1.0f/__zoom) * translate(__center);
	}
	else
	{

		return translate(-__origin) * translate(-__position) * axis_angle(camera->right(), __relative_pitch) * Ry(-__yaw)*Rx(-__pitch)*Rz(__roll) * tran::zoom(1.0f/__zoom) * translate(__center);
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

double Model::relative_pitch()const
{
	return __relative_pitch;
}

void Model::relative_pitch(double _pitch)
{
	__relative_pitch += _pitch;
	if(__relative_pitch > camera->pitch()+pi/2)
	{
		__relative_pitch = camera->pitch()+pi/2;
	}
	else if(__relative_pitch < camera->pitch()-pi/2)
	{
		__relative_pitch = camera->pitch()-pi/2;
	}

	update_mat();
}

void Model::relative_pitch_to(double _pitch)
{
	__relative_pitch = _pitch;
	if(__relative_pitch > camera->pitch()+pi/2)
	{
		__relative_pitch = camera->pitch()+pi/2;
	}
	else if(__relative_pitch < camera->pitch()-pi/2)
	{
		__relative_pitch = camera->pitch()-pi/2;
	}

	update_mat();
}

Model::Model() {}

Model::Model(Model* parent)
{
	this->parent = parent;
}

Model::Model(const string& filename)
{
	load(filename);
}

Model::Model(const string& filename, Model* parent)
{
	this->parent = parent;
	load(filename);
}

void Model::load_ambient_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_AMBIENT) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_AMBIENT, 0, &filename);
        material.set_ambient_map(base_dir + "/" + path::basename(filename.C_Str()));
    }
}

void Model::load_reflection_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_AMBIENT) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_AMBIENT, 0, &filename);
        material.set_reflection_map(base_dir + "/" + path::basename(filename.C_Str()));
    }
}

void Model::load_diffuse_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_DIFFUSE, 0, &filename);
        material.set_diffuse_map(base_dir + "/" + path::basename(filename.C_Str()));
    }
}

void Model::load_specular_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_SPECULAR) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_SPECULAR, 0, &filename);
        material.set_specular_map(base_dir + "/" + path::basename(filename.C_Str()));
    }
}

void Model::load_depth_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_HEIGHT) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_HEIGHT, 0, &filename);
        material.set_depth_map(base_dir + "/" + path::basename(filename.C_Str()));
    }
}

void Model::load_normal_map(aiMaterial* ptr_material)
{
	if(ptr_material && ptr_material->GetTextureCount(aiTextureType_NORMALS) > 0)
    {
        aiString filename;
        ptr_material->GetTexture(aiTextureType_NORMALS, 0, &filename);
        material.set_normal_map(base_dir + "/" + path::basename(filename.C_Str()));
    }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<vec3> positions;
	vector<vec3> normals;
	vector<vec4> tex_coords;
	vector<vec3> tangents;
	vector<vec3> bitangents;
	for(uint i = 0; i < mesh->mNumVertices; i++)
    {
        positions.push_back(vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
        normals.push_back(normalize(vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)));
        if(mesh->mTextureCoords[0])
        {
        	tex_coords.push_back(vec4(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y,
        		                      mesh->mTextureCoords[0][i].z, 1));
        }
        else
        {
        	tex_coords.push_back(vec4(0, 0, 0, 1));
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

	layout(0) = positions;
	layout(1) = normals;
	layout(2) = tex_coords;
	layout(3) = tangents;
	layout(4) = bitangents;
	
	layout.order(indices);

    if(mesh->mMaterialIndex >= 0)
	{
	    aiMaterial *ptr_material = scene->mMaterials[mesh->mMaterialIndex];
	    load_reflection_map(ptr_material);
	    load_diffuse_map(ptr_material);
	    load_specular_map(ptr_material);
	    load_depth_map(ptr_material);
	    load_normal_map(ptr_material);
	}
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for(uint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        (*this)[str::str(this) + "/mesh" + str::str(i)].base_dir = base_dir;
        (*this)[str::str(this) + "/mesh" + str::str(i)].processMesh(mesh, scene);
    }
    for(uint i = 0; i < node->mNumChildren; i++)
    {
    	(*this)[str::str(this) + "/model" + str::str(i)].base_dir = base_dir;
    	(*this)[str::str(this) + "/model" + str::str(i)].processNode(node->mChildren[i], scene);
    }
}

void Model::load(const string& filename)
{
	base_dir = path::dirname(filename);
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
    	throw RuntimeError("Error when loading model " + filename + ": " + importer.GetErrorString());
    }
    processNode(scene->mRootNode, scene);
}

mat4 Model::_mat()
{
	if(parent)
	{
		return parent->_mat() * __mat();
	}
	else
	{
		return __mat();
	}
}

mat3 Model::_normal_mat()
{
	return transpose(inverse(mat3(mat)));
}

void Model::update_mat()
{
	mat = _mat();
	normal_mat = _normal_mat();
}

Model& Model::operator [](const string& name)
{
	if(models.count(name) == 0)
	{
		models[name] = Model(this);
	}
	return models[name];
}

vector<string> Model::keys()
{
	vector<string> result;
	for(auto it = models.begin(); it != models.end(); it++)
	{
		result.push_back(it->first);
	}
	return result;
}