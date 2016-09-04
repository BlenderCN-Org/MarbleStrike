#if defined( RENDER_PLAT_OPENGL )

#ifndef OGLWRAPPER_HPP
#define OGLWRAPPER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "gl/glew.h"
#include <windows.h>
#include <gl/GL.h>

//////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////

namespace OGL
{
	void CheckError();

	void oglEnable( GLenum cap );
	void oglDisable( GLenum cap );
	void oglViewport( GLint x, GLint y, GLsizei width, GLsizei height );
	void oglCullFace( GLenum mode );
	void oglFrontFace( GLenum mode );
	void oglDepthFunc( GLenum func );
	void oglClearDepth( GLclampd depth );
	void oglClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
	void oglDepthMask( GLboolean flag );
	void oglColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
	void oglClear( GLbitfield mask );
	void oglMatrixMode( GLenum mode );
	void oglLoadIdentity();
	void oglBindTexture( GLenum target, GLuint texture );
	void oglGenTextures( GLsizei n, GLuint* textures );
	void oglDeleteTextures( GLsizei n, const GLuint* textures );
	void oglCompressedTexImage2D( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data );
	void oglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels );
	void oglClipPlane( GLenum plane, const GLdouble* equation );
	void oglBlendFunc( GLenum sfactor, GLenum dfactor );
	void oglAlphaFunc( GLenum func, GLclampf ref );
	void oglActiveTexture( GLenum texture );
	void oglTexParameteri( GLenum target, GLenum pname, GLint param );
	void oglTexParameterf( GLenum target, GLenum pname, GLfloat param );
	void oglTexParameterfv( GLenum target, GLenum pname, const GLfloat* param );
	void oglGenFramebuffersEXT( GLsizei n, GLuint* framebuffers );
	void oglBindFramebufferEXT( GLenum target, GLuint framebuffer );
	void oglFramebufferTexture2DEXT( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
	void oglGenRenderbuffersEXT( GLsizei n, GLuint* renderbuffers );
	void oglBindRenderbufferEXT( GLenum target, GLuint renderbuffer );
	void oglRenderbufferStorageEXT( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );
	void oglFramebufferRenderbufferEXT( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );
	GLhandleARB oglCreateProgramObject();
	void oglDeleteObject( GLhandleARB object );
	void oglGetProgramiv( GLuint program, GLenum pname, GLint* params );
	void oglGetActiveUniform( GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name );
	GLint oglGetUniformLocation( GLhandleARB program, const GLcharARB *name );
	void oglDeleteFramebuffersEXT( GLsizei n, const GLuint* framebuffers );
	void oglDeleteRenderbuffersEXT( GLsizei n, const GLuint* renderbuffers );
	void oglUseProgramObject( GLhandleARB programObj );
	void oglVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
	void oglEnableClientState( GLenum array );
	void oglNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer );
	void oglTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
	void oglColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
	void oglEnableVertexAttribArray( GLuint index );
	void oglVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer );
	void oglDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
	void oglDrawArrays( GLenum mode, GLint start, GLint count );
	void oglDisableVertexAttribArray( GLuint index );
	void oglDisableClientState( GLenum array );
	void oglBegin( GLenum mode );
	void oglTexCoord2f( GLfloat s, GLfloat t );
	void oglVertex2f( GLfloat x, GLfloat y );
	void oglEnd();
	GLhandleARB oglCreateShaderObject( GLenum shaderType );
	void oglShaderSource( GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint *length );
	void oglCompileShader( GLhandleARB shaderObj );
	void oglGetObjectParameteriv( GLhandleARB obj, GLenum pname, GLint* params );
	void oglGetInfoLog( GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog );
	GLint oglGetAttribLocation( GLhandleARB programObj, const GLcharARB* name );
	void oglUniform1i( GLint location, GLint v0 );
	void oglUniform1f( GLint location, GLfloat v0 );
	void oglUniform4fv( GLint location, GLsizei count, const GLfloat* value );
	void oglUniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
	void oglAttachObject( GLhandleARB containerObj, GLhandleARB obj );
	void oglLinkProgram( GLhandleARB programObj );
	void oglDetachObject( GLhandleARB containerObj, GLhandleARB attachedObj );
	void oglBlendEquation( GLenum mode );
	void oglBlendEquationSeparate( GLenum modeRGB, GLenum modeAlpha );
	void oglBlendFuncSeparate( GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha );
	void oglPolygonMode( GLenum face, GLenum mode );
	void oglClientActiveTexture( GLenum texture );
	void oglGenBuffers( GLsizei n, GLuint* buffers );
	void oglBindBuffer( GLenum target, GLuint buffer );
	void oglBufferData( GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage );
	void oglGetBufferSubData( GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data );
	void oglBindAttribLocation( GLuint program, GLuint index, const char* name );
	void oglPointSize( GLfloat size );
	void oglGetIntegerv( GLenum pname, GLint* params );
	const GLubyte* oglGetString( GLenum pname );
	void oglDrawBuffers( GLsizei n, const GLenum* bufs );
	void oglStencilFuncSeparate( GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask );
	void oglStencilOpSeparate( GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass );
	void oglScissor( GLint x, GLint y, GLsizei width, GLsizei height );
	void oglPixelStorei( GLenum pname, GLint param );
	void oglPolygonOffset( GLfloat factor, GLfloat units);
    void oglFlush();
}

#endif

#endif