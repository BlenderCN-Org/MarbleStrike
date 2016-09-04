///////////////////////////////////////////////////// 
// INCLUDES 
///////////////////////////////////////////////////// 

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/gui/guicheckbox.hpp"
#include "common/engine/gui/guiinput.hpp"
#include "common/engine/gui/guiradiobutton.hpp"
#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/gui/guislider.hpp"
#include "common/engine/gui/guistatic.hpp"
#include "common/engine/gui/guisprite.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/utilities.hpp"

///////////////////////////////////////////////////// 
// GLOBALS
///////////////////////////////////////////////////// 

GuiControl* GuiScreen::mControlPressed = NULL;

////////////////////////////////////////////////////// 
// CLASS METHODS 
////////////////////////////////////////////////////// 

GuiScreen::GuiScreen( int screenType ) 
{ 
	mScreenType = screenType;
	mVisible = true;
	mX = 0;
	mY = 0;
	mWidth = 0;
	mHeight = 0;
	mControlMouseOver = NULL;
	mControlFocus = NULL;
	mUniqueID = 0;
} 

//=========================================================================== 

GuiScreen::~GuiScreen() 
{ 
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		DELETE_PTR( mControlList[i] );
	}
} 

//===========================================================================

void GuiScreen::Load( const char* filename )
{
	char extension[16];
	System::Path::GetExt( filename, extension, 16 );
	char filenameNoExt[32];
	System::Path::GetFilenameNoExt( filename, filenameNoExt, 32 );
	char filepath[128];
	System::Path::GetFilePath( filename, filepath, 128 );

	char newFilename[256];
	System::StringCopy( newFilename, 256, filename );
	
	System::FixedString<256> loadPath = System::Path::SystemSlash( newFilename ); 
	char filenameFullPath[256]; 
	const char* resourcePath = Database::Get()->GetResourcePath(); 
	System::StringCopy( filenameFullPath, 256, resourcePath ); 
	System::StringConcat( filenameFullPath, 256, loadPath ); 

	LoadCommon( filenameFullPath );
}

//===========================================================================

void GuiScreen::LoadTool( const char* filename )
{
	LoadCommon( filename );
}

//===========================================================================

void GuiScreen::LoadCommon( const char* filename )
{	
	FILE* file = System::OpenFile( filename, "rb" );
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError loadOkay = doc.LoadFile( file ); 
	fclose( file );
	file = NULL;
    std::string errorMsg;
    errorMsg = "Cannot open file: ";
    errorMsg += filename;
    Assert( loadOkay == tinyxml2::XML_SUCCESS, errorMsg.c_str() ); 
    if ( loadOkay == tinyxml2::XML_SUCCESS ) 
    { 
        tinyxml2::XMLNode* node = &doc; 

        for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() ) 
        { 

            tinyxml2::XMLElement* element = item->ToElement(); 
            if ( element )
            { 
                const char* elementValue = element->Value();
                if ( strcmp( elementValue, "GuiStatic" ) == 0 ) 
                { 
                    GuiStatic* guiStatic = NULL;
                    AddStatic( "", 0, 0, 0, 0, &guiStatic );
                    LoadGuiStatic( item->FirstChild(), guiStatic );
                } 
                else if ( strcmp( elementValue, "GuiButton" ) == 0 ) 
                { 
                    GuiButton* guiButton = NULL;
                    AddButton( "", 0, 0, 0, 0, &guiButton );
                    LoadGuiButton( item->FirstChild(), guiButton );
                } 
                else if ( strcmp( elementValue, "GuiSlider" ) == 0 ) 
                { 
                    GuiSlider* guiSlider = NULL;

                    int min = element->IntAttribute( "min" );
                    int max = element->IntAttribute( "max" );
                    int value = element->IntAttribute( "value" );
                    AddSlider( 0, 0, 0, 0, min, max, value, &guiSlider );
                    LoadGuiSlider( item->FirstChild(), guiSlider );
                } 
				else if ( strcmp( elementValue, "GuiSprite" ) == 0 )
				{
					GuiSprite* guiSprite = NULL;
					AddSprite( 0, 0, 0, 0, &guiSprite );
					LoadGuiSprite( item->FirstChild(), guiSprite );

				}
                else if ( strcmp( elementValue, "ScreenProperties" ) == 0 ) 
                { 
                    mWidth = element->IntAttribute( "width" );                    
                    mHeight = element->IntAttribute( "height" );

                    double r = element->DoubleAttribute( "r" );
                    double g = element->DoubleAttribute( "g" );
                    double b = element->DoubleAttribute( "b" );

                    mBackgroundColor.Set( static_cast<float>( r ) , static_cast<float>( g ), static_cast<float>( b ), 1.0 );
                }
            } 
        } 
    }
}

//===========================================================================

