#include "rdpch.h"

#include "Application.h"
#include "Log.h"
#include "Input.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

namespace Radiant { 
	Application* Application::s_application = nullptr;

	static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float: return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::Int: return GL_INT;
			case ShaderDataType::Int2: return GL_INT;
			case ShaderDataType::Int3: return GL_INT;
			case ShaderDataType::Int4: return GL_INT;
			case ShaderDataType::Mat3: return GL_FLOAT;
			case ShaderDataType::Mat4: return GL_FLOAT;
			case ShaderDataType::Bool: return GL_BOOL;
		}

		RD_CORE_ASSERT(false, "Unknown Shader data type!");
		return 0;
	}

	Application::Application()
	{
		RD_CORE_ASSERT(!s_application, "Application already exists!  Cannot create another instance!");
		s_application = this;

		m_window = std::unique_ptr<RadiantWindow>(RadiantWindow::Create());
		m_window->SetEventCallback(RD_BIND_EVENT_FN(Application::OnEvent));

		m_imgui_layer = new ImGuiLayer;
		PushOverlay(m_imgui_layer);

		glGenVertexArrays(1, &m_vertex_array);
		glBindVertexArray(m_vertex_array);

		float verts[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.2f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 0.0f,
		};
		m_vertex_buffer.reset(VertexBuffer::Create(sizeof(verts), verts));
		m_vertex_buffer->Bind();

		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_position"},
				{ShaderDataType::Float4, "a_color"}
			};
			m_vertex_buffer->SetLayout(layout);
		}

		unsigned int index = 0;
		const auto& layout = m_vertex_buffer->GetLayout();
		for (const auto& elem : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(	index,
									elem.GetComponentCount(),
									ShaderDataTypeToGLBaseType(elem.type),
									elem.normalized ? GL_TRUE : GL_FALSE,
									layout.GetStride(),
									(const void*)elem.offset);
			index++;
		}

		uint32_t indices[3] = { 0, 1, 2 };
		m_index_buffer.reset(IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));
		m_index_buffer->Bind();

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			out vec3 v_position;
			out vec4 v_color;
			
			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0f);
			}
		)";

		std::string pixelSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_position;
			in vec4 v_color;
			
			void main()
			{
				color = v_color;
			}
		)";

		m_shader.reset(new Shader(vertexSrc, pixelSrc));
	}

	Application::~Application()
	{

	}


	void Application::Run()
	{
		while (m_running)
		{
			glClearColor(0.3f, 0.4f, 0.8f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->Bind();

			glBindVertexArray(m_vertex_array);
			glDrawElements(GL_TRIANGLES, m_index_buffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (auto layer : m_layer_stack)
			{
				layer->OnUpdate();
			}
			m_imgui_layer->Begin();
			for (auto layer : m_layer_stack)
			{
				layer->OnImGuiRender();
			}
			m_imgui_layer->End();

			// @TODO:  This code will need to be abstracted to the Renderer API in Radiant.  

			m_window->OnUpdate();
		}
	}


	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(RD_BIND_EVENT_FN(Application::OnWindowClose));
		
		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.handled) break;
		}
	}


	void Application::PushOverlay(Layer* overlay)
	{
		m_layer_stack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layer_stack.PushLayer(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

}
