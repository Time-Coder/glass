#include <cmath>

#include "glass/utils/geometry.h"
#include "glass/Model"

using namespace std;

void glass::generateTBN(const vector<vec3>& positions, const vector<vec4>& tex_coords, const vector<uvec3>& indices, vector<vec3>& tangents, vector<vec3>& bitangents, vector<vec3>& normals)
{
	vector< vector<double> > weights(positions.size());
	vector< vector<mat3 > > TBNs(positions.size());
	for(int i = 0; i < indices.size(); i++)
	{
		uint P1 = indices[i][0];
		uint P2 = indices[i][1];
		uint P3 = indices[i][2];

		vec2 e1 = vec2(tex_coords[P1])/tex_coords[P1].w - vec2(tex_coords[P2])/tex_coords[P2].w;
		vec2 e2 = vec2(tex_coords[P3])/tex_coords[P3].w - vec2(tex_coords[P2])/tex_coords[P2].w;
		mat2 right_mat2 = inverse(mat2(e1, e2));
		mat3 right_mat3(vec3(right_mat2[0], 0), vec3(right_mat2[1], 0), vec3());
		vec3 E1 = positions[P1] - positions[P2];
		vec3 E2 = positions[P3] - positions[P2];
		mat3 left_mat3(E1, E2, vec3());
		mat3 tbn = left_mat3 * right_mat3;
		tbn[0] = normalize(tbn[0]);
		tbn[1] = normalize(tbn[1]);
		tbn[2] = normalize(cross(E2, E1));
		TBNs[P1].push_back(tbn);
		TBNs[P2].push_back(tbn);
		TBNs[P3].push_back(tbn);
		weights[P1].push_back(acos(dot(normalize(positions[P2]-positions[P1]), normalize(positions[P3]-positions[P1]))));
		weights[P2].push_back(acos(dot(normalize(E1), normalize(E2))));
		weights[P3].push_back(acos(dot(normalize(positions[P1]-positions[P3]), normalize(positions[P2]-positions[P3]))));
	}

	tangents = vector<vec3>(positions.size());
	bitangents = vector<vec3>(positions.size());
	normals = vector<vec3>(positions.size());
	for(int i = 0; i < positions.size(); i++)
	{
		mat3 tbn(0);
		for(int j = 0; j < TBNs[i].size(); j++)
		{
			tbn += weights[i][j] * TBNs[i][j];
		}
		tbn[0] = normalize(tbn[0]);
		tbn[1] = normalize(tbn[1]);
		tbn[2] = normalize(tbn[2]);
		tbn[0] = normalize(tbn[0] - dot(tbn[0], tbn[2]) * tbn[2]);
		tbn[1] = normalize(tbn[1] - dot(tbn[1], tbn[2]) * tbn[2]);
		tangents[i] = tbn[0];
		bitangents[i] = tbn[1];
		normals[i] = tbn[2];
	}
}

void glass::generateTBN(const vector<vec3>& positions, const vector<vec3>& normals, const vector<vec4>& tex_coords, const vector<uvec3>& indices, vector<vec3>& tangents, vector<vec3>& bitangents)
{
	vector< vector<double> > weights(positions.size());
	vector< vector<mat3 > > TBNs(positions.size());
	for(int i = 0; i < indices.size(); i++)
	{
		uint P1 = indices[i][0];
		uint P2 = indices[i][1];
		uint P3 = indices[i][2];
		vec2 e1 = vec2(tex_coords[P1])/tex_coords[P1].w - vec2(tex_coords[P2])/tex_coords[P2].w;
		vec2 e2 = vec2(tex_coords[P3])/tex_coords[P3].w - vec2(tex_coords[P2])/tex_coords[P2].w;
		mat2 right_mat2 = inverse(mat2(e1, e2));
		mat3 right_mat3(vec3(right_mat2[0], 0), vec3(right_mat2[1], 0), vec3());
		vec3 E1 = positions[P1] - positions[P2];
		vec3 E2 = positions[P3] - positions[P2];
		mat3 left_mat3(E1, E2, vec3());
		mat3 tbn = left_mat3 * right_mat3;
		TBNs[P1].push_back(tbn);
		TBNs[P2].push_back(tbn);
		TBNs[P3].push_back(tbn);
		weights[P1].push_back(acos(dot(normalize(positions[P2]-positions[P1]), normalize(positions[P3]-positions[P1]))));
		weights[P2].push_back(acos(dot(normalize(E1), normalize(E2))));
		weights[P3].push_back(acos(dot(normalize(positions[P1]-positions[P3]), normalize(positions[P2]-positions[P3]))));
	}

	tangents = vector<vec3>(positions.size());
	bitangents = vector<vec3>(positions.size());
	for(int i = 0; i < positions.size(); i++)
	{
		mat3 tbn(0);
		double total_weight = 0;
		for(int j = 0; j < TBNs[i].size(); j++)
		{
			tbn += weights[i][j] * TBNs[i][j];
			total_weight += weights[i][j];
		}
		tbn[0] = tbn[0]/total_weight;
		tbn[1] = tbn[1]/total_weight;
		tbn[2] = normalize(normals[i]);
		tbn[0] = tbn[0] - dot(tbn[0], tbn[2]) * tbn[2];
		tbn[1] = tbn[1] - dot(tbn[1], tbn[2]) * tbn[2];
		tangents[i] = tbn[0];
		bitangents[i] = tbn[1];
	}
}

