#pragma once
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <glad/glad.h>

#include "VertexBuffer.h";
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	unsigned int ID;

	VertexArray()
	{
		glGenVertexArrays(1, &ID);
	}

	void AddBuffer(VertexBuffer& vertexBuffer, VertexBufferLayout& layout)
	{
		Bind();
		vertexBuffer.Bind();

		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
		//vertexBuffer.Unbind();
	}

	void Bind()
	{
		glBindVertexArray(ID);
	}

	void Unbind()
	{
		glBindVertexArray(0);
	}

	void Delete()
	{
		glDeleteVertexArrays(1, &ID);
	}
};

#endif
