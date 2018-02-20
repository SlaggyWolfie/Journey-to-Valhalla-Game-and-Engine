#pragma once
#ifndef NEEDS_GL_SETUP_HPP
#define NEEDS_GL_SETUP_HPP

namespace Engine
{
	class NeedsGLSetup
	{
	public:
		NeedsGLSetup() = default;
		virtual ~NeedsGLSetup() = default;
	protected:
		virtual void setupGL() = 0;
	};
}

#endif