void GuiScreen::Save( const char* filename )
{
	tinyxml2::XMLDocument doc; 

	tinyxml2::XMLElement* screenElement = doc.NewElement( "ScreenProperties" ); 
	int screenWidth = Database::Get()->GetAppScreenWidth(); 
	int screenHeight = Database::Get()->GetAppScreenHeight(); 
	screenElement->SetAttribute( "width", screenWidth ); 
	screenElement->SetAttribute( "height", screenHeight ); 
	screenElement->SetAttribute( "r", mBackgroundColor[0] );
	screenElement->SetAttribute( "g", mBackgroundColor[1] );
	screenElement->SetAttribute( "b", mBackgroundColor[2] );
	doc.LinkEndChild( screenElement ); 

	std::vector< GuiControl* >::iterator iter = mControlList.begin(); 
	while ( iter != mControlList.end() ) 
	{ 
		GuiControl* control = *iter;

		switch ( control->GetType() )
		{
		case CONTROL_TYPE_STATIC:
			{
				GuiStatic* guiStatic = reinterpret_cast<GuiStatic*>( control );
				tinyxml2::XMLElement* guiStaticElement = doc.NewElement( "GuiStatic" );
				guiStaticElement->SetAttribute( "id", guiStatic->GetID() );
				SaveStatic( &doc, guiStaticElement, guiStatic );
				doc.LinkEndChild( guiStaticElement );
			}
			break;
		case CONTROL_TYPE_BUTTON:
			{
				GuiButton* guiButton = reinterpret_cast<GuiButton*>( control );
				tinyxml2::XMLElement* guiButtonElement = doc.NewElement( "GuiButton" );
				guiButtonElement->SetAttribute( "id", guiButton->GetID() );
				SaveButton( &doc, guiButtonElement, guiButton );
				doc.LinkEndChild( guiButtonElement );
			}
			break;
		case CONTROL_TYPE_SLIDER:
			{
				GuiSlider* guiSlider = reinterpret_cast<GuiSlider*>( control );				
				tinyxml2::XMLElement* guiSliderElement = doc.NewElement( "GuiSlider" );
				guiSliderElement->SetAttribute( "id", guiSlider->GetID() );
				guiSliderElement->SetAttribute( "min", guiSlider->GetMin() );
				guiSliderElement->SetAttribute( "max", guiSlider->GetMax() );
				guiSliderElement->SetAttribute( "value", guiSlider->GetValue() );
				SaveSlider( &doc, guiSliderElement, guiSlider );
				doc.LinkEndChild( guiSliderElement );
			}
			break;
		case CONTROL_TYPE_SPRITE:
			{
				GuiSprite* guiSprite = reinterpret_cast<GuiSprite*>( control );				
				tinyxml2::XMLElement* guiSpriteElement = doc.NewElement( "GuiSprite" );
				guiSpriteElement->SetAttribute( "id", guiSprite->GetID() );
				SaveSprite( &doc, guiSpriteElement, guiSprite );
				doc.LinkEndChild( guiSpriteElement );
			}
			break;
         default:
            break;
		}
		
		iter++; 
	} 

	doc.LinkEndChild( doc.NewDeclaration() ); 

	doc.SaveFile( filename ); 
}

//===========================================================================

void GuiScreen::Initialize()
{
}

//===========================================================================

bool GuiScreen::UpdateInput( GuiInput* input )
{
	Assert( input != NULL, "" );

    if ( mVisible == false )
    {
        return false;
    }

    if ( 
        mControlFocus &&
        mControlFocus->GetParent() == this &&
        mControlFocus->GetEnabled() 
        )
    {
        if ( mControlFocus->HandleMouse( input ) )
        {
            return true;
        }
    }

    std::vector<int> downResults = input->GetInput( GuiInput::INPUT_TYPE_ONDOWN );
    if ( downResults.size() > 0 )
    {
        int ID = downResults[0];
        int mouseX = input->GetInputX( ID ); 
        int mouseY = Database::Get()->GetAppScreenHeight() - input->GetInputY( ID ); 

        GuiControl* control = GetControlAtPoint( mouseX, mouseY );
        if ( control != NULL && control->GetEnabled() )
        {
            bool handled = control->HandleMouse( input );
            if ( handled )
            {
                return true;
            }
        }
        else
        {	
            if ( mControlFocus && mControlFocus->GetParent() == this )
            {
                mControlFocus->OnFocusOut();
                mControlFocus = NULL;
            }        
        }
    }
    else
    {
        size_t numControls = mControlList.size();
        for ( int i = 0; i < numControls; ++i )
        {
            GuiControl* guiControl = mControlList[i];
            if ( guiControl->GetType() == CONTROL_TYPE_BUTTON )
            {
                GuiButton* button = reinterpret_cast<GuiButton*>( guiControl );
                button->SetDirty( false );
            }
        }
    }

	return false;
}

//===========================================================================

void GuiScreen::Render( Math::Matrix44 &orthoMatrix, bool drawBoundingBox )
{
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		mControlList[i]->Render( orthoMatrix, drawBoundingBox );
	}
}

//===========================================================================

void GuiScreen::Update( float elapsedTime )
{
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		mControlList[i]->Update( elapsedTime );
	}
}

//===========================================================================

void GuiScreen::PlayAnimation()
{
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		mControlList[i]->PlayAnimation();
	}
}

//===========================================================================

void GuiScreen::LoopAnimation()
{
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		mControlList[i]->LoopAnimation();
	}
}

//===========================================================================

void GuiScreen::OnEvent( EventType /*event*/, int /*controlID*/, GuiControl* /*control*/ )
{	
}

//===========================================================================

void GuiScreen::CopyControl( GuiControl* control )
{
	GuiControl* newControl = NULL;
	switch ( control->GetType() )
	{
	case CONTROL_TYPE_STATIC:
		newControl = NEW_PTR("Static Control") GuiStatic( control->GetParent() );
		break;
	case CONTROL_TYPE_BUTTON:
		newControl = NEW_PTR("Button Control") GuiButton( control->GetParent() );
		break;
	case CONTROL_TYPE_SLIDER:
		newControl = NEW_PTR("Slider Control") GuiSlider( control->GetParent() );
		break;
	case CONTROL_TYPE_SPRITE:
		newControl = NEW_PTR("Sprite control") GuiSprite( control->GetParent() );
		break;
    default:
        break;
	}
	
    if ( newControl != NULL )
    {
        newControl->Copy( control );

        mUniqueID++;
        newControl->SetID( mUniqueID );

        AddControl( newControl );
    }
}

//===========================================================================

void GuiScreen::AddControl( GuiControl* control )
{
	mControlList.push_back( control );	
}

//===========================================================================

