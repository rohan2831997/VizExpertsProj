#ifndef CURVE_MAKER_H
#define CURVE_MAKER_H

#include<vector>
#include"VAO.h"
#include"EBO.h"
#include"shaderClass.h"

struct sPoint2D
{
	float x;
	float y;
};

struct sSpline
{
	std::vector<sPoint2D> points;

	sPoint2D GetSplinePoint(float t, bool bLooped = false)
	{
		int p0, p1, p2, p3;
		if (!bLooped)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -ttt + 2.0f * tt - t;
		float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
		float q3 = -3.0f * ttt + 4.0f * tt + t;
		float q4 = ttt - tt;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return{ tx, ty };
	}
};



class curveMaker
{
private:
	std::vector<Vertex> vertices;
	std::vector <GLuint> indices;
	VAO vao;
	VAO vao_ext;
	std::vector<Vertex> vertices_ext;
	std::vector<GLuint> indices_ext;
	float ext_height;

	void initIndices()
	{
		int size = vertices.size();
		for (int i = 0; i < size - 1; i++)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
		}


		for (int i = 0; i < (vertices.size() - 1); i++)
		{
			int i1 = 0 + 2 * i;
			int i2 = 1 + 2 * i;
			int i3 = 3 + 2 * i;
			int i4 = 3 + 2 * i;
			int i5 = 2 + 2 * i;
			int i6 = 0 + 2 * i;

			indices_ext.push_back(i1);
			indices_ext.push_back(i2);
			indices_ext.push_back(i3);
			indices_ext.push_back(i4);
			indices_ext.push_back(i5);
			indices_ext.push_back(i6);
		}

	}

	void initVertices(std::vector<Vertex>& vertices_)
	{
		Vertex V;
		V.position = glm::vec3{ -0.5f,-0.5f,0.0f };
		V.color = glm::vec3{ 0.0f,1.0f,0.0f };

		Vertex E;
		E.position = glm::vec3{ 0.5f,0.5f,0.0f };
		V.color = glm::vec3{ 0.0f,1.0f,0.0f };

		vertices_.insert(vertices_.begin(), V);
		vertices_.push_back(E);

		for (int i = 0; i < vertices_.size() - 3; i++)
		{
			sSpline curve;

			sPoint2D point1;
			point1.x = vertices_[i].position.x;
			point1.y = vertices_[i].position.y;
			curve.points.push_back(point1);

			sPoint2D point2;
			point2.x = vertices_[i+1].position.x;
			point2.y = vertices_[i+1].position.y;
			curve.points.push_back(point2);

			sPoint2D point3;
			point3.x = vertices_[i+2].position.x;
			point3.y = vertices_[i+2].position.y;
			curve.points.push_back(point3);

			sPoint2D point4;
			point4.x = vertices_[i+3].position.x;
			point4.y = vertices_[i+3].position.y;
			curve.points.push_back(point4);


			for (float t = 0; t < 1.0f; t += 0.1f)
			{
				sPoint2D pos = curve.GetSplinePoint(t, false);
				Vertex v;
				v.position = glm::vec3{ pos.x,pos.y,0.0f };
				v.color = glm::vec3{ 0.0f,1.0f,0.0f };
				vertices.push_back(v);
			}
		}

		for (int i = 0; i < vertices.size()-1; i++)
		{
			glm::vec3 grad_vec (vertices[i+1].position.x- vertices[i].position.x, 
								vertices[i + 1].position.y - vertices[i].position.y,
								0.0f);

			glm::vec3 normal = glm::cross(grad_vec, glm::vec3(0.0f, 0.0f, -1.0f));
			normal = glm::normalize(normal);

			float length1 = glm::length(normal);
			
			glm::vec3 p1 = (vertices[i].position + (normal* ext_height));
			glm::vec3 p2 = (vertices[i].position - (normal * ext_height));

			glm::vec3 color_p1 = glm::vec3{ 1.0f,0.0f,0.0f };
			glm::vec3 color_p2 = glm::vec3{ 1.0f,0.0f,0.0f };

			Vertex v1;
			v1.position = p1;
			v1.color = color_p1;
			vertices_ext.push_back(v1);

			Vertex v2;
			v2.position = p2;
			v2.color = color_p2;
			vertices_ext.push_back(v2);


		}


		//for the last vertex
		{
			int index = vertices.size() - 1;

			glm::vec3 grad_vec(vertices[index].position.x - vertices[index - 1].position.x,
				vertices[index].position.y - vertices[index - 1].position.y,
				0.0f);

			glm::vec3 normal = glm::cross(grad_vec, glm::vec3(0.0f, 0.0f, -1.0f));
			normal = glm::normalize(normal);

			float length1 = glm::length(normal);

			glm::vec3 p1 = (vertices[index].position + (normal * ext_height));
			glm::vec3 p2 = (vertices[index].position - (normal * ext_height));

			glm::vec3 color_p1 = glm::vec3{ 1.0f,0.0f,0.0f };
			glm::vec3 color_p2 = glm::vec3{ 1.0f,0.0f,0.0f };

			Vertex v1;
			v1.position = p1;
			v1.color = color_p1;
			vertices_ext.push_back(v1);

			Vertex v2;
			v2.position = p2;
			v2.color = color_p2;
			vertices_ext.push_back(v2);
		}

	}
public:
	curveMaker(std::vector<Vertex> &vertices_, float height) : ext_height(height)
	{	
		initVertices(vertices_);
		initIndices();

		vao.Bind();
		// Generates Vertex Buffer Object and links it to vertices
		VBO VBO_(vertices);
		// Generates Element Buffer Object and links it to indices
		EBO EBO_(indices);
		// Links VBO attributes such as coordinates and colors to VAO
		vao.LinkAttrib(VBO_, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		vao.LinkAttrib(VBO_, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

		// Unbind all to prevent accidentally modifying them
		vao.Unbind();
		VBO_.Unbind();
		EBO_.Unbind();//put values into indices

		//Same for the extruded part
		vao_ext.Bind();
		// Generates Vertex Buffer Object and links it to vertices
		VBO VBO_ext(vertices_ext);
		// Generates Element Buffer Object and links it to indices
		EBO EBO_ext(indices_ext);
		// Links VBO attributes such as coordinates and colors to VAO
		vao.LinkAttrib(VBO_ext, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		vao.LinkAttrib(VBO_ext, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));

		// Unbind all to prevent accidentally modifying them
		vao_ext.Unbind();
		VBO_ext.Unbind();
		EBO_ext.Unbind();//put values into indices
	};

	void Draw(Shader& shader)
	{
		shader.Activate();
		vao.Bind();
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	void Draw_ext(Shader& shader)
	{
		shader.Activate();
		vao_ext.Bind();
		glDrawElements(GL_TRIANGLES, indices_ext.size(), GL_UNSIGNED_INT, 0);
	}
};

#endif // !CURVE_MAKER_H