Model glass::plane()
{
	Model model;
	model.setKey("plane()");
	if(model.exists())
	{
		return model;
	}

	vector<vec2> positions = 
	{
		vec2(-1.0f, -1.0f),
		vec2( 1.0f, -1.0f),
		vec2( 1.0f,  1.0f),
		vec2(-1.0f,  1.0f)
	};

	vector<uvec3> indices =
	{
		uvec3(0, 1, 2),
		uvec3(0, 2, 3)
	};

	model.layout(0) = positions;
	model.layout().order(indices);

	return model;
}

Model glass::cube(float a)
{
	Model model;
	model.setKey("cube(" + str::str(a) + ")");
	if(model.exists())
	{
		return model;
	}

	vector<vec3> positions = 
	{
		// bottom
		a*vec3(-0.5f, -0.5f,  0.5f),
		a*vec3( 0.5f, -0.5f,  0.5f),
		a*vec3( 0.5f, -0.5f, -0.5f),
		a*vec3(-0.5f, -0.5f, -0.5f),

		// up
		a*vec3(-0.5f,  0.5f,  0.5f),
		a*vec3( 0.5f,  0.5f,  0.5f),
		a*vec3( 0.5f,  0.5f, -0.5f),
		a*vec3(-0.5f,  0.5f, -0.5f),

		// front
		a*vec3(-0.5f, -0.5f,  0.5f),
		a*vec3( 0.5f, -0.5f,  0.5f),
		a*vec3( 0.5f,  0.5f,  0.5f),
		a*vec3(-0.5f,  0.5f,  0.5f),

		// back
		a*vec3(-0.5f, -0.5f, -0.5f),
		a*vec3( 0.5f, -0.5f, -0.5f),
		a*vec3( 0.5f,  0.5f, -0.5f),
		a*vec3(-0.5f,  0.5f, -0.5f),

		// right
		a*vec3( 0.5f, -0.5f,  0.5f),
		a*vec3( 0.5f, -0.5f, -0.5f),
		a*vec3( 0.5f,  0.5f, -0.5f),
		a*vec3( 0.5f,  0.5f,  0.5f),

		// left
		a*vec3(-0.5f, -0.5f,  0.5f),
		a*vec3(-0.5f, -0.5f, -0.5f),
		a*vec3(-0.5f,  0.5f, -0.5f),
		a*vec3(-0.5f,  0.5f,  0.5f),
	};

	vector<vec3> normals =
	{
		// bottom
		vec3( 0.0f, -1.0f,  0.0f),
		vec3( 0.0f, -1.0f,  0.0f),
		vec3( 0.0f, -1.0f,  0.0f),
		vec3( 0.0f, -1.0f,  0.0f),

		// up
		vec3( 0.0f,  1.0f,  0.0f),
		vec3( 0.0f,  1.0f,  0.0f),
		vec3( 0.0f,  1.0f,  0.0f),
		vec3( 0.0f,  1.0f,  0.0f),

		// front
		vec3( 0.0f,  0.0f,  1.0f),
		vec3( 0.0f,  0.0f,  1.0f),
		vec3( 0.0f,  0.0f,  1.0f),
		vec3( 0.0f,  0.0f,  1.0f),

		// back
		vec3( 0.0f,  0.0f, -1.0f),
		vec3( 0.0f,  0.0f, -1.0f),
		vec3( 0.0f,  0.0f, -1.0f),
		vec3( 0.0f,  0.0f, -1.0f),

		// right
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),

		// left
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f)
	};

	vector<vec4> tex_coords =
	{
		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 0.0f, 0.0f, 1.0f),
		vec4(1.0f, 1.0f, 0.0f, 1.0f),
		vec4(0.0f, 1.0f, 0.0f, 1.0f)
	};

	vector<uvec3> indices = 
	{
		uvec3(0, 3, 2),
		uvec3(2, 1, 0),

		uvec3(4, 5, 6),
		uvec3(6, 7, 4),

		uvec3(8, 9, 10),
		uvec3(10, 11, 8),

		uvec3(15, 14, 13),
		uvec3(13, 12, 15),

		uvec3(16, 17, 18),
		uvec3(18, 19, 16),

		uvec3(23, 22, 21),
		uvec3(21, 20, 23)
	};

	model.layout(0) = positions;
	model.layout(1) = normals;
	model.layout(2) = tex_coords;

	vector<vec3> tangents, bitangents;
	generateTBN(positions, normals, tex_coords, indices, tangents, bitangents);
	model.layout(3) = tangents;
	model.layout(4) = bitangents;
	
	model.layout().order(indices);

	return model;
}

