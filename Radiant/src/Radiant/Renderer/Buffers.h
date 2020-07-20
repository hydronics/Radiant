#pragma once

namespace Radiant {

	enum class ShaderDataType {
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Mat3,
		Mat4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type) {
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		RD_CORE_ASSERT(false, "Unknown Shader data type!");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		uint32_t size;
		uint32_t offset;
		bool normalized = false;
		
		ShaderDataType type;

		BufferElement() {}

		BufferElement(ShaderDataType t, std::string n, bool normalized = false)
			: name(n), size(ShaderDataTypeSize(t)), offset(0), type(t)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Bool:		return 1;
			}

			RD_CORE_ASSERT(false, "Unknown Shader data type!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elem_list)
			: m_elements(elem_list)
		{
			CalculateOffsetsAndStride();
		}


		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

		inline const std::vector<BufferElement> GetElements() const { return m_elements; }

		inline uint32_t GetStride() const { return m_stride; }

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_stride = 0;

			for (auto& element : m_elements)
			{
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride;

	};

	class VertexBuffer
	{
	public:
		VertexBuffer() {}
		virtual ~VertexBuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(uint32_t size, float* vertices);

	};

	class IndexBuffer
	{
	public:
		IndexBuffer() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t count, uint32_t* indices);

	};

}
