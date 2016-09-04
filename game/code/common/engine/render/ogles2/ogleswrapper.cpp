#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////

namespace OGLES
{

	void CheckFrameBufferStatus()
	{
		GLenum status = ( GLenum )glCheckFramebufferStatus( GL_FRAMEBUFFER );
		switch ( status ) 
		{ 
		case GL_FRAMEBUFFER_COMPLETE:
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_UNSUPPORTED: 
			System::PrintToConsole("GL_FRAMEBUFFER_UNSUPPORTED_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n"); 
			break; 
		default: 
			System::PrintToConsole("UNKNWON\n"); 
			break;
		}
	}

	void CheckError()
	{
#if defined _DEBUG
		GLenum error = glGetError();
		Assert( error == GL_NO_ERROR, "" );
		switch( error )
		{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			System::PrintToConsole("GL_INVALID_ENUM\n");
			break;
		case GL_INVALID_VALUE:
			System::PrintToConsole("GL_INVALID_VALUE\n");
			break;
		case GL_INVALID_OPERATION:
			System::PrintToConsole("GL_INVALID_OPERATION\n");
			break;
		case GL_OUT_OF_MEMORY:
			System::PrintToConsole("GL_OUT_OF_MEMORY\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			System::PrintToConsole("GL_INVALID_FRAMEBUFFER_OPERATION_EXT\n");
			CheckFrameBufferStatus();
			break;
		default:
			System::PrintToConsole("UNKNWON\n");
			break;
		}
#endif        
	}

	void oglEnable( GLenum cap )
	{
		glEnable( cap );
		CheckError();
	}

	void oglDisable( GLenum cap )
	{
		glDisable( cap );
		CheckError();
	}

	void oglGetIntegerv( GLenum pname, GLint* params )
	{
		glGetIntegerv( pname, params );
		CheckError();
	}

	void oglViewport( GLint x, GLint y, GLsizei width, GLsizei height )
	{
		glViewport( x, y, width, height );
		CheckError();
	}

	void oglCullFace( GLenum mode )
	{
		glCullFace( mode );
		CheckError();
	}

	void oglFrontFace( GLenum mode )
	{
		glFrontFace( mode );
		CheckError();
	}

	void oglDepthFunc( GLenum func )
	{
		glDepthFunc( func );
		CheckError();
	}

	void oglClearDepthf( GLclampf depth )
	{
		glClearDepthf( depth );
		CheckError();
	}

	void oglClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
	{
		glClearColor( red, green, blue, alpha );
		CheckError();
	}

	void oglDepthMask( GLboolean flag )
	{
		glDepthMask( flag );
		CheckError();
	}

	void oglColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
	{
		glColorMask( red, green, blue, alpha );
		CheckError();
	}

	void oglClear( GLbitfield mask )
	{
		glClear( mask );
		CheckError();
	}

	void oglBindTexture( GLenum target, GLuint texture )
	{
		glBindTexture( target, texture );
		CheckError();
	}

	void oglDeleteTextures( GLsizei n, const GLuint *textures )
	{
		glDeleteTextures( n, textures );
		CheckError();
	};

	void oglGenTextures( GLsizei n, GLuint *textures )
	{
		glGenTextures( n, textures );
		CheckError();
	};

	void oglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels )
	{
		glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
		CheckError();
	}

	void oglBlendFunc( GLenum sfactor, GLenum dfactor )
	{
		glBlendFunc( sfactor, dfactor );
		CheckError();
	}

	void oglActiveTexture( GLenum texture )
	{
		glActiveTexture( texture );
		CheckError();
	}

	void oglTexParameteri( GLenum target, GLenum pname, GLint param )
	{
		glTexParameteri( target, pname, param );
		CheckError();
	}

	void oglTexParameterf( GLenum target, GLenum pname, GLfloat param )
	{
		glTexParameterf( target, pname, param );
		CheckError();
	}

	void oglTexParameterfv( GLenum target, GLenum pname, const GLfloat* param )
	{
		glTexParameterfv( target, pname, param );
		CheckError();
	}

	void oglGenFramebuffers( GLsizei n, GLuint* framebuffers )
	{
		glGenFramebuffers( n, framebuffers );
		CheckError();
	}

	void oglBindFramebuffer( GLenum target, GLuint framebuffer )
	{
		glBindFramebuffer( target, framebuffer );
		CheckError();
	}

	void oglFramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
	{
		glFramebufferTexture2D( target, attachment, textarget, texture, level );
		CheckError();
	}

	void oglGenRenderbuffers( GLsizei n, GLuint* renderbuffers )
	{
		glGenRenderbuffers( n, renderbuffers );
		CheckError();
	}

	void oglBindRenderbuffer( GLenum target, GLuint renderbuffer )
	{
		glBindRenderbuffer( target, renderbuffer );
		CheckError();
	}

