#pragma once

class Array {
public:
	Array();
	virtual ~Array();

	void bind();
	void unbind();
	unsigned int getId() const;

protected:
	unsigned int m_array_id;
};