void GuiScreen::AddStatic(
	const char* strText, 
	int x, 
	int y, 
	int width, 
	int height, 	
	GuiStatic** createdStatic )
{

	GuiStatic* staticControl = NEW_PTR( "Gui Static" ) GuiStatic( this );

	if( createdStatic != NULL )
	{
		*createdStatic = staticControl;
	}

	AddControl( staticControl );
		
	mUniqueID++;
	staticControl->SetID( mUniqueID );
	staticControl->SetText( strText );
	staticControl->SetPosition( x, y );
	staticControl->SetSize( width, height );
}

//===========================================================================

void GuiScreen::AddButton(
	const char* strText, 
	int x, 
	int y, 
	int width, 
	int height, 
	GuiButton** created 
	)
{
	GuiButton* button = NEW_PTR( "Gui Button" ) GuiButton( this );
	Assert( button != NULL, "" );

	if ( created != NULL )
	{
		*created = button;
	}

	AddControl( button );

	mUniqueID++;
	button->SetID( mUniqueID );
	button->SetText( strText );
	button->SetPosition( x, y );
	button->SetSize( width, height );
}

//===========================================================================

void GuiScreen::AddCheckBox(
	const char* strText, 
	int x, 
	int y, 
	int width, 
	int height, 
	bool checked,
	GuiCheckBox** created
	)
{
	GuiCheckBox* checkBox = NEW_PTR( "Gui Checkbox" ) GuiCheckBox( this );

	if ( created != NULL )
	{
		*created = checkBox;
	}

	AddControl( checkBox );
	
	mUniqueID++;
	checkBox->SetID( mUniqueID );
	checkBox->SetText( strText );
	checkBox->SetPosition( x, y );
	checkBox->SetSize( width, height );	
	checkBox->SetChecked( checked );
}

//===========================================================================

void GuiScreen::AddRadioButton(
	int buttonGroup, 
	const char* strText, 
	int x, 
	int y, 
	int width, 
	int height,
	bool checked, 	
	GuiRadioButton** created
	)
{
	GuiRadioButton* radioButton = NEW_PTR( "Gui Radio Button" ) GuiRadioButton( this );

	if ( created != NULL )
	{
		*created = radioButton;
	}

	AddControl( radioButton );
		
	mUniqueID++;
	radioButton->SetID( mUniqueID );
	radioButton->SetText( strText );
	radioButton->SetButtonGroup( buttonGroup );
	radioButton->SetPosition( x, y );
	radioButton->SetSize( width, height );	
	radioButton->SetChecked( checked );	
}

//===========================================================================

void GuiScreen::AddSlider(
	int x, 
	int y, 
	int width, 
	int height, 
	int min, 
	int max, 
	int value,
	GuiSlider** created
	)
{
	GuiSlider* slider = NEW_PTR( "Gui Slider") GuiSlider( this );

	if ( created != NULL )
	{
		*created = slider;
	}
	
	AddControl( slider );
	
	mUniqueID++;
	slider->SetID( mUniqueID );
	slider->SetPosition( x, y );
	slider->SetSize( width, height );	
	slider->SetRange( min, max );
	slider->SetValue( value );
}

//===========================================================================

void GuiScreen::AddSprite(
	int x, 
	int y, 
	int width, 
	int height, 
	GuiSprite** created 
	)
{
	GuiSprite* sprite = NEW_PTR( "Gui sprite" ) GuiSprite( this );
	Assert( sprite != NULL, "" );

	if ( created != NULL )
	{
		*created = sprite;
	}

	AddControl( sprite );

	mUniqueID++;
	sprite->SetID( mUniqueID );
	sprite->SetPosition( x, y );
	sprite->SetSize( width, height );
}

//===========================================================================

GuiStatic* GuiScreen::GetStaticByName( const char* name )
{
	GuiControl* control = GetControlByName( name, CONTROL_TYPE_STATIC );
	GuiStatic* staticControl = reinterpret_cast<GuiStatic*>( control );
	return staticControl;
}

//===========================================================================

GuiButton* GuiScreen::GetButtonByName( const char* name )
{
	GuiControl* control = GetControlByName( name, CONTROL_TYPE_BUTTON );
	GuiButton* button = reinterpret_cast<GuiButton*>( control );
	return button;
}

//===========================================================================

GuiSlider* GuiScreen::GetSliderByName( const char* name )
{
	GuiControl* control = GetControlByName( name, CONTROL_TYPE_SLIDER );
	GuiSlider* slider = reinterpret_cast<GuiSlider*>( control );
	return slider;
}

//===========================================================================

GuiSprite* GuiScreen::GetSpriteByName( const char* name )
{
	GuiControl* control = GetControlByName( name, CONTROL_TYPE_SPRITE );
	GuiSprite* sprite = reinterpret_cast<GuiSprite*>( control );
	return sprite;
}

//===========================================================================

GuiControl* GuiScreen::GetControlByName( const char* name, ControlType controlType )
{
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		GuiControl* control = mControlList[i];
		if ( control->GetType() == controlType )
		{
			if ( strcmp( control->GetName(), name ) == 0 )
			{
				return control;
			}
		}
	}

	Assert( false, "" );

	return NULL;
}

//===========================================================================

GuiStatic* GuiScreen::GetStatic( int id )
{
	GuiControl* control = GetControl( id, CONTROL_TYPE_STATIC );
	GuiStatic* staticControl = reinterpret_cast<GuiStatic*>( control );
	return staticControl;
}

//===========================================================================

GuiButton* GuiScreen::GetButton( int id )
{
	GuiControl* control = GetControl( id, CONTROL_TYPE_BUTTON );
	GuiButton* button = reinterpret_cast<GuiButton*>( control );
	return button;
}

//===========================================================================

GuiSlider* GuiScreen::GetSlider( int id )
{
	GuiControl* control = GetControl( id, CONTROL_TYPE_SLIDER );
	GuiSlider* slider = reinterpret_cast<GuiSlider*>( control );
	return slider;
}

//===========================================================================

