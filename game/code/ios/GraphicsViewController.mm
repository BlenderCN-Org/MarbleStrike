//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#import <QuartzCore/QuartzCore.h>
#import "GraphicsViewController.h"
#import "EAGLView.h"
#import <iAd/ADBannerView.h>

#include "common/engine/database/database.hpp"
#include "common/engine/game/game.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/game/resolutionconfigs.hpp"
#include "common/engine/input/inputmanager.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/profilermanager.hpp"
#include "ios/input/iosinputmanager.hpp"
#include <sys/stat.h>
#include <sys/types.h>

#if defined RENDER_PLAT_OGLES1_1
#include "common/engine/render/ogles1.1/fixedoglrenderer.hpp"
#endif

#if defined RENDER_PLAT_OGLES2
#include "common/engine/render/ogles2/oglesrenderer.hpp"
#endif

//////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////

static EAGLContext* gContextThread;

@implementation GraphicsViewController

@synthesize animating;
@synthesize mContextMain;
@synthesize mSharedGroup;
@synthesize mAdBannerView;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

- (void)awakeFromNib
{
    mIsBannerVisible = NO;
    
#if defined RENDER_PLAT_OGLES1_1
    self.mContextMain = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
#endif
    
#if defined RENDER_PLAT_OGLES2
    self.mContextMain = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    gContextThread = [[EAGLContext alloc] initWithAPI:[self.mContextMain API] sharegroup:[self.mContextMain sharegroup]];
#endif
    
    if (!self.mContextMain)
    {
		NSLog(@"Failed to create ES context");
	}
    else if (![EAGLContext setCurrentContext:self.mContextMain])
	{
		NSLog(@"Failed to set ES context current");
	}
	
    [(EAGLView *)self.view setContext:self.mContextMain];
    [(EAGLView *)self.view setFramebuffer];
    
    [self.view setMultipleTouchEnabled:YES];
    
    animating = FALSE;
    mDisplayLinkSupported = FALSE;
    mAnimationFrameInterval = 1;
    mDisplayLink = nil;
    mAnimationTimer = nil;
    
    // Use of CADisplayLink requires iOS version 3.1 or greater.
	// The NSTimer object is used as fallback when it isn't available.
    NSString *reqSysVer = @"3.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
    {
		mDisplayLinkSupported = TRUE;
	}
	
	EAGLView* eaglView = (EAGLView*)self.view;
    
	System::Memory::Create();
	
	Database::Create();
    
    ResolutionConfig currentResolutionConfig = eaglView.mResolutionConfig;
    
    Database::Get()->SetBackBufferWidth( eaglView.mFramebufferHeight );
    Database::Get()->SetBackBufferHeight( eaglView.mFramebufferWidth );
	Database::Get()->SetIsRetinaDisplay( eaglView.mIsRetinaDisplay );
	Database::Get()->SetTouchScreenWidth( eaglView.mFramebufferHeight );
	Database::Get()->SetTouchScreenHeight( eaglView.mFramebufferWidth );
    
	NSString *defaultDBPath = [[NSBundle mainBundle] resourcePath];
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *libraryDirectory = [paths objectAtIndex:0];
	
    char buffer[256];
	strcpy( buffer, [defaultDBPath UTF8String] );
	strcat( buffer, "/ios/" );
    
    char libraryPath[256];
    strcpy( libraryPath, [libraryDirectory UTF8String] );
    strcat( libraryPath, "/ios/" );
	
	Database::Get()->SetResourcePath( buffer );
    Database::Get()->SetLibraryPath( libraryPath );
    
    struct stat st;
    if ( stat( libraryPath, &st ) != 0 )
    {
        mkdir( libraryPath, S_IRWXU );
    }
    
    strcat( libraryPath, "game/" );
    if ( stat( libraryPath, &st ) != 0 )
    {
        mkdir( libraryPath, S_IRWXU );
    }
    
    strcat( libraryPath, "bowler/" );
    if ( stat( libraryPath, &st ) != 0 )
    {
        mkdir( libraryPath, S_IRWXU );
    }
    
	GameApp::Create();
    GameApp::Get()->SetScreenWidth( currentResolutionConfig.mGameScreenWidth );
    GameApp::Get()->SetScreenHeight( currentResolutionConfig.mGameScreenHeight );
    GameApp::Get()->SetIsLanscape( false );
	GameApp::Get()->CreateRenderer();
    GameApp::Get()->Initialize( GameApp::GAME_TYPE_BOWLER );
	Database::Get()->SetIsLandscape( GameApp::Get()->GetIsLandscape() );
	
#if defined RENDER_PLAT_OGLES1_1
	FixedOGLRenderer* fixedOGLRenderer = reinterpret_cast<FixedOGLRenderer*>( Renderer::Get() );
	fixedOGLRenderer->SetDefaultFramebuffer( eaglView.mDefaultFramebuffer );
#endif
    
#if defined RENDER_PLAT_OGLES2
	OGLESRenderer* oglesRenderer = reinterpret_cast<OGLESRenderer*>( Renderer::Get() );
	oglesRenderer->SetDefaultFramebuffer( eaglView.mDefaultFramebuffer );
#endif
    
    mTempClass = new TempClass;
    mTempClass->mViewController = self;
    //GameApp::Get()->GetMessageHandler()->RegisterListener( PORT_GAME, &OnMessageCallback, this );
    
    [self CreateAdBannerView];
}

