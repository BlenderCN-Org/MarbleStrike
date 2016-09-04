//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#import "buildiphoneAppDelegate.h"
#import "GraphicsViewController.h"

//////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////

@implementation buildiphoneAppDelegate

@synthesize mWindow;
@synthesize mViewController;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

- (void)applicationWillResignActive:(UIApplication *)application
{
    [mViewController stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [mViewController startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [mViewController stopAnimation];
	[[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Handle any background procedures not related to animation here.
    [mViewController stopAnimation];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Handle any foreground procedures not related to animation here.
    [mViewController startAnimation];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application
{	
}

- (void)dealloc
{
    [mViewController release];
    [mWindow release];
	[mOpenFeintGameDelegate release];
    
    [super dealloc];
}

@end