GuiSprite* GuiScreen::GetSprite( int id )
{
	GuiControl* control = GetControl( id, CONTROL_TYPE_SPRITE );
	GuiSprite* sprite = reinterpret_cast<GuiSprite*>( control );
	return sprite;
}

//===========================================================================

GuiControl* GuiScreen::GetControlByID( int id )
{
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		GuiControl* control = mControlList[i];
		if ( control->GetID() == id )
		{
			return control;
		}
	}

	return NULL;
}

//===========================================================================

GuiControl* GuiScreen::GetControl( int id, ControlType controlType )
{
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		GuiControl* control = mControlList[i];
		if ( control->GetType() == controlType )
		{
			if ( control->GetID() == id )
			{
				return control;
			}
		}
	}

	return NULL;
}

//===========================================================================

int GuiScreen::GetNumControls()
{
	int numControls = static_cast<int>( mControlList.size() );
	return numControls;
}

//===========================================================================

GuiControl* GuiScreen::GetControlByIndex( int index )
{
	return mControlList[index];
}

//===========================================================================

void GuiScreen::MoveControlBack( int id )
{
	GuiControl* previousControl = NULL;	
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		GuiControl* currentControl = mControlList[i];
		if ( currentControl->GetID() == id && i > 0 )
		{
			mControlList[i - 1] = currentControl;
			mControlList[i] = previousControl;
		}
		previousControl = currentControl;
	}
}

//===========================================================================

void GuiScreen::MoveControlFront( int id )
{
	int numControls = static_cast<int>( mControlList.size() );
	for ( int i = 0; i < numControls; ++i )
	{
		GuiControl* currentControl = mControlList[i];
		if ( currentControl->GetID() == id && i < ( numControls - 1 ) )
		{
			mControlList[i] = mControlList[i + 1];
			mControlList[i + 1] = currentControl;			
            break;
		}
	}
}

//===========================================================================

void GuiScreen::DeleteControl( int id )
{	
	std::vector< GuiControl* >::iterator iter = mControlList.begin();
	
	while ( iter != mControlList.end() )
	{
		GuiControl* guiControl = *iter;
		if ( guiControl->GetID() == id )
		{
			DELETE_PTR( guiControl );
			mControlList.erase( iter );
			return;
		}

		iter++;
	}
}

//===========================================================================

void GuiScreen::RequestFocus( GuiControl* control )
{
	if ( mControlFocus == control )
	{
		return;
	}

	if ( control->CanHaveFocus() == false )
	{
		return;
	}

	if ( mControlFocus )
	{
		mControlFocus->OnFocusOut();
	}

	control->OnFocusIn();
	mControlFocus = control;
}

//===========================================================================

void GuiScreen::OnMouseMove( int x, int y )
{	
	GuiControl* control = GetControlAtPoint( x, y );
		
	if ( control == mControlMouseOver )
	{
		return;
	}
		
	if ( mControlMouseOver )
	{
		mControlMouseOver->OnMouseLeave();
	}

	mControlMouseOver = control;
	if ( control != NULL )
	{
		mControlMouseOver->OnMouseEnter();
	}
}

//===========================================================================

GuiControl* GuiScreen::GetControlAtPoint( int x, int y )
{
	int numControls = static_cast<int>( mControlList.size() );
	for( int i = numControls - 1; i >= 0; --i )
	{
		GuiControl* control = mControlList[i];
		
		if ( 
			control->ContainsPoint( x, y ) && 
			control->GetEnabled() && 
			control->GetVisible() 
			)
		{
			return control;
		}
	}

	return NULL;
}

//===========================================================================

void GuiScreen::LoadGuiControl( tinyxml2::XMLNode* node, GuiControl* guiControl )
{
    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() ) 
    { 
        tinyxml2::XMLElement* element = item->ToElement(); 
        if ( element )
        { 
            const char* elementValue = element->Value(); 
            if ( strcmp( elementValue, "Name" ) == 0 ) 
            { 
                const char* name = element->Attribute( "value" );
                guiControl->SetName( name );
            } 
            else if ( strcmp( elementValue, "CurrentData" ) == 0 ) 
            {                    
                LoadGuiCurrentData( item->FirstChild(), guiControl );
            }
            else if ( strcmp( elementValue, "Animation" ) == 0 ) 
            { 
                double fps = element->DoubleAttribute( "fps" );
                guiControl->AddAnimationSequence( element->Attribute( "Name" ) );
                guiControl->SetFramesPerSecond( static_cast<float>( fps ) );
                guiControl->SetCurrentAnimationSequence( guiControl->GetNumAnimationSequences() - 1 );
                guiControl->ClearKeyFrames();
                LoadGuiAnimation( item->FirstChild(), guiControl );
            }				
        }
    } 
}

//===========================================================================

void GuiScreen::LoadGuiCurrentData( tinyxml2::XMLNode* node, GuiControl* guiControl )
{
    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() ) 
    { 
        tinyxml2::XMLElement* element = item->ToElement(); 
        const char* elementValue = element->Value(); 
        if ( strcmp( elementValue, "Size" ) == 0 ) 
        { 
            int width = element->IntAttribute( "width" );
            int height = element->IntAttribute( "height" );

            guiControl->SetSize( width, height );
        }
        else if ( strcmp( elementValue, "Position" ) == 0 ) 
        { 
            int x = element->IntAttribute( "x" );
            int y = element->IntAttribute( "y" );

            guiControl->SetPosition( x, y );
        }
        else if ( strcmp( elementValue, "Alpha" ) == 0 ) 
        {
            double value = element->DoubleAttribute( "value" );

            guiControl->SetAlpha( static_cast<float>( value ) );
        }
    }
}

//===========================================================================