//===========================================================================

- (void)dealloc
{
    delete mTempClass;
    
    // Tear down context.
    if ([EAGLContext currentContext] == self.mContextMain)
    {
		[EAGLContext setCurrentContext:nil];
	}
    
    [self.mContextMain release];
    [gContextThread release];
	
    [super dealloc];
}

//===========================================================================

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

//===========================================================================

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

//===========================================================================


-(void)viewDidLoad
{
    
}

//===========================================================================

- (void)viewDidUnload
{
	[super viewDidUnload];
	
	GameApp::Get()->Shutdown();
	GameApp::Destroy();
	
	Database::Terminate();
	
	System::Memory::Destroy();
	
    // Tear down context.
    if ([EAGLContext currentContext] == self.mContextMain)
	{
		[EAGLContext setCurrentContext:nil];
	}
	self.mContextMain = nil;
    gContextThread = nil;
}

//===========================================================================

- (NSInteger)animationFrameInterval
{
    return mAnimationFrameInterval;
}

//===========================================================================

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    //
	// Frame interval defines how many display frames must pass between each time the display link fires.
	// The display link will only fire 30 times a second when the frame internal is two on a display that
	// refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second
	// when the display refreshes at 60 times a second. A frame interval setting of less than one results
	// in undefined behavior.
	//
    if (frameInterval >= 1)
    {
        mAnimationFrameInterval = frameInterval;
        
        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

//===========================================================================

- (void)startAnimation
{
    GameApp* game = GameApp::Get();
    if ( game )
    {
        GameApp::Get()->SetShow();
    }
    
    if (!animating)
    {
        if ( mDisplayLinkSupported )
        {
            //
			// CADisplayLink is API new in iOS 3.1. Compiling against earlier versions will result in a warning,
			// but can be dismissed if the system version runtime check for CADisplayLink exists in -awakeFromNib.
			// The runtime check ensures this code will not be called in system versions earlier than 3.1.
			//
            mDisplayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawFrame)];
            [mDisplayLink setFrameInterval:mAnimationFrameInterval];
            
            // The run loop will retain the display link on add.
            [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
		{
			/*mAnimationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * mAnimationFrameInterval)
             target:self
             selector:@selector(drawFrame)
             userInfo:nil
             repeats:TRUE];*/
		}
        
        animating = TRUE;
    }
}

//===========================================================================

- (void)stopAnimation
{
    if ( GameApp::Get() )
    {
        GameApp::Get()->SetHide();
        Database::Get()->SetWasSuspended( true );
        glFlush();
    }
    
    if (animating)
    {
        if ( mDisplayLinkSupported)
        {
            [mDisplayLink invalidate];
            mDisplayLink = nil;
        }
        else
        {
            [mAnimationTimer invalidate];
            mAnimationTimer = nil;
        }
        
        animating = FALSE;
    }
}

//===========================================================================

- (void)drawFrame
{
    int index = System::ProfilerManager::Get()->BeginProfiling( "drawFrame" );
    [(EAGLView *)self.view setFramebuffer];
    
    //
	// Draw/Render
	//
	GameApp::Get()->Update();
    
    int index0 = System::ProfilerManager::Get()->BeginProfiling( "presentFramebuffer" );
    [(EAGLView *)self.view presentFramebuffer];
    System::ProfilerManager::Get()->EndProfiling( index0 );
    
    System::ProfilerManager::Get()->EndProfiling( index );
        
    System::ProfilerManager::Get()->Print();
    System::ProfilerManager::Get()->Clear();
}

//===========================================================================

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

//===========================================================================

- (void)mouseDown:(CGPoint)location touchID:(int)ID
{
	GameApp* gameApp = GameApp::Get();
	if ( gameApp != NULL )
	{
        float scale = 1.0f;
        if ( Database::Get()->GetIsRetinaDisplay() )
        {
            scale = 2.0f;
        }
		float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
		float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
		float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
		float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );
        float x = ( appScreenWidth / touchScreenWidth ) * location.x * scale;
		float y = ( appScreenHeight / touchScreenHeight ) * location.y * scale;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
        
		gameApp->GetInputManager()->GetInputManagerPlat()->TouchDown( ID, xInt, yInt );
	}
}

