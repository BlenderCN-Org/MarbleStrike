//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#import <QuartzCore/QuartzCore.h>
#import "EAGLView.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/sysctl.h>

//////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////

@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

//////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////

@implementation EAGLView
@synthesize mFramebufferWidth;
@synthesize mFramebufferHeight;
@synthesize mIsRetinaDisplay;
@synthesize mDefaultFramebuffer;
@synthesize mResolutionConfig;
@dynamic mContext;

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//#define USE_RETINA

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

NSString* GetPlatformString()
{
    // Gets a string with the device model
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = (char*)malloc(size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
    free(machine);
    /*
     if ([platform isEqualToString:@"iPhone1,1"])    return @"iPhone 2G";
     if ([platform isEqualToString:@"iPhone1,2"])    return @"iPhone 3G";
     if ([platform isEqualToString:@"iPhone2,1"])    return @"iPhone 3GS";
     if ([platform isEqualToString:@"iPhone3,1"])    return @"iPhone 4";
     if ([platform isEqualToString:@"iPhone3,2"])    return @"iPhone 4";
     if ([platform isEqualToString:@"iPhone3,3"])    return @"iPhone 4 (CDMA)";
     if ([platform isEqualToString:@"iPhone4,1"])    return @"iPhone 4S";
     if ([platform isEqualToString:@"iPhone5,1"])    return @"iPhone 5";
     if ([platform isEqualToString:@"iPhone5,2"])    return @"iPhone 5 (GSM+CDMA)";
     
     if ([platform isEqualToString:@"iPod1,1"])      return @"iPod Touch (1 Gen)";
     if ([platform isEqualToString:@"iPod2,1"])      return @"iPod Touch (2 Gen)";
     if ([platform isEqualToString:@"iPod3,1"])      return @"iPod Touch (3 Gen)";
     if ([platform isEqualToString:@"iPod4,1"])      return @"iPod Touch (4 Gen)";
     if ([platform isEqualToString:@"iPod5,1"])      return @"iPod Touch (5 Gen)";
     
     if ([platform isEqualToString:@"iPad1,1"])      return @"iPad";
     if ([platform isEqualToString:@"iPad1,2"])      return @"iPad 3G";
     if ([platform isEqualToString:@"iPad2,1"])      return @"iPad 2 (WiFi)";
     if ([platform isEqualToString:@"iPad2,2"])      return @"iPad 2";
     if ([platform isEqualToString:@"iPad2,3"])      return @"iPad 2 (CDMA)";
     if ([platform isEqualToString:@"iPad2,4"])      return @"iPad 2";
     if ([platform isEqualToString:@"iPad2,5"])      return @"iPad Mini (WiFi)";
     if ([platform isEqualToString:@"iPad2,6"])      return @"iPad Mini";
     if ([platform isEqualToString:@"iPad2,7"])      return @"iPad Mini (GSM+CDMA)";
     if ([platform isEqualToString:@"iPad3,1"])      return @"iPad 3 (WiFi)";
     if ([platform isEqualToString:@"iPad3,2"])      return @"iPad 3 (GSM+CDMA)";
     if ([platform isEqualToString:@"iPad3,3"])      return @"iPad 3";
     if ([platform isEqualToString:@"iPad3,4"])      return @"iPad 4 (WiFi)";
     if ([platform isEqualToString:@"iPad3,5"])      return @"iPad 4";
     if ([platform isEqualToString:@"iPad3,6"])      return @"iPad 4 (GSM+CDMA)";
     
     if ([platform isEqualToString:@"i386"])         return @"Simulator";
     if ([platform isEqualToString:@"x86_64"])       return @"Simulator";
     */
    return platform;
}

//===========================================================================

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
	if (self)
    {
		
		int w = 320;
		int h = 480;
        
        NSString* platform = GetPlatformString();
		
		mIsRetinaDisplay = NO;
		
		float ver = [[[UIDevice currentDevice] systemVersion] floatValue];
		// You can't detect screen resolutions in pre 3.2 devices, but they are all 320x480
		if (ver >= 3.2f)
		{
			UIScreen* mainscr = [UIScreen mainScreen];
			w = mainscr.currentMode.size.width;
			h = mainscr.currentMode.size.height;
            
            if ( w == 320 && h == 640 )
            {
                mResolutionConfig = giPhoneiPodConfig;
            }
            else if ( w == 640 && h == 960 )
            {
                mResolutionConfig = giPhoneiPodRetinaConfig;
            }
            else if ( w == 640 && h == 1136 )
            {
                mResolutionConfig = giPhoneiPodWideRetinaConfig;
            }
            else if ( w == 768 && h == 1024 )
            {
                mResolutionConfig = giPadConfig;
            }
            else if ( w == 1536 && h == 2048 )
            {
                mResolutionConfig = giPadRetinaConfig;
            }
            
            if (
                [platform isEqualToString:@"iPhone3,1"] ||
                [platform isEqualToString:@"iPhone3,2"] ||
                [platform isEqualToString:@"iPhone3,3"] ||
                [platform isEqualToString:@"iPod4,1"]
                )
            {
                mResolutionConfig = giPhoneiPodConfig;
            }
            else
            {
                CGFloat screenScale = [[UIScreen mainScreen] scale];
                if ( screenScale == 2.0f  )
                {
                    self.contentScaleFactor = 2.0;
                    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
                    eaglLayer.contentsScale = 2;
                    
                    mIsRetinaDisplay = YES;
                }
            }
        }
        else
        {
            mResolutionConfig = giPhoneiPodConfig;
        }
        
		
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
    }
    
    //[platform release];
    
    return self;
}