	void oglRenderbufferStorage( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
	{
		glRenderbufferStorage( target, internalformat, width, height );
		CheckError();
	}

	void oglFramebufferRenderbuffer( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
	{
		glFramebufferRenderbuffer( target, attachment, renderbuffertarget, renderbuffer );
		CheckError();
	}

	GLuint oglCreateProgram()
	{
		GLuint handle = glCreateProgram();
		CheckError();
		return handle;
	}

	void oglDeleteProgram( GLuint object )
	{
		glDeleteProgram( object );
		CheckError();
	}

	GLint oglGetUniformLocation( GLuint program, const char* name )
	{
		GLint value = glGetUniformLocation( program, name );
		CheckError();
		return value;
	}

	void oglDeleteFramebuffers( GLsizei n, const GLuint* framebuffers )
	{
		glDeleteFramebuffers( n, framebuffers );
		CheckError();
	}

	void oglDeleteRenderbuffers( GLsizei n, const GLuint* renderbuffers )
	{
		glDeleteRenderbuffers( n, renderbuffers );
		CheckError();
	}

	void oglUseProgram( GLuint program )
	{
		glUseProgram( program );
		CheckError();
	}

	void oglEnableVertexAttribArray( GLuint index )
	{
		glEnableVertexAttribArray( index );
		CheckError();
	}

	void oglVertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer )
	{
		glVertexAttribPointer( index, size, type, normalized, stride, pointer );
		CheckError();
	}

	void oglDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
	{
		glDrawElements( mode, count, type, indices );
		CheckError();
	}

	void oglDrawArrays( GLenum mode, GLint start, GLint count )
	{
		glDrawArrays( mode, start, count );
		CheckError();
	}

	void oglDisableVertexAttribArray( GLuint index )
	{
		glDisableVertexAttribArray( index );
		CheckError();
	}

	GLuint oglCreateShader( GLenum type )
	{
		GLuint handle = glCreateShader( type );
		CheckError();
		return handle;
	}

	void oglShaderSource( GLuint shader, GLsizei count, const char** string, const GLint* length )
	{
		glShaderSource( shader, count, string, length );
		CheckError();
	}

	void oglCompileShader( GLuint shaderObj )
	{
		glCompileShader( shaderObj );
		CheckError();
	}

	GLint oglGetAttribLocation( GLuint program, const char* name )
	{
		GLint value = glGetAttribLocation( program, name );
		CheckError();
		return value;
	}

	void oglUniform1i( GLint location, GLint x )
	{
		glUniform1i( location, x );
		CheckError();
	}

	void oglUniform1f( GLint location, GLfloat x )
	{
		glUniform1f( location, x );
		CheckError();
	}

	void oglUniform4fv( GLint location, GLsizei count, const GLfloat* v )
	{
		glUniform4fv( location, count, v );
		CheckError();
	}

	void oglUniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* valueF )
	{
		glUniformMatrix4fv( location, count, transpose, valueF );
		CheckError();
	}

	void oglAttachShader( GLuint program, GLuint shader )
	{
		glAttachShader( program, shader );
		CheckError();
	}

	void oglLinkProgram( GLuint program )
	{
		glLinkProgram( program );
		CheckError();
	}

	void oglDetachShader( GLuint program, GLuint shader )
	{
		glDetachShader( program, shader );
		CheckError();
	}

	void oglGenBuffers( GLsizei n, GLuint* buffers )
	{
		glGenBuffers( n, buffers );
		CheckError();
	}

	void oglBindBuffer( GLenum target, GLuint buffer )
	{
		glBindBuffer( target, buffer );
		CheckError();
	}

	void oglBufferData( GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage )
	{
		glBufferData( target, size, data, usage );
		CheckError();
	}

	void oglGetShaderiv( GLuint shader, GLenum pname, GLint* params )
	{
		glGetShaderiv( shader, pname, params );
		CheckError();

	}
	void oglGetShaderInfoLog( GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog )
	{
		glGetShaderInfoLog( shader, bufsize, length, infolog );
		CheckError();
	}

	void oglGetProgramiv (GLuint program, GLenum pname, GLint* params)
	{
		glGetProgramiv( program, pname, params );
		CheckError();
	}

	void oglGetProgramInfoLog (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
	{
		glGetProgramInfoLog( program, bufsize, length, infolog );
		CheckError();
	}

	void oglBindAttribLocation( GLuint program, GLuint index, const char* name )
	{
		glBindAttribLocation( program, index, name );
		CheckError();
	}

	void oglStencilFuncSeparate(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask )
	{
		glStencilFuncSeparate( frontfunc, backfunc, ref, mask );
		CheckError();
	}

	void oglStencilOpSeparate(  GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass )
	{
		glStencilOpSeparate( face, sfail, dpfail, dppass );
		CheckError();
	}

	void oglScissor( GLint x, GLint y, GLsizei width, GLsizei height )
	{
		glScissor( x, y, width, height );
		CheckError();
	}

	void oglGetActiveUniform( GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name )
	{
		glGetActiveUniform( program, index, bufsize, length, size, type, name );
		CheckError();
	}

	void oglCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
	{
		glCompressedTexImage2D(target,level,internalformat,width,height,border,imageSize,data);
		CheckError();
	}
    
    void oglPolygonOffset( GLfloat factor, GLfloat units)
	{
		glPolygonOffset( factor, units );
		CheckError();
	}
    
    void oglFlush()
    {
        glFlush();
        CheckError();
    }
}

#endif