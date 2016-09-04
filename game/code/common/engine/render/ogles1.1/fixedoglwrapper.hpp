#if defined( RENDER_PLAT_OGLES1_1)

#ifndef FIXED_OGLWRAPPER_HPP
#define FIXED_OGLWRAPPER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PC
#include "opengles1.1/include/gles/gl.h"
#include "opengles1.1/include/gles/egl.h"
#include "opengles1.1/include/gles/glext.h"
#endif

#if defined PLAT_IOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif

//////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////

namespace FixedOGL
{
	void CheckError();
	void CheckFrameBufferStatus();

	void InitializeExt();

	void oglEnable( GLenum cap );
	void oglDisable( GLenum cap );
	void oglGetIntegerv( GLenum pname, GLint* params );
	void oglPointSize( GLfloat size );
	void oglViewport( GLint x, GLint y, GLsizei width, GLsizei height );
	void oglCullFace( GLenum mode );
	void oglFrontFace( GLenum mode );
	void oglDepthFunc( GLenum func );
	void oglClearDepth( GLclampf depth );
	void oglClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
	void oglDepthMask( GLboolean flag );
	void oglColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
	void oglClear( GLbitfield mask );
	void oglMatrixMode( GLenum mode );
	void oglLoadMatrixf( const GLfloat* m );
	void oglLoadIdentity();
	void oglBindTexture( GLenum target, GLuint texture );
	void oglGenTextures( GLsizei n, GLuint* textures );
	void oglDeleteTextures( GLsizei n, const GLuint* textures );
	void oglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels );
	void oglClipPlane( GLenum plane, const GLfloat* equation );
	void oglBlendFunc( GLenum sfactor, GLenum dfactor );
	void oglAlphaFunc( GLenum func, GLclampf ref );
	void oglActiveTexture( GLenum texture );
	void oglTexParameteri( GLenum target, GLenum pname, GLint param );
	void oglTexParameterf( GLenum target, GLenum pname, GLfloat param );
	void oglTexParameterfv( GLenum target, GLenum pname, const GLfloat* param );	
	void oglVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
	void oglEnableClientState( GLenum array );
	void oglNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
	void oglTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );	
	void oglColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
	void oglDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
	void oglDrawArrays( GLenum mode, GLint start, GLint count );	
	void oglDisableClientState( GLenum array );	
	void oglColor4fv( const GLfloat* v );
	void oglClientActiveTexture( GLenum texture );
	void oglGenBuffers( GLsizei n, GLuint* buffers );
	void oglBindBuffer( GLenum target, GLuint buffer );
	void oglBufferData( GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage );
	void oglLightfv( GLenum light, GLenum pname, const GLfloat* params );
	void oglShadeModel( GLenum mode );
	void oglMultMatrixf( const GLfloat* m );
	void oglMaterialfv( GLenum face, GLenum pname, const GLfloat *params );
	void oglGenFramebuffers( GLsizei n, GLuint* framebuffers );
	void oglBindFramebuffer( GLenum target, GLuint framebuffer );
	void oglFramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
	void oglGenRenderbuffers( GLsizei n, GLuint *renderbuffers );
	void oglBindRenderbuffer( GLenum target, GLuint renderbuffer );
	void oglRenderbufferStorage( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );
	void oglFramebufferRenderbuffer( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );	
	void oglDeleteFramebuffers( GLsizei n, const GLuint *framebuffers );
	void oglDeleteRenderbuffers( GLsizei n, const GLuint *renderbuffers );	
	const GLubyte* oglGetString( GLenum pname );
	void oglStencilFunc( GLenum func, GLint ref, GLuint mask );
	void oglStencilOp( GLenum fail, GLenum zfail, GLenum zpass );
	void oglScissor( GLint x, GLint y, GLsizei width, GLsizei height );
	void oglCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
}

#endif

#endif