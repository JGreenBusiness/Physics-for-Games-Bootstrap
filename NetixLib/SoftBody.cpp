#include "pch.h"
#include "SoftBody.h"
#include "Circle.h"
#include "Spring.h"

void SoftBody::Build(PhysicsScene* _scene, glm::vec2 _position, float _damping, float _springForce, float _spacing, std::vector<std::string>& _strings)
{
	int numColumns = _strings.size();
	int numRows = _strings[0].length();

	// traverse across the array and add balls where the ascii art says they should be
	Circle** circles = new Circle * [numRows * numColumns];
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			if (_strings[j][i] == '0')
			{
				circles[i * numColumns + j] = new Circle(_position + glm::vec2(i, j) * _spacing, glm::vec2(0, 0), 1.0f, 2.0f, glm::vec4(1, 0, 0, 1));
				_scene->AddActor(circles[i * numColumns + j]);
			}
			else
			{
				circles[i * numColumns + j] = nullptr;
			}
		}
	}

	// second pass - add springs in
	for (int i = 1; i < numRows; i++)
	{
		for (int j = 1; j < numColumns; j++)
		{
			Circle* s11 = circles[i * numColumns + j];
			Circle* s01 = circles[(i - 1) * numColumns + j];
			Circle* s10 = circles[i * numColumns + j - 1];
			Circle* s00 = circles[(i - 1) * numColumns + j - 1];
			                                    
			// make springs to cardinal neighbours
			if (s11 && s01)
				_scene->AddActor(new Spring(s11, s01, _damping, _springForce, _spacing, glm::vec4(1)));
			if (s11 && s10)
				_scene->AddActor(new Spring(s11, s10, _damping, _springForce, _spacing, glm::vec4(1)));
			if (s10 && s00)
				_scene->AddActor(new Spring(s10, s00, _damping, _springForce, _spacing, glm::vec4(1)));
			if (s01 && s00)
				_scene->AddActor(new Spring(s01, s00, _damping, _springForce, _spacing, glm::vec4(1)));

			if (s11 && s00)
				_scene->AddActor(new Spring(s11, s00, _damping, _springForce, _spacing*sqrt(2.0f), glm::vec4(1)));
			if (s01 && s10)												   
				_scene->AddActor(new Spring(s01, s10, _damping, _springForce, _spacing * sqrt(2.0f), glm::vec4(1)));

		}
	}
}
