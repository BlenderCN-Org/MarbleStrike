#if defined( RENDER_PLAT_OGLES2 )

#ifndef OGLES_WRAPPER_HPP
#define OGLES_WRAPPER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined( PLAT_PC )
#if defined( RENDER_PLAT_OPENGL )
#include <gl/gl.h>
#elif defined( RENDER_PLAT_OGLES2 )
#include "opengles2/include/gles2/gl2.h"
#include "opengles2/include/gles2/gl2ext.h"
#endif
#endif

#if defined( PLAT_IOS )
#import <OpenGLES/ES2/gl.h> 
#import <OpenGLES/ES2/glext.h> 
#endif

#if defined( PLAT_ANDROID )
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

//////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////

namespace OGLES
{
	void CheckError();

	void oglEnable( GLenum cap );
	void oglDisable( GLenum cap );
	void oglGetIntegerv( GLenum pname, GLint* params );
	void oglViewport( GLint x, GLint y, GLsizei width, GLsizei height );
	void oglCullFace( GLenum mode );
	void oglFrontFace( GLenum mode );
	void oglDepthFunc( GLenum func );
	void oglClearDepthf( GLclampf depth );
	void oglClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
	void oglDepthMask( GLboolean flag );
	void oglColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
	void oglClear( GLbitfield mask );	
	void oglBindTexture( GLenum target, GLuint texture );
	void oglGenTextures( GLsizei n, GLuint* textures );
	void oglDeleteTextures( GLsizei n, const GLuint* textures );
	void oglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels );
	void oglBlendFunc( GLenum sfactor, GLenum dfactor );
	void oglActiveTexture( GLenum texture );
	void oglTexParameteri( GLenum target, GLenum pname, GLint param );
	void oglTexParameterf( GLenum target, GLenum pname, GLfloat param );
	void oglTexParameterfv( GLenum target, GLenum pname, const GLfloat* param );
	void oglGenFramebuffers( GLsizei n, GLuint* framebuffers );
	void oglBindFramebuffer( GLenum target, GLuint framebuffer );
	void oglFramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
	void oglGenRenderbuffers( GLsizei n, GLuint* renderbuffers );
	void oglBindRenderbuffer( GLenum target, GLuint renderbuffer );
	void oglRenderbufferStorage( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );
	void oglFramebufferRenderbuffer( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );
	GLuint oglCreateProgram();
	void oglDeleteProgram( GLuint object );
	GLint oglGetUniformLocation( GLuint program, const char* name );
	void oglDeleteFramebuffers( GLsizei n, const GLuint* framebuffers );
	void oglDeleteRenderbuffers( GLsizei n, const GLuint* renderbuffers );
	void oglUseProgram( GLuint program );
	void oglEnableVertexAttribArray( GLuint index );
	void oglVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer );
	void oglDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
	void oglDrawArrays( GLenum mode, GLint start, GLint count );
	void oglDisableVertexAttribArray( GLuint index );
	GLuint oglCreateShader( GLenum type );
	void oglShaderSource( GLuint shader, GLsizei count, const char** string, const GLint* length );
	void oglCompileShader( GLuint shaderObj );
	GLint oglGetAttribLocation( GLuint program, const char* name );
	void oglUniform1i( GLint location, GLint x );
	void oglUniform1f( GLint location, GLfloat x );
	void oglUniform4fv( GLint location, GLsizei count, const GLfloat* v );
	void oglUniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
	void oglAttachShader( GLuint program, GLuint shader );
	void oglLinkProgram( GLuint program );
	void oglDetachShader( GLuint program, GLuint shader );
	void oglGenBuffers( GLsizei n, GLuint* buffers );
	void oglBindBuffer( GLenum target, GLuint buffer );
	void oglBufferData( GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage );
	void oglGetShaderiv (GLuint shader, GLenum pname, GLint* params);
	void oglGetShaderInfoLog (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
	void oglGetProgramiv( GLuint program, GLenum pname, GLint* params );
	void oglGetProgramInfoLog( GLuint program, GLsizei bufsize, GLsizei* length, char* infolog );
	void oglBindAttribLocation( GLuint program, GLuint index, const char* name );
	void oglStencilFuncSeparate(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask );
	void oglStencilOpSeparate(  GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass );
	void oglScissor( GLint x, GLint y, GLsizei width, GLsizei height );
	void oglGetActiveUniform( GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name );
	void oglCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
    void oglPolygonOffset( GLfloat factor, GLfloat units);
    void oglFlush();
}

#endif

#endif