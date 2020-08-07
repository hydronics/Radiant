#pragma once

#include "Radiant/Renderer/FrameBuffer.h"

namespace Radiant {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferProperties& props);

		virtual ~OpenGLFrameBuffer();
		virtual const FrameBufferProperties& GetProps() const override { return m_props; }

		void Invalidate();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentId() const override { return m_color_attachment_tex; }

	private:
		FrameBufferProperties m_props;
		uint32_t m_renderer_id = 0;
		uint32_t m_color_attachment_tex = 0;
		uint32_t m_depth_attachment = 0;
	};

}
