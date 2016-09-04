#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////

#if defined PLAT_PC

//////////////////////////////////////////////////////
// FRAME BUFFER OBJECTS
//////////////////////////////////////////////////////

typedef GLboolean (GL_APIENTRY *PFNGLISRENDERBUFFEROES)(GLuint renderbuffer);
typedef void (GL_APIENTRY *PFNGLBINDRENDERBUFFEROES)(GLenum target, GLuint renderbuffer);
typedef void (GL_APIENTRY *PFNGLDELETERENDERBUFFERSOES)(GLsizei n, const GLuint *renderbuffers);
typedef void (GL_APIENTRY *PFNGLGENRENDERBUFFERSOES)(GLsizei n, GLuint *renderbuffers);
typedef void (GL_APIENTRY *PFNGLRENDERBUFFERSTORAGEOES)(GLenum target, GLenum internalformat,GLsizei width, GLsizei height);
typedef void (GL_APIENTRY *PFNGLGETRENDERBUFFERPARAMETERIVOES)(GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GL_APIENTRY *PFNGLISFRAMEBUFFEROES)(GLuint framebuffer);
typedef void (GL_APIENTRY *PFNGLBINDFRAMEBUFFEROES)(GLenum target, GLuint framebuffer);
typedef void (GL_APIENTRY *PFNGLDELETEFRAMEBUFFERSOES)(GLsizei n, const GLuint *framebuffers);
typedef void (GL_APIENTRY *PFNGLGENFRAMEBUFFERSOES)(GLsizei n, GLuint *framebuffers);
typedef GLenum (GL_APIENTRY *PFNGLCHECKFRAMEBUFFERSTATUSOES)(GLenum target);
typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERTEXTURE2DOES)(GLenum target, GLenum attachment,GLenum textarget, GLuint texture,GLint level);
typedef void (GL_APIENTRY *PFNGLFRAMEBUFFERRENDERBUFFEROES)(GLenum target, GLenum attachment,GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GL_APIENTRY *PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES)(GLenum target, GLenum attachment,GLenum pname, GLint *params);
typedef void (GL_APIENTRY *PFNGLGENERATEMIPMAPOES)(GLenum target);

PFNGLISRENDERBUFFEROES glIsRenderbufferOES;
PFNGLBINDRENDERBUFFEROES glBindRenderbufferOES;
PFNGLDELETERENDERBUFFERSOES glDeleteRenderbuffersOES;
PFNGLGENRENDERBUFFERSOES glGenRenderbuffersOES;
PFNGLRENDERBUFFERSTORAGEOES glRenderbufferStorageOES;
PFNGLGETRENDERBUFFERPARAMETERIVOES glGetRenderbufferParameterivOES;
PFNGLISFRAMEBUFFEROES glIsFramebufferOES;
PFNGLBINDFRAMEBUFFEROES glBindFramebufferOES;
PFNGLDELETEFRAMEBUFFERSOES glDeleteFramebuffersOES;
PFNGLGENFRAMEBUFFERSOES glGenFramebuffersOES;
PFNGLCHECKFRAMEBUFFERSTATUSOES glCheckFramebufferStatusOES;
PFNGLFRAMEBUFFERTEXTURE2DOES glFramebufferTexture2DOES;
PFNGLFRAMEBUFFERRENDERBUFFEROES glFramebufferRenderbufferOES;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES glGetFramebufferAttachmentParameterivOES;
PFNGLGENERATEMIPMAPOES glGenerateMipmapOES;

#endif

namespace FixedOGL
{

	void CheckFrameBufferStatus()
	{
		GLenum status = ( GLenum )glCheckFramebufferStatusOES( GL_FRAMEBUFFER_OES );
		switch ( status ) 
		{ 
		case GL_FRAMEBUFFER_COMPLETE_OES: 	
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES\n"); 
			break; 
		case GL_FRAMEBUFFER_UNSUPPORTED_OES: 
			System::PrintToConsole("GL_FRAMEBUFFER_UNSUPPORTED_OES\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES\n"); 
			break; 
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES: 
			System::PrintToConsole("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES\n"); 
			break; 
		default: 
			System::PrintToConsole("UNKNWON\n"); 
			break;
		}
	}

