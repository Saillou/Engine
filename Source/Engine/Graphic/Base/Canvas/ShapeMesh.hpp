#pragma once

#include <memory>

#include "Shape.hpp"
#include "../../Wrapper/Array.hpp"
#include "../../Wrapper/Buffer.hpp"

struct ShapeMesh {
	virtual ~ShapeMesh() = default;

	struct Vertex {
		float x = 0.0f;
		float y = 0.0f;
	};

	static void Draw(const CanvasShape& shape);

protected:
	ShapeMesh();

	void _draw();
	void _update_buffer(const CanvasShape::Points& points);

private:
	static void Create();
	static std::unique_ptr<ShapeMesh> _s_mesh;

	// Gpu data
	Array  m_vao;
	Buffer m_vbo;
};