//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <iAd/ADBannerView.h>


//////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////

class TempClass;

@interface GraphicsViewController : UIViewController<UIAccelerometerDelegate, ADBannerViewDelegate>
{
    EAGLContext* mContextMain;
    EAGLSharegroup* mSharedGroup;
    
    BOOL mAnimating;
    BOOL mDisplayLinkSupported;
    NSInteger mAnimationFrameInterval;
   
	//
	// Use of the CADisplayLink class is the preferred method for controlling your animation timing.
	// CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
	// The NSTimer object is used only as fallback when running on a pre-3.1 device where CADisplayLink isn't available.
	//
    id mDisplayLink;
    NSTimer* mAnimationTimer;
	
	float mStartDist;
	bool mStartZoom;
    bool mIsBannerVisible;
    
    ADBannerView* mAdBannerView;
    TempClass* mTempClass;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;
@property (nonatomic, retain) EAGLContext *mContextMain;
@property (nonatomic, retain) EAGLSharegroup *mSharedGroup;
@property (nonatomic, retain) ADBannerView *mAdBannerView;

- (void)startAnimation;
- (void)stopAnimation;

- (void)handleSwipeUp:(UISwipeGestureRecognizer *)recognizer;
- (void)mouseDown:(CGPoint)location touchID:(int)ID;
- (void)mouseMove:(CGPoint)location touchID:(int)ID;
- (void)mouseUp:(CGPoint)location touchID:(int)ID;

+ (void)setThreadContext;

-(void)CreateAdBannerView;

class TempClass
{
public:
    GraphicsViewController* mViewController;
};

@end