void GuiScreen::LoadGuiAnimation( tinyxml2::XMLNode* node, GuiControl* guiControl )
{
    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() ) 
    { 
        tinyxml2::XMLElement* element = item->ToElement(); 
        if ( element )
        { 
            const char* elementValue = element->Value(); 
            if ( strcmp( elementValue, "KeyFrame" ) == 0 ) 
            { 
                int frame = element->IntAttribute( "frame" );
                LoadGuiFrame( item->FirstChild(), guiControl, frame );
            }
        } 
    } 

    guiControl->GoToKeyFrame( 0 );
}

//===========================================================================

void GuiScreen::LoadGuiFrame( tinyxml2::XMLNode* node, GuiControl* guiControl, int frame )
{
	ControlKeyFrameData keyFrame;

	for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() ) 
    { 
        tinyxml2::XMLElement* element = item->ToElement(); 
        if ( element )
        { 
            const char* elementValue = element->Value(); 
            if ( strcmp( elementValue, "Size" ) == 0 ) 
            { 
                int width = element->IntAttribute( "width" );
                int height = element->IntAttribute( "height" );

                guiControl->SetSize( width, height );

                keyFrame.mSize.Set( width, height );
            }
            else if ( strcmp( elementValue, "Position" ) == 0 ) 
            { 
                int x = element->IntAttribute( "x" );
                int y = element->IntAttribute( "y" );

                keyFrame.mPosition.Set( x, y );
            }
            else if ( strcmp( elementValue, "Alpha" ) == 0 ) 
            {
                double value = element->DoubleAttribute( "value" );

                keyFrame.mAlpha = static_cast<float>( value );
            }
        }
    } 

    guiControl->InsertKeyFrame( keyFrame, frame );
}

//===========================================================================

void GuiScreen::LoadGuiStatic( tinyxml2::XMLNode* node, GuiStatic* guiStatic )
{
    LoadGuiControl( node, guiStatic );

    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() ) 
    { 
        tinyxml2::XMLElement* element = item->ToElement(); 
        if ( element )
        { 
            const char* elementValue = element->Value(); 
            if ( strcmp( elementValue, "Font" ) == 0 )
            {
                const char* fontName = element->Attribute( "name" );
                int fontIndex = GameApp::Get()->GetFontManager()->AddFont( fontName );
                guiStatic->SetFontIndex( fontIndex );
            }
            else if ( strcmp( elementValue, "Text" ) == 0 )
            {
                const char* text = element->Attribute( "value" );
                guiStatic->SetText( text );

                const char* horizontalAlignmentText = element->Attribute( "horizontalAlignment" );
                TextHorizontalAlignment horizontalAlignment = TEXT_HORIZONTAL_ALIGN_LEFT;
                if ( strcmp( horizontalAlignmentText, "left" ) == 0 )
                {
                    horizontalAlignment = TEXT_HORIZONTAL_ALIGN_LEFT;
                }
                else if ( strcmp( horizontalAlignmentText, "right" ) == 0 )
                {
                    horizontalAlignment = TEXT_HORIZONTAL_ALIGN_RIGHT;
                }
                else if ( strcmp( horizontalAlignmentText, "centre" ) == 0 )
                {
                    horizontalAlignment = TEXT_HORIZONTAL_ALIGN_CENTRE;
                }
                guiStatic->SetTextHorizontalAlignment( horizontalAlignment );

                const char* verticalAlignmentText = element->Attribute( "verticalAlignment" );
                TextVerticalAlignment verticalAlignment = TEXT_VERTICAL_ALIGN_TOP;
                if ( strcmp( verticalAlignmentText, "top" ) == 0 )
                {
                    verticalAlignment = TEXT_VERTICAL_ALIGN_TOP;
                }
                else if ( strcmp( verticalAlignmentText, "bottom" ) == 0 )
                {
                    verticalAlignment = TEXT_VERTICAL_ALIGN_BOTTOM;
                }
                else if ( strcmp( verticalAlignmentText, "centre" ) == 0 )
                {
                    verticalAlignment = TEXT_VERTICAL_ALIGN_CENTRE;
                }
                guiStatic->SetTextVerticalAlignment( verticalAlignment );

                double fontSize = element->DoubleAttribute( "size" );
                guiStatic->SetFontSize( static_cast<float>( fontSize ) );

                Math::Vector4 color;
                int value = 0;
                value = element->IntAttribute( "r" );
                color[0] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "g" );
                color[1] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "b" );
                color[2] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "a" );
                color[3] = static_cast<float>( value ) / 255.0f;
                guiStatic->SetFontColor( color );
            }
            else if ( strcmp( elementValue, "UV" ) == 0 )
            {
                double value = 0;

                Math::Vector2 size;					
                value = element->DoubleAttribute( "width" );
                size[0] = static_cast<float>( value );
                value = element->DoubleAttribute( "height" );
                size[1] = static_cast<float>( value );
                guiStatic->SetUVSize( size );

                Math::Vector2 position;
                value = element->DoubleAttribute( "x" );
                position[0] = static_cast<float>( value );
                value = element->DoubleAttribute( "y" );
                position[1] = static_cast<float>( value );
                guiStatic->SetUVPosition( position );
            }
            else if ( strcmp( elementValue, "Texture" ) == 0 )
            {
                const char* textureName = element->Attribute( "name" );
                Texture* texture = NULL;
                if ( strcmp( textureName, "" ) != 0 )
                {
                    texture = GameApp::Get()->GetTextureManager()->GetTexture( textureName );
                }
                guiStatic->SetTexture( texture );
            }
            else if ( strcmp( elementValue, "Color" ) == 0 )
            {
                Math::Vector4 color;

                int value = 0;
                value = element->IntAttribute( "r" );
                color[0] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "g" );
                color[1] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "b" );
                color[2] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "a" );
                color[3] = static_cast<float>( value ) / 255.0f;

                guiStatic->SetColor( color );
            }
        } 
    } 
}

//===========================================================================

