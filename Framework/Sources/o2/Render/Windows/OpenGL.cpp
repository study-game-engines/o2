#include "o2/stdafx.h"

#ifdef PLATFORM_WINDOWS

#include "OpenGL.h"
#include "o2/Utils/Debug/Log/LogStream.h"

// Returns address of function
PROC GetSafeWGLProcAddress(const char* id, o2::LogStream* log)
{
	PROC res = wglGetProcAddress(id);
	if (!res)
		log->Error("Failed to get function address: " + (o2::String)id);

	return res;
}

void GetGLExtensions(o2::LogStream* log /*= nullptr*/)
{
	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)GetSafeWGLProcAddress("glGenFramebuffersEXT", log);
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)GetSafeWGLProcAddress("glBindFramebufferEXT", log);
	glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)GetSafeWGLProcAddress("glFramebufferTexture", log);
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)GetSafeWGLProcAddress("glDrawBuffers", log);
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)GetSafeWGLProcAddress("glDeleteBuffers", log);
	glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSPROC)GetSafeWGLProcAddress("glDeleteFramebuffersEXT", log);
	glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)GetSafeWGLProcAddress("glCheckFramebufferStatusEXT", log);

}

bool IsGLExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;

	GLubyte *where, *terminator;
	/* Extension names should not have spaces. */

	where = (GLubyte *)strchr(extension, ' ');

	if (where || *extension == '\0')
		return 0;

	extensions = glGetString(GL_EXTENSIONS);

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */

	start = extensions;
	for (;;)
	{
		where = (GLubyte *)strstr((const char *)start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;

		start = terminator;
	}

	return false;
}

const char* GetGLErrorDesc(GLenum errorId)
{
	if (errorId == GL_NO_ERROR) return "GL_NO_ERROR";
	if (errorId == GL_INVALID_ENUM) return "GL_INVALID_ENUM";
	if (errorId == GL_INVALID_VALUE) return "GL_INVALID_VALUE";
	if (errorId == GL_INVALID_OPERATION) return "GL_INVALID_OPERATION";
	if (errorId == GL_INVALID_FRAMEBUFFER_OPERATION) return "GL_INVALID_FRAMEBUFFER_OPERATION";
	if (errorId == GL_OUT_OF_MEMORY) return "GL_OUT_OF_MEMORY";
	if (errorId == GL_STACK_UNDERFLOW) return "GL_STACK_UNDERFLOW";
	if (errorId == GL_STACK_OVERFLOW) return "GL_STACK_OVERFLOW";

	return "UNKNOWN";
}

void glCheckError(const char* filename /*= nullptr*/, unsigned int line /*= 0*/)
{
	GLenum errId = glGetError();
	if (errId != GL_NO_ERROR)
	{
		o2Debug.LogError("OpenGL ERROR " + (o2::String)errId + ": " + (o2::String)GetGLErrorDesc(errId) +
						 " at file: " + (o2::String)(filename ? filename : "unknown") + " line: " + (o2::String)line);
	}
}

extern PFNGLGENFRAMEBUFFERSEXTPROC        glGenFramebuffersEXT = NULL;
extern PFNGLBINDFRAMEBUFFEREXTPROC        glBindFramebufferEXT = NULL;
extern PFNGLFRAMEBUFFERTEXTUREPROC        glFramebufferTexture = NULL;
extern PFNGLDRAWBUFFERSPROC               glDrawBuffers = NULL;
extern PFNGLDELETEBUFFERSPROC             glDeleteBuffers = NULL;
extern PFNGLDELETEFRAMEBUFFERSPROC        glDeleteFramebuffersEXT = NULL;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;

#endif // PLATFORM_WINDOWS
