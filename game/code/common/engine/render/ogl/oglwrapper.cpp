#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////

namespace OGL
{

	int IsExtensionSupported( const char* extension )
	{
		const GLubyte* extensions = NULL;
		const GLubyte* start;
		GLubyte* where;
		GLubyte* terminator;

		where = ( GLubyte* )strchr( extension, ' ' );

		if ( where || *extension == '\0' )
		{
			return 0;
		}

		extensions = glGetString( GL_EXTENSIONS );
		start = extensions;
		for (;;) 
		{
			where = ( GLubyte* ) strstr( ( const char* ) start, extension );
			if ( !where )
			{
				break;
			}

			terminator = where + strlen( extension );
			if ( where == start || *( where - 1 ) == ' ')
			{
				if ( *terminator == ' ' || *terminator == '\0' )
				{
					return 1;
				}
			}
			start = terminator;
		}

		return 0;
	}

	void CheckFrameBufferStatus()
	{
		GLenum status = ( GLenum )glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
		switch ( status ) 
		{ 
		case GL_FRAMEBUFFER_COMPLETE_EXT: 			
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT: 
			System::PrintToConsole("GL_FRAMEBUFFER_UNSUPPORTED_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n"); 
			break; 
		default: 
			System::PrintToConsole("UNKNWON\n"); 
			break;
		}
	}