Model glass::sphere(float R, unsigned int n_theta, unsigned int n_phi)
{
	if(n_phi == 0)
	{
		n_phi = n_theta / 2.0;
	}

	Model model;
	model.setKey("sphere(" + str::str(R) + ", " + str::str(n_theta) + ", " + str::str(n_phi) + ")");
	if(model.exists())
	{
		return model;
	}

	vector<vec3> positions(n_phi*n_theta);
	vector<vec3> normals(n_phi*n_theta);
	vector<vec4> tex_coords(n_phi*n_theta);
	vector<uvec3> indices(2*(n_phi-1)*(n_theta-1));

	vec3 frag_norm, current_vertex;
	int it_row, it_col, index;
	int indices_index = 0;
	double weight;
	// int indices_reverse = 1;
	for(it_row = 0; it_row < n_phi; it_row++)
	{
		for(it_col = 0; it_col < n_theta; it_col++)
		{
			index = it_row + it_col*n_phi;

			double x = 1.0*it_col/(n_theta-1);
			double y = 1.0*it_row/(n_phi-1);
			double theta = x * 2*PI;
			double phi = (y - 0.5) * PI;
			tex_coords[index][0] = x;
			tex_coords[index][1] = y;
			tex_coords[index][2] = 0;
			tex_coords[index][3] = 1;

			normals[index][0] = cos(phi)*sin(theta);
			normals[index][1] = sin(phi);
			normals[index][2] = cos(phi)*cos(theta);

			positions[index] = R * normals[index];

			if(it_col != n_theta-1 && it_row != n_phi-1)
			{
				indices[indices_index][0] = index;
				indices[indices_index][1] = index + n_phi;
				// if(indices_reverse == 1)
				// {
					indices[indices_index][2] = index + n_phi + 1;
					indices[indices_index+1][0] = index;
				// }
				// else
				// {
				// 	indices(indices_index, 2) = index + 1;
				// 	indices(indices_index+1, 0) = index + n_phi;
				// }
				indices[indices_index+1][1] = index + n_phi+1;
				indices[indices_index+1][2] = index + 1;

				// indices_reverse *= -1;
				indices_index += 2;
			}
		}
	}

	model.layout(0) = positions;
	model.layout(1) = normals;
	model.layout(2) = tex_coords;

	vector<vec3> tangents, bitangents;
	generateTBN(positions, normals, tex_coords, indices, tangents, bitangents);
	model.layout(3) = tangents;
	model.layout(4) = bitangents;

	model.layout().order(indices);

	return model;
}