void GuiScreen::LoadGuiButton( tinyxml2::XMLNode* node, GuiButton* guiButton )
{
    LoadGuiStatic( node, guiButton );

    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() ) 
    { 
        tinyxml2::XMLElement* element = item->ToElement(); 
        if ( element )
        {                 
            const char* elementValue = element->Value();
            if ( strcmp( elementValue, "OnPressUV" ) == 0 )
            {
                double value = 0;

                Math::Vector2 size;					
                value = element->DoubleAttribute( "width" );
                size[0] = static_cast<float>( value );
                value = element->DoubleAttribute( "height" );
                size[1] = static_cast<float>( value );
                guiButton->SetOnPressUVSize( size );

                Math::Vector2 position;
                value = element->DoubleAttribute( "x" );
                position[0] = static_cast<float>( value );
                value = element->DoubleAttribute( "y" );
                position[1] = static_cast<float>( value );
                guiButton->SetOnPressUVPosition( position );
            }
            else if ( strcmp( elementValue, "OnPressTexture" ) == 0 )
            {
                const char* textureName = element->Attribute( "name" );
                Texture* texture = NULL;
                if ( strcmp( textureName, "" ) != 0 )
                {
                    texture = GameApp::Get()->GetTextureManager()->GetTexture( textureName );
                }
                guiButton->SetOnPressTexture( texture );
            }
        } 
    } 
}

//===========================================================================

void GuiScreen::LoadGuiSlider( tinyxml2::XMLNode* node, GuiSlider* guiSlider )
{
    LoadGuiControl( node, guiSlider );

    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() ) 
    { 
        tinyxml2::XMLElement* element = item->ToElement(); 
        if ( element )
        { 				
            const char* elementValue = element->Value(); 
            if ( strcmp( elementValue, "BarUV" ) == 0 )
            {
                double value = 0;

                Math::Vector2 position;					
                value = element->DoubleAttribute( "x" );
                position[0] = static_cast<float>( value );
                value = element->DoubleAttribute( "y" );
                position[1] = static_cast<float>( value );
                guiSlider->SetUVPositionBar( position );

                Math::Vector2 size;
                value = element->DoubleAttribute( "width" );
                size[0] = static_cast<float>( value );
                value = element->DoubleAttribute( "height" );
                size[1] = static_cast<float>( value );
                guiSlider->SetUVSizeBar( size );
            }
            else if ( strcmp( elementValue, "BarTexture" ) == 0 )
            {
                const char* textureName = element->Attribute( "name" );
                Texture* texture = NULL;
                if ( strcmp( textureName, "" ) != 0 )
                {
                    texture = GameApp::Get()->GetTextureManager()->GetTexture( textureName );
                }
                guiSlider->SetTextureBar( texture );
            }
            else if ( strcmp( elementValue, "ButtonUV" ) == 0 )
            {
                double value = 0;

                Math::Vector2 position;					
                value = element->DoubleAttribute( "x" );
                position[0] = static_cast<float>( value );
                value = element->DoubleAttribute( "y" );
                position[1] = static_cast<float>( value );
                guiSlider->SetUVPositionButton( position );

                Math::Vector2 size;					
                value = element->DoubleAttribute( "width" );
                size[0] = static_cast<float>( value );
                value = element->DoubleAttribute( "height" );
                size[1] = static_cast<float>( value );
                guiSlider->SetUVSizeButton( size );
            }
            else if ( strcmp( elementValue, "ButtonTexture" ) == 0 )
            {
                const char* textureName = element->Attribute( "name" );
                Texture* texture = NULL;
                if ( strcmp( textureName, "" ) != 0 )
                {
                    texture = GameApp::Get()->GetTextureManager()->GetTexture( textureName );
                }
                guiSlider->SetTextureButton( texture );
            }
        } 
    } 
}

//===========================================================================

void GuiScreen::LoadGuiSprite( tinyxml2::XMLNode* node, GuiSprite* guiSprite )
{
    LoadGuiControl( node, guiSprite );

    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() ) 
    { 
        tinyxml2::XMLElement* element = item->ToElement(); 
        if ( element )
        { 
            const char* elementValue = element->Value(); 
            
            if ( strcmp( elementValue, "TextureFile" ) == 0 )
            {
                const char* textureFile = element->Attribute( "value" ); 
                guiSprite->LoadSprite( textureFile );
            }
			else if ( strcmp( elementValue, "fps" ) == 0 )
			{
				double value = 1.0f;
				value = element->DoubleAttribute( "value" );

				guiSprite->SetFramesPerSecond( static_cast<float>( value ) );
			}
            else if ( strcmp( elementValue, "Color" ) == 0 )
            {
                Math::Vector4 color;

                int value = 0;
                value = element->IntAttribute( "r" );
                color[0] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "g" );
                color[1] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "b" );
                color[2] = static_cast<float>( value ) / 255.0f;
                value = element->IntAttribute( "a" );
                color[3] = static_cast<float>( value ) / 255.0f;

                guiSprite->SetColor( color );
            }
        } 
    } 
}

//===========================================================================

