#pragma once
#include<vector>
#include<memory>

namespace TurboGE
{
	enum class AttribType { None = 0, Float, Float2, Float3, Float4, vec2, vec3, vec4, Int };

	struct VertexAttrib
	{
		unsigned int m_index;
		AttribType m_type;
		int m_size;
		bool m_normalized;
		int m_offset = 0;
		bool m_entityID = false;

	public:
		VertexAttrib(unsigned int index, AttribType type, bool normalized, bool entityID)
			:m_index{ index }, m_type{ type }, m_normalized{ normalized }, m_entityID{ entityID }
		{
		}
	};

	class VertexLayout
	{
		int m_stride = 0;
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
		static std::unique_ptr<VertexBuffer> Create(float*, uint32_t);
		static std::unique_ptr<VertexBuffer> Create(uint32_t);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual unsigned int getCount() = 0;
		
		static std::shared_ptr<IndexBuffer> Create(unsigned int*, uint32_t);
	};
}