	void CheckError()
	{
#if defined ( _DEBUG )
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
		case GL_STACK_OVERFLOW:
			System::PrintToConsole("GL_STACK_OVERFLOW\n");
			break;
		case GL_STACK_UNDERFLOW:
			System::PrintToConsole("GL_STACK_UNDERFLOW\n");
			break;
		case GL_OUT_OF_MEMORY:
			System::PrintToConsole("GL_OUT_OF_MEMORY\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
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

	void oglClearDepth( GLclampd depth )
	{
		glClearDepth( depth );
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

	void oglMatrixMode( GLenum mode )
	{
		glMatrixMode( mode );
		CheckError();
	}

	void oglLoadIdentity()
	{
		glLoadIdentity();
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

	void oglCompressedTexImage2D( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data )
	{
		glCompressedTexImage2D( target, level, internalformat, width, height, border, imageSize, data );
		CheckError();
	}

	void oglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels )
	{
		glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
		CheckError();
	}

	void oglClipPlane( GLenum plane, const GLdouble* equation )
	{
		glClipPlane( plane, equation );
		CheckError();
	}

	void oglBlendFunc( GLenum sfactor, GLenum dfactor )
	{
		glBlendFunc( sfactor, dfactor );
		CheckError();
	}

	void oglAlphaFunc( GLenum func, GLclampf ref )
	{
		glAlphaFunc( func, ref );
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

	void oglTexParameterfv( GLenum target, GLenum pname, CONST GLfloat* param )
	{
		glTexParameterfv( target, pname, param );
		CheckError();
	}

	void oglGenFramebuffersEXT( GLsizei n, GLuint* framebuffers )
	{
		glGenFramebuffersEXT( n, framebuffers );
		CheckError();
	}

	void oglBindFramebufferEXT( GLenum target, GLuint framebuffer )
	{
		glBindFramebufferEXT( target, framebuffer );
		CheckError();
	}

	void oglFramebufferTexture2DEXT( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
	{
		glFramebufferTexture2DEXT( target, attachment, textarget, texture, level );
		CheckError();
	}

	void oglGenRenderbuffersEXT( GLsizei n, GLuint* renderbuffers )
	{
		glGenRenderbuffersEXT( n, renderbuffers );
		CheckError();
	}

	void oglBindRenderbufferEXT( GLenum target, GLuint renderbuffer )
	{
		glBindRenderbufferEXT( target, renderbuffer );
		CheckError();
	}

	void oglRenderbufferStorageEXT( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
	{
		glRenderbufferStorageEXT( target, internalformat, width, height );
		CheckError();
	}

	void oglFramebufferRenderbufferEXT( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
	{
		glFramebufferRenderbufferEXT( target, attachment, renderbuffertarget, renderbuffer );
		CheckError();
	}

	GLhandleARB oglCreateProgramObject()
	{
		GLhandleARB handle = glCreateProgramObjectARB();
		CheckError();
		return handle;
	}

	void oglDeleteObject( GLhandleARB object )
	{
		glDeleteObjectARB( object );
		CheckError();
	}

	void oglGetProgramiv( GLuint program, GLenum pname, GLint* params )
	{
		glGetProgramiv( program, pname, params );
		CheckError();
	}

	void oglGetActiveUniform( GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name )
	{
		glGetActiveUniform( program, index, bufSize, length, size, type, name );
		CheckError();
	}

	GLint oglGetUniformLocation( GLhandleARB program, const GLcharARB *name )
	{
		GLint value = glGetUniformLocationARB( program, name );
		CheckError();
		return value;
	}

	void oglDeleteFramebuffersEXT( GLsizei n, const GLuint* framebuffers )
	{
		glDeleteFramebuffersEXT( n, framebuffers );
		CheckError();
	}

	void oglDeleteRenderbuffersEXT( GLsizei n, const GLuint* renderbuffers )
	{
		glDeleteRenderbuffersEXT( n, renderbuffers );
		CheckError();
	}

	void oglUseProgramObject( GLhandleARB programObj )
	{
		glUseProgramObjectARB( programObj );
		CheckError();
	}

	void oglVertexPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
	{
		glVertexPointer( size, type, stride, pointer );
		CheckError();
	}

	void oglEnableClientState( GLenum array )
	{
		glEnableClientState( array );
		CheckError();
	}

	void oglNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer )
	{
		glNormalPointer( type, stride, pointer );
		CheckError();
	}

	void oglTexCoordPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
	{
		glTexCoordPointer( size, type, stride, pointer );
		CheckError();
	}

	void oglColorPointer( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer )
	{
		glColorPointer( size, type, stride, pointer );
		CheckError();
	}

	void oglEnableVertexAttribArray( GLuint index )
	{
		glEnableVertexAttribArrayARB( index );
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
		glDisableVertexAttribArrayARB( index );
		CheckError();
	}

	void oglDisableClientState( GLenum array )
	{
		glDisableClientState( array );
		CheckError();
	}

	void oglBegin( GLenum mode )
	{
		glBegin( mode );
	}

	void oglTexCoord2f( GLfloat s, GLfloat t )
	{
		glTexCoord2f( s, t );
	}

	void oglVertex2f( GLfloat x, GLfloat y )
	{
		glVertex2f( x, y );
	}

	void oglEnd()
	{
		glEnd();
		CheckError();
	}

	GLhandleARB oglCreateShaderObject( GLenum shaderType )
	{
		GLhandleARB handle = glCreateShaderObjectARB( shaderType );
		CheckError();
		return handle;
	}

	void oglShaderSource( GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint *length )
	{
		glShaderSourceARB( shaderObj, count, string, length );
		CheckError();
	}

	void oglCompileShader( GLhandleARB shaderObj )
	{
		glCompileShaderARB( shaderObj );
		CheckError();
	}

	void oglGetObjectParameteriv( GLhandleARB obj, GLenum pname, GLint* params )
	{
		glGetObjectParameterivARB( obj, pname, params );
		CheckError();
	}

	void oglGetInfoLog( GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog )
	{
		glGetInfoLogARB( obj, maxLength, length, infoLog );
		CheckError();
	}

	GLint oglGetAttribLocation( GLhandleARB programObj, const GLcharARB* name )
	{
		GLint value = glGetAttribLocationARB( programObj, name );
		CheckError();
		return value;
	}

	void oglUniform1i( GLint location, GLint v0 )
	{
		glUniform1iARB( location, v0 );
		CheckError();
	}

	void oglUniform1f( GLint location, GLfloat v0 )
	{
		glUniform1fARB( location, v0 );
		CheckError();
	}

	void oglUniform4fv( GLint location, GLsizei count, const GLfloat* value )
	{
		glUniform4fvARB( location, count, value );
		CheckError();
	}

	void oglUniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value )
	{
		glUniformMatrix4fv( location, count, transpose, value );
		CheckError();
	}

	void oglAttachObject( GLhandleARB containerObj, GLhandleARB obj )
	{
		glAttachObjectARB( containerObj, obj );
		CheckError();
	}

	void oglLinkProgram( GLhandleARB programObj )
	{
		glLinkProgramARB( programObj );
		CheckError();
	}

	void oglDetachObject( GLhandleARB containerObj, GLhandleARB attachedObj )
	{
		glDetachObjectARB( containerObj, attachedObj );
		CheckError();
	}

	void oglBlendEquation( GLenum mode )
	{
		glBlendEquation( mode );
		CheckError();
	}

	void oglBlendEquationSeparate( GLenum modeRGB, GLenum modeAlpha )
	{
		glBlendEquationSeparate( modeRGB, modeAlpha );
		CheckError();
	}

	void oglBlendFuncSeparate( GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha )
	{
		glBlendFuncSeparate( sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha );
		CheckError();
	}

	void oglPolygonMode( GLenum face, GLenum mode )
	{
		glPolygonMode( face, mode);
		CheckError();
	}

	void oglClientActiveTexture( GLenum texture )
	{
		glClientActiveTexture( texture );
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

	void oglGetBufferSubData( GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data )
	{
		glGetBufferSubData( target, offset, size, data );
		CheckError();
	}

	void oglBindAttribLocation( GLuint program, GLuint index, const char* name )
	{
		glBindAttribLocation( program, index, name );
		CheckError();
	}

	void oglPointSize( GLfloat size )
	{
		glPointSize( size );
		CheckError();
	}

	void oglGetIntegerv( GLenum pname, GLint* params )
	{
		glGetIntegerv( pname, params );
		CheckError();
	}

	const GLubyte* oglGetString( GLenum pname )
	{
		const GLubyte* getString = glGetString( pname );
		CheckError();
		return getString;
	}

	void oglDrawBuffers( GLsizei n, const GLenum* bufs )
	{
		glDrawBuffers( n, bufs );
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

	void oglPixelStorei( GLenum pname, GLint param )
	{
		glPixelStorei( pname, param );
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