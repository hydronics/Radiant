#pragma once

#include <string>

namespace Radiant {
	
	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual bool operator==(const Texture& rhs) const = 0;
	};

	class Texture2d : public Texture
	{
	public:
		static Ref<Texture2d> Create(const std::string& texture_filepath);
		static Ref<Texture2d> Create(uint32_t width, uint32_t height);
	};

}