//===========================================================================

- (void)mouseUp:(CGPoint)location touchID:(int)ID
{
    GameApp* gameApp = GameApp::Get();
	if ( gameApp != NULL )
	{
        float scale = 1.0f;
        if ( Database::Get()->GetIsRetinaDisplay() )
        {
            scale = 2.0f;
        }
        
		float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
		float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
		float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
		float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );
        float x = ( appScreenWidth / touchScreenWidth ) * location.x * scale;
		float y = ( appScreenHeight / touchScreenHeight ) * location.y * scale;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
        
		gameApp->GetInputManager()->GetInputManagerPlat()->TouchUp( ID, xInt, yInt );
	}
}

//===========================================================================

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration
{
	GameApp* gameApp = GameApp::Get();
	if ( gameApp != NULL )
	{
		gameApp->GetInputManager()->GetInputManagerPlat()->Accelerometer(acceleration.x, acceleration.y, acceleration.z);
	}
}

//===========================================================================

- (void)handleSwipeUp:(UISwipeGestureRecognizer *)recognizer
{
	GameApp* gameApp = GameApp::Get();
	if ( gameApp != NULL )
	{
		gameApp->GetInputManager()->GetInputManagerPlat()->SetSwipeDirection( 0, SWIPE_DIR_UP );
	}
}

//===========================================================================

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	GameApp* gameApp = GameApp::Get();
	if ( gameApp != NULL )
	{
		int numTouches = (int)[touches count];
		gameApp->GetInputManager()->GetInputManagerPlat()->SetNumTouches( numTouches );
        
        for ( int i = 0; i < numTouches; ++i )
        {
            UITouch *touch = [[touches allObjects] objectAtIndex:i];
            UIView* view = touch.view;
            bool isAdBannerView = [view isMemberOfClass:[ADBannerView class]];
            if(!isAdBannerView)
            {
                CGPoint point = [touch locationInView:(EAGLView *)self.view];
                int touchInt = *((int*)&touch);
                [self mouseDown:point touchID:touchInt];
            }
        }
		
		mStartZoom = true;
	}
}

//===========================================================================

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	GameApp* gameApp = GameApp::Get();
	if ( gameApp != NULL )
	{
		int numTouches = (int)[touches count];
		gameApp->GetInputManager()->GetInputManagerPlat()->SetNumTouches( numTouches );
		
        for ( int i = 0; i < numTouches; ++i )
        {
            UITouch *touch = [[touches allObjects] objectAtIndex:i];
			CGPoint point = [touch locationInView:(EAGLView *)self.view];
            int touchInt = *((int*)&touch);
			[self mouseUp:point touchID:touchInt];
		}
	}
}