Model glass::floor(float blocks_per_meter)
{
	Model model;
	model.setKey("floor(" + str::str(blocks_per_meter) + ")");
	if(model.exists())
	{
		return model;
	}

	vector<vec3> positions = 
	{
		// bottom
		vec3(-1000.0f, -1000.0f,  1000.0f),
		vec3( 1000.0f, -1000.0f,  1000.0f),
		vec3( 1000.0f, -1000.0f, -1000.0f),
		vec3(-1000.0f, -1000.0f, -1000.0f),

		// up
		vec3(-1000.0f,  0.0f,  1000.0f),
		vec3( 1000.0f,  0.0f,  1000.0f),
		vec3( 1000.0f,  0.0f, -1000.0f),
		vec3(-1000.0f,  0.0f, -1000.0f),

		// front
		vec3(-1000.0f, -1000.0f,  1000.0f),
		vec3( 1000.0f, -1000.0f,  1000.0f),
		vec3( 1000.0f,  0.0f,  1000.0f),
		vec3(-1000.0f,  0.0f,  1000.0f),

		// back
		vec3(-1000.0f, -1000.0f, -1000.0f),
		vec3( 1000.0f, -1000.0f, -1000.0f),
		vec3( 1000.0f,  0.0f, -1000.0f),
		vec3(-1000.0f,  0.0f, -1000.0f),

		// right
		vec3( 1000.0f, -1000.0f,  1000.0f),
		vec3( 1000.0f, -1000.0f, -1000.0f),
		vec3( 1000.0f,  0.0f, -1000.0f),
		vec3( 1000.0f,  0.0f,  1000.0f),

		// left
		vec3(-1000.0f, -1000.0f,  1000.0f),
		vec3(-1000.0f, -1000.0f, -1000.0f),
		vec3(-1000.0f,  0.0f, -1000.0f),
		vec3(-1000.0f,  0.0f,  1000.0f),
	};

	vector<vec3> normals =
	{
		// bottom
		vec3( 0.0f, -1.0f,  0.0f),
		vec3( 0.0f, -1.0f,  0.0f),
		vec3( 0.0f, -1.0f,  0.0f),
		vec3( 0.0f, -1.0f,  0.0f),

		// up
		vec3( 0.0f,  1.0f,  0.0f),
		vec3( 0.0f,  1.0f,  0.0f),
		vec3( 0.0f,  1.0f,  0.0f),
		vec3( 0.0f,  1.0f,  0.0f),

		// front
		vec3( 0.0f,  0.0f,  1.0f),
		vec3( 0.0f,  0.0f,  1.0f),
		vec3( 0.0f,  0.0f,  1.0f),
		vec3( 0.0f,  0.0f,  1.0f),

		// back
		vec3( 0.0f,  0.0f, -1.0f),
		vec3( 0.0f,  0.0f, -1.0f),
		vec3( 0.0f,  0.0f, -1.0f),
		vec3( 0.0f,  0.0f, -1.0f),

		// right
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),

		// left
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(-1.0f, 0.0f, 0.0f)
	};

	vector<vec4> tex_coords =
	{
		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),
		vec4(0.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),
		vec4(0.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),
		vec4(0.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),
		vec4(0.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),
		vec4(0.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),

		vec4(0.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, 0.0f, 0.0f, 1.0f),
		vec4(blocks_per_meter*1000.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f),
		vec4(0.0f, blocks_per_meter*1000.0f, 0.0f, 1.0f)
	};

	vector<uvec3> indices = 
	{
		uvec3(0, 3, 2),
		uvec3(2, 1, 0),

		uvec3(4, 5, 6),
		uvec3(6, 7, 4),

		uvec3(8, 9, 10),
		uvec3(10, 11, 8),

		uvec3(15, 14, 13),
		uvec3(13, 12, 15),

		uvec3(16, 17, 18),
		uvec3(18, 19, 16),

		uvec3(23, 22, 21),
		uvec3(21, 20, 23)
	};

	model.layout(0) = positions;
	model.layout(1) = normals;
	model.layout(2) = tex_coords;

	vector<vec3> tangents, bitangents;
	generateTBN(positions, normals, tex_coords, indices, tangents, bitangents);
	model.layout(3) = tangents;
	model.layout(4) = bitangents;
	
	model.layout().order(indices);

	return model;
}