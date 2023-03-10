#pragma once
#include<vector>

namespace TurboGE
{
	enum class AttribType { None = 0, Float, Float2, Float3, Float4, vec2, vec3, vec4 };

	struct VertexAttrib
	{
		unsigned int m_index;
		AttribType m_type;
		int m_size;
		bool m_normalized;
		int m_offset = 0; //This is the offset for start of the vertex attribute in bytes

	public:
		VertexAttrib(unsigned int index, AttribType type, bool normalized)
			:m_index{ index }, m_type{ type }, m_normalized{ normalized }
		{
		}
	};

	// USED attribVec IN CLASS BECAUSE IF I USE OUTSIDE IN THIS FILE, LINKER ERROR DUE TO ALREADY DEFINED IN .OBJ
	// WHY NOT DIRECTLY DECLARE IN APPLICATION.CPP? MAYBE VECTOR IS REQUIRED OUTSIDE SCOPE OF APPLICATION.CPP LATER
	class VertexLayout
	{
		int m_stride = 0; //This is the number of bytes for each vertex
	public:
		std::vector<VertexAttrib> m_attribVec;
		int typeToStride(AttribType, int&);
		void MakeLayout();
		inline int getStride() { return m_stride; }

	};


	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void SetBatchData(uint32_t, const void*) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static VertexBuffer* Create(float*, uint32_t);
		static VertexBuffer* Create(uint32_t);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual unsigned int getCount() = 0;
		
		static IndexBuffer* Create(unsigned int*, uint32_t);
	};
}