void GuiScreen::SaveControl( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiControl* guiControl )
{
	tinyxml2::XMLElement* nameElement = NULL;
	nameElement = doc->NewElement( "Name" );
	nameElement->SetAttribute( "value",  guiControl->GetName() );
	element->LinkEndChild( nameElement );

    const ControlKeyFrameData* currentData = guiControl->GetControlKeyFrameData();
    tinyxml2::XMLElement* currentDataElement = NULL;
    currentDataElement = doc->NewElement( "CurrentData" );
    element->LinkEndChild( currentDataElement );
    tinyxml2::XMLElement* currentDataSizeElement = NULL;
    currentDataSizeElement = doc->NewElement( "Size" );	
    currentDataSizeElement->SetAttribute( "width", currentData->mSize[0] );
    currentDataSizeElement->SetAttribute( "height", currentData->mSize[1] );
    currentDataElement->LinkEndChild( currentDataSizeElement );

    tinyxml2::XMLElement* currentDataPositionElement = NULL;
    currentDataPositionElement = doc->NewElement( "Position" );	
    currentDataPositionElement->SetAttribute( "x", currentData->mPosition[0] );
    currentDataPositionElement->SetAttribute( "y", currentData->mPosition[1] );
    currentDataElement->LinkEndChild( currentDataPositionElement );

    tinyxml2::XMLElement* currentDataAlphaElement = NULL;
    currentDataAlphaElement = doc->NewElement( "Alpha" );	
    currentDataAlphaElement->SetAttribute( "value", currentData->mAlpha );		
    currentDataElement->LinkEndChild( currentDataAlphaElement );

    int numAnimationSequences = guiControl->GetNumAnimationSequences();
    for ( int i = 0; i < numAnimationSequences; ++i )
    {
        AnimationSequence* animationSequence = guiControl->GetAnimationSquence( i );

	    tinyxml2::XMLElement* animationElement = NULL;
	    animationElement = doc->NewElement( "Animation" );
	    int fps = 24;
	    animationElement->SetAttribute( "fps", fps );
        animationElement->SetAttribute( "Name", animationSequence->mAnimationSequenceName );
	    element->LinkEndChild( animationElement );

        int numKeyFrames = animationSequence->mAnimationController.GetNumKeyFrames();
	    for ( int i = 0; i < numKeyFrames; ++i )
	    {
		    AnimationController<ControlKeyFrameData>::KeyFrameData* keyFrameData = animationSequence->mAnimationController.GetAnimationKeyFrameData( i );

		    tinyxml2::XMLElement* keyFrameElement = NULL;
		    keyFrameElement = doc->NewElement( "KeyFrame" );
		
		    int frame = keyFrameData->mKeyFrame;
		    keyFrameElement->SetAttribute( "frame", frame );
		    animationElement->LinkEndChild( keyFrameElement );

		    tinyxml2::XMLElement* sizeElement = NULL;
		    sizeElement = doc->NewElement( "Size" );	
		    sizeElement->SetAttribute( "width", keyFrameData->mData.mSize[0] );
		    sizeElement->SetAttribute( "height", keyFrameData->mData.mSize[1] );
		    keyFrameElement->LinkEndChild( sizeElement );

		    tinyxml2::XMLElement* positionElement = NULL;
		    positionElement = doc->NewElement( "Position" );	
		    positionElement->SetAttribute( "x", keyFrameData->mData.mPosition[0] );
		    positionElement->SetAttribute( "y", keyFrameData->mData.mPosition[1] );
		    keyFrameElement->LinkEndChild( positionElement );

		    tinyxml2::XMLElement* alphaElement = NULL;
		    alphaElement = doc->NewElement( "Alpha" );	
		    alphaElement->SetAttribute( "value", keyFrameData->mData.mAlpha );		
		    keyFrameElement->LinkEndChild( alphaElement );
    	}
    }
}

//===========================================================================

void GuiScreen::SaveStatic( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiStatic* guiStatic )
{
	SaveControl( doc, element, guiStatic );	

	tinyxml2::XMLElement* fontElement = NULL;
	fontElement = doc->NewElement( "Font" );
	std::string fontName = "";
	if ( guiStatic->GetFontIndex() >= 0 )
	{
		Font* font = GameApp::Get()->GetFontManager()->GetFont( guiStatic->GetFontIndex() );
		fontName = font->GetFontName();		
	}
	fontElement->SetAttribute( "name", fontName.c_str() );
	element->LinkEndChild( fontElement );

	tinyxml2::XMLElement* textElement = NULL;
	textElement = doc->NewElement( "Text" );
	textElement->SetAttribute( "value", guiStatic->GetText() );
	std::string horizontalAlignment = "left";
	switch ( guiStatic->GetTextHorizontalAlignment() )
	{
	case TEXT_HORIZONTAL_ALIGN_LEFT:
		horizontalAlignment = "left";
		break;
	case TEXT_HORIZONTAL_ALIGN_RIGHT:
		horizontalAlignment = "right";
		break;
	case TEXT_HORIZONTAL_ALIGN_CENTRE:
		horizontalAlignment = "centre";
		break;
	}
	std::string verticalAlignment = "top";
	switch ( guiStatic->GetTextVerticalAlignment() )
	{
	case TEXT_VERTICAL_ALIGN_BOTTOM:
		verticalAlignment = "bottom";
		break;
	case TEXT_VERTICAL_ALIGN_TOP:
		verticalAlignment = "top";
		break;
	case TEXT_VERTICAL_ALIGN_CENTRE:
		verticalAlignment = "centre";
		break;
	}
	textElement->SetAttribute( "horizontalAlignment", horizontalAlignment.c_str() );
	textElement->SetAttribute( "verticalAlignment", verticalAlignment.c_str() );		
	textElement->SetAttribute( "size", guiStatic->GetFontSize() );
	Math::Vector4 fontColor = guiStatic->GetFontColor();
	textElement->SetAttribute( "r", static_cast<int>( fontColor[0] * 255.0f ) );
	textElement->SetAttribute( "g", static_cast<int>( fontColor[1] * 255.0f ) );
	textElement->SetAttribute( "b", static_cast<int>( fontColor[2] * 255.0f ) );
	textElement->SetAttribute( "a", static_cast<int>( fontColor[3] * 255.0f ) );
	element->LinkEndChild( textElement );

	tinyxml2::XMLElement* uvElement = NULL;
	uvElement = doc->NewElement( "UV" );
	uvElement->SetAttribute( "x", guiStatic->GetUVPosition()[0] );
	uvElement->SetAttribute( "y", guiStatic->GetUVPosition()[1] );
	uvElement->SetAttribute( "width", guiStatic->GetUVSize()[0] );
	uvElement->SetAttribute( "height", guiStatic->GetUVSize()[1] );
	element->LinkEndChild( uvElement );

	tinyxml2::XMLElement* textureElement = NULL;
	textureElement = doc->NewElement( "Texture" );
	std::string textureName = "";
	if ( guiStatic->GetTexture() )
	{
		textureName = guiStatic->GetTexture()->GetName();
	}
	textureElement->SetAttribute( "name", textureName.c_str() );	
	element->LinkEndChild( textureElement );

	tinyxml2::XMLElement* colorElement = NULL;
	colorElement = doc->NewElement( "Color" );
	Math::Vector4 color = guiStatic->GetColor();
	colorElement->SetAttribute( "r", static_cast<int>( color[0] * 255.0f ) );
	colorElement->SetAttribute( "g", static_cast<int>( color[1] * 255.0f ) );
	colorElement->SetAttribute( "b", static_cast<int>( color[2] * 255.0f ) );
	colorElement->SetAttribute( "a", static_cast<int>( color[3] * 255.0f ) );
	element->LinkEndChild( colorElement );	
}

