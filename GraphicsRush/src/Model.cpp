#include "Model.H"

Model::
Model()
{

}

Model::
Model(const std::string fileName)
{
	if (fileName == "CUBE")
	{
		indexed_vertices = {
			glm::vec3( -0.5f,  0.0f, -0.5f),
			glm::vec3( -0.5f,  0.0f,  0.5f),
			glm::vec3(  0.5f,  0.0f,  0.5f),
			glm::vec3(  0.5f,  0.0f, -0.5f),

			glm::vec3( -0.5f,  0.0f, -0.5f),
			glm::vec3( -0.5f,  0.0f,  0.5f),
			glm::vec3( -0.5f,  1.0f,  0.5f),
			glm::vec3( -0.5f,  1.0f, -0.5f),

			glm::vec3(  0.5f,  0.0f,  0.5f),
			glm::vec3( -0.5f,  0.0f,  0.5f),
			glm::vec3( -0.5f,  1.0f,  0.5f),
			glm::vec3(  0.5f,  1.0f,  0.5f),

			glm::vec3(  0.5f,  0.0f, -0.5f),
			glm::vec3(  0.5f,  0.0f,  0.5f),
			glm::vec3(  0.5f,  1.0f,  0.5f),
			glm::vec3(  0.5f,  1.0f, -0.5f),

			glm::vec3(  0.5f,  0.0f, -0.5f),
			glm::vec3( -0.5f,  0.0f, -0.5f),
			glm::vec3( -0.5f,  1.0f, -0.5f),
			glm::vec3(  0.5f,  1.0f, -0.5f),

			glm::vec3(  0.5f,  1.0f,  0.5f),
			glm::vec3(  0.5f,  1.0f, -0.5f),
			glm::vec3( -0.5f,  1.0f, -0.5f),
			glm::vec3( -0.5f,  1.0f,  0.5f),
		};
		indexed_normals = {
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),

			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),

			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),

			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),

			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		};
		indexed_uvs = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),

			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};
		indices = {
			0, 1, 2,
			0, 2, 3,

			6, 7, 4,
			6, 4, 5,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23
		};

		VAOProcess();
	}
	else if (fileName == "SPHERE") // Under construction
	{
		int d_angle = 6;
		unsigned short indices_index = 0;
		float r = 0.5f;
		float sphere_x, sphere_y, sphere_z;
		for (int phi = -90; phi < 90; phi += d_angle)
		{
			for (int theta = 0; theta < 360; theta += d_angle)
			{
				sphere_x = r * glm::cos(glm::radians((float)phi)) * glm::cos(glm::radians((float)theta));
				sphere_y = r * glm::sin(glm::radians((float)phi));
				sphere_z = r * glm::cos(glm::radians((float)phi)) * glm::sin(glm::radians((float)theta));
				indexed_vertices.push_back(glm::vec3(sphere_x, sphere_y, sphere_z));
				indexed_normals.push_back(glm::normalize(glm::vec3(sphere_x, sphere_y, sphere_z)));
				indexed_uvs.push_back(glm::vec2(theta / 360.0f, phi / 180.0f + 0.5f));

				sphere_x = r * glm::cos(glm::radians((float)phi)) * glm::cos(glm::radians((float)(theta + d_angle)));
				sphere_y = r * glm::sin(glm::radians((float)phi));
				sphere_z = r * glm::cos(glm::radians((float)phi)) * glm::sin(glm::radians((float)(theta + d_angle)));
				indexed_vertices.push_back(glm::vec3(sphere_x, sphere_y, sphere_z));
				indexed_normals.push_back(glm::normalize(glm::vec3(sphere_x, sphere_y, sphere_z)));
				indexed_uvs.push_back(glm::vec2((theta + d_angle) / 360.0f, phi / 180.0f + 0.5f));

				sphere_x = r * glm::cos(glm::radians((float)(phi + d_angle))) * glm::cos(glm::radians((float)(theta + d_angle)));
				sphere_y = r * glm::sin(glm::radians((float)(phi + d_angle)));
				sphere_z = r * glm::cos(glm::radians((float)(phi + d_angle))) * glm::sin(glm::radians((float)(theta + d_angle)));
				indexed_vertices.push_back(glm::vec3(sphere_x, sphere_y, sphere_z));
				indexed_normals.push_back(glm::normalize(glm::vec3(sphere_x, sphere_y, sphere_z)));
				indexed_uvs.push_back(glm::vec2((theta + d_angle) / 360.0f, (phi + d_angle) / 180.0f + 0.5f));

				sphere_x = r * glm::cos(glm::radians((float)(phi + d_angle))) * glm::cos(glm::radians((float)theta));
				sphere_y = r * glm::sin(glm::radians((float)(phi + d_angle)));
				sphere_z = r * glm::cos(glm::radians((float)(phi + d_angle))) * glm::sin(glm::radians((float)theta));
				indexed_vertices.push_back(glm::vec3(sphere_x, sphere_y, sphere_z));
				indexed_normals.push_back(glm::normalize(glm::vec3(sphere_x, sphere_y, sphere_z)));
				indexed_uvs.push_back(glm::vec2(theta / 360.0f, (phi + d_angle) / 180.0f + 0.5f));

				indices.push_back(indices_index);
				indices.push_back(indices_index + 1);
				indices.push_back(indices_index + 2);
				indices.push_back(indices_index);
				indices.push_back(indices_index + 2);
				indices.push_back(indices_index + 3);
				indices_index += 4;
			}
		}

		VAOProcess();
	}
	else if (fileName == "CUBEMAPSPHERE") // Under construction
	{
		unsigned short indices_index = 0;
		int boundary = 5;
		int cube_x, cube_y, cube_z;
		glm::vec3 new_sphere;

		for (cube_z = -boundary; cube_z <= boundary; cube_z += 2 * boundary)
		{
			for (cube_x = -boundary; cube_x < boundary; cube_x++)
			{
				for (cube_y = -boundary; cube_y < boundary; cube_y++)
				{
					new_sphere = glm::vec3(
						cube_x / 10.0f * glm::sqrt(1.0f - cube_y / 10.0f * cube_y / 10.0f * 0.5f),
						cube_y / 10.0f * glm::sqrt(1.0f - cube_x / 10.0f * cube_x / 10.0f * 0.5f),
						cube_z / 10.0f);
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2(cube_x / 10.0f + 0.5f, cube_y / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						(cube_x + 1) / 10.0f * glm::sqrt(1.0f - cube_y / 10.0f * cube_y / 10.0f * 0.5f),
						cube_y / 10.0f * glm::sqrt(1.0f - (cube_x + 1) / 10.0f * (cube_x + 1) / 10.0f * 0.5f),
						cube_z / 10.0f);
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2((cube_x + 1) / 10.0f + 0.5f, cube_y / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						(cube_x + 1) / 10.0f * glm::sqrt(1.0f - (cube_y + 1) / 10.0f * (cube_y + 1) / 10.0f * 0.5f),
						(cube_y + 1) / 10.0f * glm::sqrt(1.0f - (cube_x + 1) / 10.0f * (cube_x + 1) / 10.0f * 0.5f),
						cube_z / 10.0f);
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2((cube_x + 1) / 10.0f + 0.5f, (cube_y + 1) / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						cube_x / 10.0f * glm::sqrt(1.0f - (cube_y + 1) / 10.0f * (cube_y + 1) / 10.0f * 0.5f),
						(cube_y + 1) / 10.0f * glm::sqrt(1.0f - cube_x / 10.0f * cube_x / 10.0f * 0.5f),
						cube_z / 10.0f);
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2(cube_x / 10.0f + 0.5f, (cube_y + 1) / 10.0f + 0.5f));

					indices.push_back(indices_index);
					indices.push_back(indices_index + 1);
					indices.push_back(indices_index + 2);
					indices.push_back(indices_index);
					indices.push_back(indices_index + 2);
					indices.push_back(indices_index + 3);
					indices_index += 4;
				}
			}
		}
		
		for (cube_x = -boundary; cube_x <= boundary; cube_x += 2 * boundary)
		{
			for (cube_z = -boundary; cube_z < boundary; cube_z++)
			{
				for (cube_y = -boundary; cube_y < boundary; cube_y++)
				{
					new_sphere = glm::vec3(
						cube_x / 10.0f,
						cube_y / 10.0f * glm::sqrt(1.0f - cube_z / 10.0f * cube_z / 10.0f * 0.5f),
						cube_z / 10.0f * glm::sqrt(1.0f - cube_y / 10.0f * cube_y / 10.0f * 0.5f));
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2(cube_z / 10.0f + 0.5f, cube_y / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						cube_x / 10.0f,
						cube_y / 10.0f * glm::sqrt(1.0f - (cube_z + 1) / 10.0f * (cube_z + 1) / 10.0f * 0.5f),
						(cube_z + 1) / 10.0f * glm::sqrt(1.0f - cube_y / 10.0f * cube_y / 10.0f * 0.5f));
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2((cube_z + 1) / 10.0f + 0.5f, cube_y / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						cube_x / 10.0f,
						(cube_y + 1) / 10.0f * glm::sqrt(1.0f - (cube_z + 1) / 10.0f * (cube_z + 1) / 10.0f * 0.5f),
						(cube_z + 1) / 10.0f * glm::sqrt(1.0f - (cube_y + 1) / 10.0f * (cube_y + 1) / 10.0f * 0.5f));
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2((cube_z + 1) / 10.0f + 0.5f, (cube_y + 1) / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						cube_x / 10.0f,
						(cube_y + 1) / 10.0f * glm::sqrt(1.0f - cube_z / 10.0f * cube_z / 10.0f * 0.5f),
						cube_z / 10.0f * glm::sqrt(1.0f - (cube_y + 1) / 10.0f * (cube_y + 1) / 10.0f * 0.5f));
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2(cube_z / 10.0f + 0.5f, (cube_y + 1) / 10.0f + 0.5f));

					indices.push_back(indices_index);
					indices.push_back(indices_index + 1);
					indices.push_back(indices_index + 2);
					indices.push_back(indices_index);
					indices.push_back(indices_index + 2);
					indices.push_back(indices_index + 3);
					indices_index += 4;
				}
			}
		}

		for (cube_y = -boundary; cube_y <= boundary; cube_y += 2 * boundary)
		{
			for (cube_z = -boundary; cube_z < boundary; cube_z++)
			{
				for (cube_x = -boundary; cube_x < boundary; cube_x++)
				{
					new_sphere = glm::vec3(
						cube_x / 10.0f * glm::sqrt(1.0f - cube_z / 10.0f * cube_z / 10.0f * 0.5f),
						cube_y / 10.0f,
						cube_z / 10.0f * glm::sqrt(1.0f - cube_x / 10.0f * cube_x / 10.0f * 0.5f));
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2(cube_x / 10.0f + 0.5f, cube_z / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						(cube_x + 1) / 10.0f * glm::sqrt(1.0f - cube_z / 10.0f * cube_z / 10.0f * 0.5f),
						cube_y / 10.0f,
						cube_z / 10.0f * glm::sqrt(1.0f - (cube_x + 1) / 10.0f * (cube_x + 1) / 10.0f * 0.5f));
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2((cube_x + 1) / 10.0f + 0.5f, cube_z / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						(cube_x + 1) / 10.0f * glm::sqrt(1.0f - (cube_z + 1) / 10.0f * (cube_z + 1) / 10.0f * 0.5f),
						cube_y / 10.0f,
						(cube_z + 1) / 10.0f * glm::sqrt(1.0f - (cube_x + 1) / 10.0f * (cube_x + 1) / 10.0f * 0.5f));
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2((cube_x + 1) / 10.0f + 0.5f, (cube_z + 1) / 10.0f + 0.5f));

					new_sphere = glm::vec3(
						cube_x / 10.0f * glm::sqrt(1.0f - (cube_z + 1) / 10.0f * (cube_z + 1) / 10.0f * 0.5f),
						cube_y / 10.0f,
						(cube_z + 1) / 10.0f * glm::sqrt(1.0f - cube_x / 10.0f * cube_x / 10.0f * 0.5f));
					new_sphere = glm::normalize(new_sphere);
					indexed_vertices.push_back(new_sphere);
					indexed_normals.push_back(glm::normalize(new_sphere));
					indexed_uvs.push_back(glm::vec2(cube_x / 10.0f + 0.5f, (cube_z + 1) / 10.0f + 0.5f));

					indices.push_back(indices_index);
					indices.push_back(indices_index + 1);
					indices.push_back(indices_index + 2);
					indices.push_back(indices_index);
					indices.push_back(indices_index + 2);
					indices.push_back(indices_index + 3);
					indices_index += 4;
				}
			}
		}

		VAOProcess();
	}
	else
	{
		loadOBJ(fileName);
		VBOIndex(vertices, uvs, normals);
	}
}

