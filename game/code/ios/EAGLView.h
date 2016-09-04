//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include "common/engine/game/resolutionconfigs.hpp"

//////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView
{
	// The pixel dimensions of the CAEAGLLayer.
    GLint mFramebufferWidth;
    GLint mFramebufferHeight;
    
    ResolutionConfig mResolutionConfig;
	
@private
    EAGLContext* mContext;
            
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    GLuint mDefaultFramebuffer;
	GLuint mColorRenderbuffer;
	GLuint mDepthRenderbuffer;
	
	GLuint mDefaultMSAAFramebuffer;
	GLuint mColorMSAARenderbuffer;
	GLuint mDepthMSAARenderbuffer;
	
	BOOL mIsRetinaDisplay;
}

@property (nonatomic, retain) EAGLContext* mContext;
@property (nonatomic) GLint mFramebufferWidth;
@property (nonatomic) GLint mFramebufferHeight;
@property (nonatomic) BOOL mIsRetinaDisplay;
@property (nonatomic) GLuint mDefaultFramebuffer;
@property (nonatomic) ResolutionConfig mResolutionConfig;

- (void)setContext:(EAGLContext *)newContext;
- (void)setFramebuffer;
- (BOOL)presentFramebuffer;

@end