//===========================================================================

- (void)mouseMove:(CGPoint)location touchID:(int)ID
{
	GameApp* gameApp = GameApp::Get();
	if ( gameApp != NULL )
	{
        float scale = 1.0f;
        if ( Database::Get()->GetIsRetinaDisplay() )
        {
            scale = 2.0f;
        }
        
		float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
		float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
		float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
		float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );
        float x = ( appScreenWidth / touchScreenWidth ) * location.x * scale;
		float y = ( appScreenHeight / touchScreenHeight ) * location.y * scale;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
        
		gameApp->GetInputManager()->GetInputManagerPlat()->TouchMove( ID, xInt, yInt );
	}
}

//===========================================================================

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	GameApp* gameApp = GameApp::Get();
	if ( gameApp != NULL )
	{
		int numTouches = (int)[touches count];
		gameApp->GetInputManager()->GetInputManagerPlat()->SetNumTouches( numTouches );
        
        for ( int i = 0; i < numTouches; ++i )
        {
            UITouch *touch = [[touches allObjects] objectAtIndex:i];
        	CGPoint point = [touch locationInView:(EAGLView *)self.view];
            int touchInt = *((int*)&touch);
			[self mouseMove:point touchID:touchInt];
		}
	}
}

//===========================================================================

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return ( UIInterfaceOrientationLandscapeRight == interfaceOrientation );
}

//===========================================================================

+ (void)setThreadContext
{
    [EAGLContext setCurrentContext:gContextThread];
}

//===========================================================================

-(void)CreateAdBannerView
{
    int width = Database::Get()->GetAppScreenWidth();
    if(Database::Get()->GetIsRetinaDisplay())
    {
        width /= 2;
    }
    mAdBannerView = [[ADBannerView alloc] initWithFrame:CGRectZero];
    int adwidth = mAdBannerView.frame.size.width;
    mAdBannerView.frame = CGRectMake(
                              (width-adwidth)/2,
                              mAdBannerView.frame.origin.y - mAdBannerView.frame.size.height,
                              mAdBannerView.frame.size.width,
                              mAdBannerView.frame.size.height
                              );
    mAdBannerView.delegate = self;
    [self.view addSubview:mAdBannerView];
}

//===========================================================================

-(void)RemoveAdBannerView
{
    [mAdBannerView removeFromSuperview];
    mAdBannerView.delegate = nil;
    [mAdBannerView release];
}

//===========================================================================

-(void)bannerViewDidLoadAd:(ADBannerView *)banner
{
    if (!mIsBannerVisible)
    {
        [UIView beginAnimations:@"animateAdBannerOn" context:NULL];
        // Assumes the banner view is just off the bottom of the screen.
        banner.frame = CGRectOffset(banner.frame, 0, banner.frame.size.height);
        [UIView commitAnimations];
        mIsBannerVisible = YES;
    }
}

//===========================================================================


-(BOOL)bannerViewActionShouldBegin:(ADBannerView *)banner willLeaveApplication:(BOOL)willLeave
{
    if ( GameApp::Get() )
    {
        GameApp::Get()->SetHide();
    }
    
    return YES;
}

//===========================================================================

-(void)bannerViewActionDidFinish:(ADBannerView *)banner
{
    GameApp* game = GameApp::Get();
    if ( game )
    {
        GameApp::Get()->SetShow();
    }
}

//===========================================================================

-(void)bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error
{
    if (mIsBannerVisible)
    {
        [UIView beginAnimations:@"animateAdBannerOff" context:NULL];
        // Assumes the banner view is placed at the bottom of the screen.
        banner.frame = CGRectOffset(banner.frame, 0, -banner.frame.size.height);
        [UIView commitAnimations];
        mIsBannerVisible = NO;
    }
}

@end