//===========================================================================

void GuiScreen::SaveButton( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiButton* guiButton )
{
	SaveStatic( doc, element, guiButton );

    tinyxml2::XMLElement* uvElement = NULL;
	uvElement = doc->NewElement( "OnPressUV" );
	uvElement->SetAttribute( "x", guiButton->GetOnPressUVPosition()[0] );
	uvElement->SetAttribute( "y", guiButton->GetOnPressUVPosition()[1] );
	uvElement->SetAttribute( "width", guiButton->GetOnPressUVSize()[0] );
	uvElement->SetAttribute( "height", guiButton->GetOnPressUVSize()[1] );
	element->LinkEndChild( uvElement );

	tinyxml2::XMLElement* textureElement = NULL;
	textureElement = doc->NewElement( "OnPressTexture" );
	std::string textureName = "";
	if ( guiButton->GetOnPressTexture() )
	{
		textureName = guiButton->GetOnPressTexture()->GetName();
	}
	textureElement->SetAttribute( "name", textureName.c_str() );	
	element->LinkEndChild( textureElement );
}

//===========================================================================

void GuiScreen::SaveSlider( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiSlider* guiSlider )
{
	SaveControl( doc, element, guiSlider );

	tinyxml2::XMLElement* uvElementBar = NULL;
	uvElementBar = doc->NewElement( "BarUV" );
	uvElementBar->SetAttribute( "x", guiSlider->GetUVPositionBar()[0] );
	uvElementBar->SetAttribute( "y", guiSlider->GetUVPositionBar()[1] );
	uvElementBar->SetAttribute( "width", guiSlider->GetUVSizeBar()[0] );
	uvElementBar->SetAttribute( "height", guiSlider->GetUVSizeBar()[1] );
	element->LinkEndChild( uvElementBar );

	tinyxml2::XMLElement* textureElementBar = NULL;
	textureElementBar = doc->NewElement( "BarTexture" );
	std::string textureNameBar = "";
	if ( guiSlider->GetTextureBar() )
	{
		textureNameBar = guiSlider->GetTextureBar()->GetName();
	}
	textureElementBar->SetAttribute( "name", textureNameBar.c_str() );	
	element->LinkEndChild( textureElementBar );

	tinyxml2::XMLElement* uvElementButton = NULL;
	uvElementButton = doc->NewElement( "ButtonUV" );
	uvElementButton->SetAttribute( "x", guiSlider->GetUVPositionButton()[0] );
	uvElementButton->SetAttribute( "y", guiSlider->GetUVPositionButton()[1] );
	uvElementButton->SetAttribute( "width", guiSlider->GetUVSizeButton()[0] );
	uvElementButton->SetAttribute( "height", guiSlider->GetUVSizeButton()[1] );
	element->LinkEndChild( uvElementButton );

	tinyxml2::XMLElement* textureElementButton = NULL;
	textureElementButton = doc->NewElement( "ButtonTexture" );
	std::string textureNameButton = "";
	if ( guiSlider->GetTextureButton() )
	{
		textureNameButton = guiSlider->GetTextureButton()->GetName();
	}
	textureElementButton->SetAttribute( "name", textureNameButton.c_str() );	
	element->LinkEndChild( textureElementButton );
}

//===========================================================================

void GuiScreen::SaveSprite( tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* element, GuiSprite* guiSprite )
{
	SaveControl( doc, element, guiSprite );	

	tinyxml2::XMLElement* textureFileElement = NULL;
	textureFileElement = doc->NewElement( "TextureFile" );
	std::string textureName = "";
	if ( guiSprite->GetTexture() )
	{
		textureName = guiSprite->GetTexture()->GetName();
	}
	textureFileElement->SetAttribute( "value", textureName.c_str() );	
	element->LinkEndChild( textureFileElement );

	tinyxml2::XMLElement* fpsElement = NULL;
	fpsElement = doc->NewElement( "fps" );
	fpsElement->SetAttribute( "value", static_cast<double>( guiSprite->GetFramesPerSecond() ) );
	element->LinkEndChild( fpsElement );

	tinyxml2::XMLElement* colorElement = NULL;
	colorElement = doc->NewElement( "Color" );
	Math::Vector4 color = guiSprite->GetColor();
	colorElement->SetAttribute( "r", static_cast<int>( color[0] * 255.0f ) );
	colorElement->SetAttribute( "g", static_cast<int>( color[1] * 255.0f ) );
	colorElement->SetAttribute( "b", static_cast<int>( color[2] * 255.0f ) );
	colorElement->SetAttribute( "a", static_cast<int>( color[3] * 255.0f ) );
	element->LinkEndChild( colorElement );	
}

//===========================================================================

void GuiScreen::CheckUniqueID( int id )
{
	size_t numControls = mControlList.size();
	for ( int i = 0; i < numControls; ++i )
	{
		if ( mControlList[i]->GetID() == id )
		{
			Assert( false, "Not a unique ID" );
		}
	}
}