	void CheckError()
	{
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
		case GL_INVALID_FRAMEBUFFER_OPERATION_OES:
			System::PrintToConsole("GL_INVALID_FRAMEBUFFER_OPERATION_OES\n");
			CheckFrameBufferStatus();
			break;
		default:
			System::PrintToConsole("UNKNWON\n");
			break;
		}
	}

	void InitializeExt()
	{
#if defined PLAT_PC
		const GLubyte *pszGLExtensions;
		pszGLExtensions = glGetString( GL_EXTENSIONS );
		if ( strstr( ( char* )pszGLExtensions, "GL_OES_framebuffer_object" ) )
		{				
			glIsRenderbufferOES = (PFNGLISRENDERBUFFEROES)eglGetProcAddress("glIsRenderbufferOES") ;
			glBindRenderbufferOES = (PFNGLBINDRENDERBUFFEROES)eglGetProcAddress("glBindRenderbufferOES");
			glDeleteRenderbuffersOES = (PFNGLDELETERENDERBUFFERSOES)eglGetProcAddress("glDeleteRenderbuffersOES");
			glGenRenderbuffersOES = (PFNGLGENRENDERBUFFERSOES)eglGetProcAddress("glGenRenderbuffersOES");
			glRenderbufferStorageOES = (PFNGLRENDERBUFFERSTORAGEOES)eglGetProcAddress("glRenderbufferStorageOES");
			glGetRenderbufferParameterivOES = (PFNGLGETRENDERBUFFERPARAMETERIVOES)eglGetProcAddress("glGetRenderbufferParameterivOES");
			glIsFramebufferOES = (PFNGLISFRAMEBUFFEROES)eglGetProcAddress("glIsFramebufferOES");
			glBindFramebufferOES = (PFNGLBINDFRAMEBUFFEROES)eglGetProcAddress("glBindFramebufferOES");
			glDeleteFramebuffersOES = (PFNGLDELETEFRAMEBUFFERSOES)eglGetProcAddress("glDeleteFramebuffersOES");
			glGenFramebuffersOES = (PFNGLGENFRAMEBUFFERSOES)eglGetProcAddress("glGenFramebuffersOES");
			glCheckFramebufferStatusOES = (PFNGLCHECKFRAMEBUFFERSTATUSOES)eglGetProcAddress("glCheckFramebufferStatusOES");
			glFramebufferTexture2DOES = (PFNGLFRAMEBUFFERTEXTURE2DOES)eglGetProcAddress("glFramebufferTexture2DOES");
			glFramebufferRenderbufferOES = (PFNGLFRAMEBUFFERRENDERBUFFEROES)eglGetProcAddress("glFramebufferRenderbufferOES");
			glGetFramebufferAttachmentParameterivOES = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOES)eglGetProcAddress("glGetFramebufferAttachmentParameterivOES");
			glGenerateMipmapOES = (PFNGLGENERATEMIPMAPOES)eglGetProcAddress("glGetFramebufferAttachmentParameterivOES");
		}
		else
		{
			Assert( false, "Error: GL_OES_frambuffer_object not supported" );
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

	void oglPointSize( GLfloat size )
	{
		glPointSize( size );
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

	void oglClearDepth( GLclampf depth )
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

	void oglMatrixMode( GLenum mode )
	{
		glMatrixMode( mode );
		CheckError();
	}

	void oglLoadMatrixf( const GLfloat* m )
	{
		glLoadMatrixf( m );
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

	void oglTexImage2D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels )
	{
		glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
		CheckError();
	}

	void oglClipPlane( GLenum plane, const GLfloat* equation )
	{		
		glClipPlanef( plane, equation );
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

	void oglTexParameterfv( GLenum target, GLenum pname, const GLfloat* param )
	{
		glTexParameterfv( target, pname, param );
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

	void oglDisableClientState( GLenum array )
	{
		glDisableClientState( array );
		CheckError();
	}

	void oglColor4fv( const GLfloat* v )
	{
		glColor4f( v[0], v[1], v[2], v[3] );
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

	void oglLightfv( GLenum light, GLenum pname, const GLfloat* params )
	{
		glLightfv( light, pname, params );
		CheckError();
	}

	void oglShadeModel( GLenum mode )
	{
		glShadeModel( mode );
		CheckError();
	}

	void oglMultMatrixf( const GLfloat* m )
	{
		glMultMatrixf( m );
		CheckError();
	}

	void oglMaterialfv( GLenum face, GLenum pname, const GLfloat *params )
	{
		glMaterialfv( face, pname,  params );
		CheckError();
	}

	void oglGenFramebuffers( GLsizei n, GLuint* framebuffers )
	{		
		glGenFramebuffersOES( n, framebuffers );
		CheckError();
	}

	void oglBindFramebuffer( GLenum target, GLuint framebuffer )
	{
		glBindFramebufferOES( target, framebuffer );
		CheckError();
	}

	void oglFramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
	{
		glFramebufferTexture2DOES( target, attachment, textarget, texture, level );
		CheckError();
	}

	void oglGenRenderbuffers( GLsizei n, GLuint *renderbuffers )
	{
		glGenRenderbuffersOES( n, renderbuffers );
		CheckError();
	}

	void oglBindRenderbuffer( GLenum target, GLuint renderbuffer )
	{
		glBindRenderbufferOES( target, renderbuffer );
		CheckError();
	}

	void oglRenderbufferStorage( GLenum target, GLenum internalformat, GLsizei width, GLsizei height )
	{
		glRenderbufferStorageOES( target, internalformat, width, height );
		CheckError();
	}

	void oglFramebufferRenderbuffer( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer )
	{
		glFramebufferRenderbufferOES( target, attachment, renderbuffertarget, renderbuffer );
		CheckError();
	}

	void oglDeleteFramebuffers( GLsizei n, const GLuint *framebuffers )
	{
		glDeleteFramebuffersOES( n, framebuffers );
		CheckError();
	}

	void oglDeleteRenderbuffers( GLsizei n, const GLuint *renderbuffers )
	{
		glDeleteRenderbuffersOES( n, renderbuffers );
		CheckError();
	}

	const GLubyte* oglGetString( GLenum pname )
	{
		const GLubyte* getString = glGetString( pname );
		CheckError();
		return getString;
	}

	void oglStencilFunc( GLenum func, GLint ref, GLuint mask )
	{
		glStencilFunc( func, ref, mask );
		CheckError();
	}

	void oglStencilOp( GLenum fail, GLenum zfail, GLenum zpass )
	{
		glStencilOp( fail, zfail, zpass );
		CheckError();
	}

	void oglScissor( GLint x, GLint y, GLsizei width, GLsizei height )
	{
		glScissor( x, y, width, height );
		CheckError();
	}

	void oglCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
	{
		glCompressedTexImage2D(target,level,internalformat,width,height,border,imageSize,data);
		CheckError();
	}
}

#endif