Model::
Model(const Model& model)
{
	indices = model.indices;
	indexed_vertices = model.indexed_vertices;
	indexed_uvs = model.indexed_uvs;
	indexed_normals = model.indexed_normals;

	pos = model.pos;
	VertexArrayID = model.VertexArrayID;
	vertexbuffer = model.vertexbuffer;
	uvbuffer = model.uvbuffer;
	normalbuffer = model.normalbuffer;
	elementbuffer = model.elementbuffer;
}

Model Model::
operator=(const Model& model)
{
	indices = model.indices;
	indexed_vertices = model.indexed_vertices;
	indexed_uvs = model.indexed_uvs;
	indexed_normals = model.indexed_normals;

	pos = model.pos;
	VertexArrayID = model.VertexArrayID;
	vertexbuffer = model.vertexbuffer;
	uvbuffer = model.uvbuffer;
	normalbuffer = model.normalbuffer;
	elementbuffer = model.elementbuffer;
	return *this;
}

// from opengl-tutorial
bool Model::
loadOBJ(const std::string fileName)
{
	printf("Loading OBJ file %s...\n", fileName.c_str());

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(fileName.c_str(), "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		//dgetchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		vertices.push_back(vertex);
		uvs.push_back(uv);
		normals.push_back(normal);

	}
	fclose(file);
	return true;
}


void Model::
draw()
{
	//bind VAO
	glBindVertexArray(VertexArrayID);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT,   // type
		(GLvoid*)0           // element array buffer offset
	);

	//unbind VAO
	glBindVertexArray(0);
}

void Model::
VBOIndex(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals)
{
	std::map<PackedVertex, unsigned short> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i < in_vertices.size(); i++) {

		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };


		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			indexed_vertices.push_back(in_vertices[i]);
			indexed_uvs.push_back(in_uvs[i]);
			indexed_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)indexed_vertices.size() - 1;
			indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}

	VAOProcess();
}

void Model::
VAOProcess()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &elementbuffer); // Generate a buffer for the indices as well

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(GLfloat),                  // stride
		(GLvoid*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		3 * sizeof(GLfloat),                                // stride
		(GLvoid*)0                          // array buffer offset
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		2 * sizeof(GLfloat),                                // stride
		(GLvoid*)0                          // array buffer offset
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	// Unbind VAO
	glBindVertexArray(0);
}

bool Model::
getSimilarVertexIndex_fast(PackedVertex& packed, std::map<PackedVertex, unsigned short>& VertexToOutIndex, unsigned short& result)
{
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}

Model::
~Model()
{
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
}