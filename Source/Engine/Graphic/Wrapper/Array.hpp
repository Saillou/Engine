#pragma once

class Array {
public:
	Array();
	virtual ~Array();

	Array& operator=(const Array&) = delete;
	Array(const Array&) = delete;
	Array(Array&&) = delete;

	void bind() const;
	void unbind() const;
	unsigned int getId() const;

protected:
	unsigned int m_array_id;
};