//===========================================================================

- (void)dealloc
{
    [self deleteFramebuffer];
    [mContext release];
    
    [super dealloc];
}

//===========================================================================

- (EAGLContext *)mContext
{
    return mContext;
}

//===========================================================================

- (void)setContext:(EAGLContext *)newContext
{
    if (mContext != newContext)
    {
        [self deleteFramebuffer];
        
        [mContext release];
        mContext = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

//===========================================================================

- (void)createFramebuffer
{
    if ( mContext && !mDefaultFramebuffer)
    {
        [EAGLContext setCurrentContext:mContext];
        
        // Create default framebuffer object.
        glGenFramebuffers(1, &mDefaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers(1, &mColorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderbuffer);
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mColorRenderbuffer);
        
        [mContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &mFramebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &mFramebufferHeight);
		
		// Generate our MSAA Frame and Render buffers
		//glGenFramebuffers( 1, &mDefaultMSAAFramebuffer);
		//glBindFramebuffer( GL_FRAMEBUFFER, mDefaultMSAAFramebuffer);
		
		//glGenRenderbuffers( 1, &mColorMSAARenderbuffer);
		//glBindRenderbuffer( GL_RENDERBUFFER, mColorMSAARenderbuffer);
		
		// Generate the msaaDepthBuffer.
		// 4 will be the number of pixels that the MSAA buffer will use in order to make one pixel on the render buffer.
		//int msaaSample = 1;
		//glRenderbufferStorageMultisampleAPPLE( GL_RENDERBUFFER, msaaSample, GL_RGBA8_OES, mFramebufferWidth, mFramebufferHeight);
		//glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mColorMSAARenderbuffer);
		//glGenRenderbuffers( 1, &mDepthMSAARenderbuffer);
		//glBindRenderbuffer( GL_RENDERBUFFER, mDepthMSAARenderbuffer);
		//glRenderbufferStorageMultisampleAPPLE( GL_RENDERBUFFER, msaaSample, GL_DEPTH_COMPONENT16, mFramebufferWidth, mFramebufferHeight);
		//glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthMSAARenderbuffer);
        
		glGenRenderbuffers(1, &mDepthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, mFramebufferWidth, mFramebufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_OES, mDepthRenderbuffer);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderbuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		}
    }
}

//===========================================================================

- (void)deleteFramebuffer
{
    if (mContext)
    {
        [EAGLContext setCurrentContext:mContext];
        
        if ( mDefaultFramebuffer )
        {
            glDeleteFramebuffers(1, &mDefaultFramebuffer);
            mDefaultFramebuffer = 0;
        }
		
		if ( mDefaultMSAAFramebuffer )
        {
            glDeleteFramebuffers(1, &mDefaultMSAAFramebuffer);
            mDefaultMSAAFramebuffer = 0;
        }
        
        if ( mColorRenderbuffer )
        {
            glDeleteRenderbuffers(1, &mColorRenderbuffer);
            mColorRenderbuffer = 0;
        }
		
		if ( mColorMSAARenderbuffer )
		{
			glDeleteRenderbuffers( 1, &mColorMSAARenderbuffer );
			mColorMSAARenderbuffer = 0;
		}
		
		if ( mDepthMSAARenderbuffer )
		{
			glDeleteRenderbuffers(1, &mDepthMSAARenderbuffer);
			mDepthMSAARenderbuffer = 0;
		}
    }
}

//===========================================================================

- (void)setFramebuffer
{
    if (mContext)
    {
        [EAGLContext setCurrentContext:mContext];
        
        if (!mDefaultMSAAFramebuffer)
        {
			[self createFramebuffer];
		}
        
        glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFramebuffer);
        
        glViewport(0, 0, mFramebufferWidth, mFramebufferHeight);
    }
}

//===========================================================================

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (mContext)
    {
        [EAGLContext setCurrentContext:mContext];
		
		//glBindFramebuffer( GL_DRAW_FRAMEBUFFER_APPLE, mDefaultFramebuffer);
		//glBindFramebuffer( GL_READ_FRAMEBUFFER_APPLE, mDefaultMSAAFramebuffer);
		//glResolveMultisampleFramebufferAPPLE();
        
		//GLenum discards[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
		//glDiscardFramebufferEXT( GL_READ_FRAMEBUFFER_APPLE, 2, discards );
		
        glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderbuffer);
        
        success = [mContext presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

//===========================================================================

- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}

@end
