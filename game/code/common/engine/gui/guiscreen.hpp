#ifndef GUI_SCREEN_HPP
#define GUI_SCREEN_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guicontrol.hpp"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class GuiStatic;
class GuiRadioButton;
class GuiCheckBox;
class GuiSlider;
class GuiButton;
class GuiInput;
class GuiSprite;

namespace tinyxml2
{
    class XMLNode;
    class XMLElement;
    class XMLDocument;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum EventType
{
	EVENT_BUTTON_CLICKED,
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GuiScreen
{
public:
	GuiScreen( int screenType );
	virtual ~GuiScreen();

	void Load( const char* filename );
	void LoadTool( const char* filename );
	void LoadCommon( const char* filename );
	void Save( const char* filename );

	virtual void Initialize();	
	virtual bool UpdateInput( GuiInput* input );
	virtual void Render( Math::Matrix44 &orthoMatrix, bool drawBoundingBox = false );	
	virtual void Update( float elapsedTime );
	void PlayAnimation();
	void LoopAnimation();
	virtual void OnEvent( EventType event, int controlID, GuiControl* control );	
		
	void CopyControl( GuiControl* control );
	void AddControl( GuiControl* control );	

	void AddStatic(
		const char* strText, 
		int x, 
		int y, 
		int width, 
		int height, 	
		GuiStatic** created = NULL
		);

	void AddButton(
		const char* strText, 
		int x, 
		int y, 
		int width, 
		int height, 
		GuiButton** created = NULL 
		);

	void AddCheckBox(
		const char* strText, 
		int x, 
		int y, 
		int width, 
		int height, 
		bool checked,
		GuiCheckBox** created = NULL
		);

	void AddRadioButton(
		int buttonGroup, 
		const char* strText, 
		int x, 
		int y, 
		int width, 
		int height,
		bool checked, 	
		GuiRadioButton** created = NULL
		);

	void AddSlider(
		int x, 
		int y, 
		int width, 
		int height, 
		int min, 
		int max, 
		int value,
		GuiSlider** created = NULL
		);

	void AddSprite(
		int x, 
		int y, 
		int width, 
		int height, 
		GuiSprite** created 
		);

	GuiStatic* GetStaticByName( const char* name );
	GuiButton* GetButtonByName( const char* name );
	GuiSlider* GetSliderByName( const char* name );
	GuiSprite* GetSpriteByName( const char* name );
	GuiControl* GetControlByName( const char* name, ControlType controlType );

	GuiStatic* GetStatic( int id );
	GuiButton* GetButton( int id );
	GuiSlider* GetSlider( int id );
	GuiSprite* GetSprite( int id );
	GuiControl* GetControlByID( int id );
	GuiControl* GetControl( int id, ControlType controlType );
	int GetNumControls();
	GuiControl* GetControlByIndex( int index );
	void MoveControlBack( int id );
	void MoveControlFront( int id );

	void DeleteControl( int id );

	void RequestFocus( GuiControl* control );

	int GetScreenType()
	{
		return mScreenType;
	}

	int GetWidth()
	{
		return mWidth;
	}

	int GetHeight()
	{
		return mHeight;
	}

	Math::Vector4 GetBackgroundColor()
	{
		return mBackgroundColor;
	}

	void SetBackgroundColor( Math::Vector4 color )
	{
		mBackgroundColor = color;
	}

private:

	void OnMouseMove( int x, int y );
	GuiControl* GetControlAtPoint( int x, int y );
		
	void LoadGuiControl( tinyxml2::XMLNode* node, GuiControl* guiControl );
    void LoadGuiCurrentData( tinyxml2::XMLNode* node, GuiControl* guiControl );
	void LoadGuiAnimation( tinyxml2::XMLNode* node, GuiControl* guiControl );
	void LoadGuiFrame( tinyxml2::XMLNode* node, GuiControl* guiControl, int frame );
	void LoadGuiStatic( tinyxml2::XMLNode* node, GuiStatic* guiStatic );
	void LoadGuiButton( tinyxml2::XMLNode* node, GuiButton* guiButton );
	void LoadGuiSlider( tinyxml2::XMLNode* node, GuiSlider* guiSlider );
	void LoadGuiSprite( tinyxml2::XMLNode* node, GuiSprite* guiSprite );

    void SaveControl( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiControl* guiControl );
	void SaveStatic( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiStatic* guiStatic );
	void SaveButton( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiButton* guiButton );
	void SaveSlider( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiSlider* guiSlider );
	void SaveSprite( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiSprite* guiSprite );

	void CheckUniqueID( int id );

	int mScreenType;
	bool mVisible;
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	GuiControl* mControlMouseOver;
	GuiControl* mControlFocus;
	static GuiControl* mControlPressed;
	std::vector< GuiControl* > mControlList;
	Math::Vector4 mBackgroundColor;

	int mUniqueID;
};

#endif
