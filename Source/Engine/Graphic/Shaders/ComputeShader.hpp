#pragma once

#include "Shader.hpp"

class ComputeShader :  public Shader {
public:
	ComputeShader();
	~ComputeShader() = default;

	void compute(int groupX, int groupY, int groupZ);
	void syncStorage();
};

