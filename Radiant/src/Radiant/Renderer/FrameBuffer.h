#pragma once

#include "Texture.h"

namespace Radiant {

	struct FrameBufferProperties {
		uint32_t width = 1280, height = 720;
		//FrameBufferFormat format;
		uint32_t samples = 1;

		bool swap_chain_target = false; //true means render directly to screen frame buffer
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		virtual const FrameBufferProperties& GetProps() const = 0;
		virtual uint32_t GetColorAttachmentId() const = 0;


		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static Ref<FrameBuffer> Create(const FrameBufferProperties& props);
	};

}
