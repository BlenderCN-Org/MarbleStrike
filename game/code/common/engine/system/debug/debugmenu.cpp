//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/system/debug/debugmenu.hpp"
#include "common/engine/system/debug/debugvariablehelper.hpp"
#include "common/engine/system/debug/directoryvariable.hpp"
#include "common/engine/system/keyboard.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace System
{
	namespace Debug
	{
		DebugMenu* DebugMenu::mInstance = NULL;
	}
}

const int NUM_DISPLAY_ITEMS = 18;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	namespace Debug
	{
		void DebugMenu::Create()
		{
			Assert( mInstance == NULL, "" );
			mInstance = NEW_PTR( "Debug Menu" ) DebugMenu;		
		}

		//===========================================================================

		void DebugMenu::Destroy()
		{		
			DELETE_PTR( mInstance );
		}

		//===========================================================================

		DebugMenu* DebugMenu::Get()
		{
			Assert( mInstance != NULL, "" );
			return mInstance;
		}

		//===========================================================================

		void DebugMenu::KeyboardDown( unsigned char key )
		{
			if ( key == System::GetKeyValue( KB_UP_ARROW ) )
			{			
				mCurrentItemIndex--;

				if ( mCurrentItemIndex < 0 )
				{
					mCurrentItemIndex = 0;
				}

				if ( mCurrentItemIndex < mStartItemIndex )
				{
					mStartItemIndex--;
					mEndItemIndex--;
				}
			}
			else if ( key == System::GetKeyValue( KB_DOWN_ARROW ) )
			{
				mCurrentItemIndex++;

				int numItems = mCurrentDirectoryItem->GetDirectoryList()->GetSize();
				if ( mCurrentItemIndex >= numItems )
				{
					mCurrentItemIndex--;
				}

				if ( mCurrentItemIndex >= mEndItemIndex )
				{
					mStartItemIndex++;
					mEndItemIndex++;
				}
			}
			else if ( key == System::GetKeyValue( KB_LEFT_ARROW ) || key == System::GetKeyValue( KB_RIGHT_ARROW ) )
			{
				int count = 0;
				List<DirectoryItem*>::Iterator iter = mCurrentDirectoryItem->GetDirectoryList()->CreateIterator();
				while ( iter.HasNext() )
				{
					DirectoryItem* item = iter.Next();

					if ( item->GetType() == VARIABLE_TYPE )
					{
						DirectoryVariable* variableItem = reinterpret_cast<DirectoryVariable*>( item );
						DebugVariable* variable = variableItem->GetVariable();

						if ( count == mCurrentItemIndex )
						{
							variable->KeyboardDown( key );
							break;
						}
					}

					count++;
				}
			}				
		}

		//===========================================================================

		void DebugMenu::KeyboardChar( unsigned char key )
		{
			if ( key == System::GetKeyValue( KB_ENTER ) )
			{
				int count = 0;
				List<DirectoryItem*>::Iterator iter = mCurrentDirectoryItem->GetDirectoryList()->CreateIterator();
				while ( iter.HasNext() )
				{
					DirectoryItem* item = iter.Next();

					if ( count == mCurrentItemIndex )
					{
						if ( item->GetType() == SUB_DIRECTORY_TYPE )
						{
							DirectorySubDir* subDirectoryItem = reinterpret_cast<DirectorySubDir*>( item );
							mCurrentDirectoryItem = subDirectoryItem;

							mSelectionHistory.push( mCurrentItemIndex );

							mCurrentItemIndex = 0;
							mStartItemIndex = 0;
							mEndItemIndex = NUM_DISPLAY_ITEMS;
							break;
						}
					}

					count++;
				}
			}
			else if ( key == System::GetKeyValue( KB_BACKSPACE ) )
			{
				DirectorySubDir* directoryItem = mCurrentDirectoryItem->GetPreviousDirectoryItem();
				if ( directoryItem != NULL )
				{
					mCurrentDirectoryItem = directoryItem;
					mCurrentItemIndex = mSelectionHistory.back();
					mSelectionHistory.pop();
					mStartItemIndex = 0;
					mEndItemIndex = NUM_DISPLAY_ITEMS;
				}
			}
		}

		//===========================================================================

		void DebugMenu::Update()
		{

		}

		//===========================================================================

		void DebugMenu::Render()
		{
			Renderer::Get()->ClearDepth( NULL );

			SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();

			Math::Matrix44 transform;
			transform.SetIdentity();	

			float halfscreenWidthSize = static_cast<float>( Database::Get()->GetAppScreenWidth() ) /2.0f;
			float halfscreenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() ) /2.0f;

			Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection( 
				-halfscreenWidthSize,
				-halfscreenHeightSize,
				halfscreenWidthSize,
				halfscreenHeightSize,
				-100,
				100
				);

			Math::Matrix44 projectionMatrix = orthoMatrix;
			Math::Matrix44 viewMatrix;
			viewMatrix.SetIdentity();

			Math::Matrix44 objectToProjection = projectionMatrix * transform;
			objectToProjection *= viewMatrix;	

			simpleDraw->SetProjectionMatrix( projectionMatrix );
			simpleDraw->SetViewMatrix( viewMatrix );
			simpleDraw->SetTransformMatrix( transform );
			simpleDraw->SetDiffuseColor( Math::Vector4( 0.3f, 0.3f, 0.3f, 0.5f ) );

			const float windowX = -120;
			const float windowWidth = 220;
			const float windowY = 100;
			const float windowHeight = 200;		

			const Math::Vector3 v0( windowX, windowY, 0 );
			const Math::Vector3 v1( windowX, windowY - windowHeight, 0 );
			const Math::Vector3 v2( windowX + windowWidth, windowY - windowHeight, 0 );
			const Math::Vector3 v3( windowX + windowWidth, windowY, 0 );
			simpleDraw->RenderQuad( v0, v1, v2, v3 );

			float textY = 0.0f;
			int count = 0;
			const float xNameOffset = windowX + 2.0f;
			const float xValueOffset = 50.0f;
			const float yNameOffset = windowY - 20.0f;
			const float yIncrement = 10.0f;

			List<DirectoryItem*>::Iterator iter = mCurrentDirectoryItem->GetDirectoryList()->CreateIterator();
			while ( iter.HasNext() )
			{
				DirectoryItem* item = iter.Next();

				if ( count >= mStartItemIndex && count < mEndItemIndex )
				{
					Math::Vector4 color = TEXT_COLOR_WHITE;
					if ( count == mCurrentItemIndex )
					{
						color = TEXT_COLOR_RED;
					}			

					float yPos = yNameOffset - textY;
					mFont->AddToBuffer( 
						xNameOffset, 
						yPos, 
						500,
						50,
						TEXT_HORIZONTAL_ALIGN_LEFT, 
						TEXT_VERTICAL_ALIGN_BOTTOM,
						color, 
						item->GetName() 
						);

					if ( item->GetType() == VARIABLE_TYPE )
					{
						DirectoryVariable* variableItem = reinterpret_cast<DirectoryVariable*>( item );
						DebugVariable* variable = variableItem->GetVariable();
						char buffer[256];
						switch ( variable->GetType() )
						{
						case System::Debug::VAR_INT:
							{					
								DebugVariableInt* variableInt = reinterpret_cast<DebugVariableInt*>( variable );
								snprintf( buffer, 256, "%i", (int)*variableInt );
							}
							break;
						case System::Debug::VAR_FLOAT:
							{					
								DebugVariableFloat* variableFloat = reinterpret_cast<DebugVariableFloat*>( variable );
								snprintf( buffer, 256, "%3.2f", (float)*variableFloat );
							}
							break;
						case System::Debug::VAR_BOOL:
							{
								DebugVariableBool* variableBool = reinterpret_cast<DebugVariableBool*>( variable );
								if ( (bool)*variableBool == true )
								{
									snprintf( buffer, 256, "true" );
								}
								else
								{
									snprintf( buffer, 256, "false" );
								}
							}
							break;
						default:
							break;
						}

						mFont->AddToBuffer( 
							xValueOffset, 
							yPos, 
							500,
							50,
							TEXT_HORIZONTAL_ALIGN_LEFT, 
							TEXT_VERTICAL_ALIGN_BOTTOM,
							color,
							buffer
							);
					}

					textY += yIncrement;
				}

				count++;
			}

			transform.SetIdentity();
			transform.Translate( 0, 0, 0 );

			Math::Rectangle<int> scissorRectangle;
			scissorRectangle.SetPosition( 0, 0 );
			scissorRectangle.SetWidth( Database::Get()->GetBackBufferWidth() );
			scissorRectangle.SetHeight( Database::Get()->GetBackBufferHeight() );
			mFont->RenderBuffers( orthoMatrix, transform, scissorRectangle );
		}

		//===========================================================================

		DirectorySubDir* DebugMenu::FindDirectoryItem( List<DirectoryItem*> &directory, const char* name )
		{
			List<DirectoryItem*>::Iterator iter = directory.CreateIterator();
			while ( iter.HasNext() )
			{
				DirectoryItem* item = iter.Next();
				if ( strcmp( item->GetName(), name ) == 0 )
				{
					DirectorySubDir* subDirectoryItem = reinterpret_cast<DirectorySubDir*>( item );
					if ( subDirectoryItem != NULL )
					{
						return subDirectoryItem;
					}
				}
			}

			return NULL;
		}

		//===========================================================================

		void DebugMenu::DeleteDirectory( List<DirectoryItem*>* directory )
		{
			List<DirectoryItem*>::Iterator iter = directory->CreateIterator();
			while ( iter.HasNext() )
			{			
				DirectoryItem* item = iter.Next();
				if ( item->GetType() == SUB_DIRECTORY_TYPE )
				{
					DirectorySubDir* subDirectoryItem = reinterpret_cast<DirectorySubDir*>( item );			
					DeleteDirectory( subDirectoryItem->GetDirectoryList() );
					subDirectoryItem->GetDirectoryList()->Clear();
				}

				DELETE_PTR( item );
			}
		}

		//===========================================================================

		bool SortFunction( DirectoryItem* item1, DirectoryItem* item2 )
		{
			DirectoryType type1 = item1->GetType();
			DirectoryType type2 = item2->GetType();

			if ( type1 == VARIABLE_TYPE && type2 == SUB_DIRECTORY_TYPE )
			{
				return false;
			}
			else if ( type2 == VARIABLE_TYPE && type1 == SUB_DIRECTORY_TYPE )
			{
				return true;
			}
			else if ( type1 == type2 )
			{
				if ( strcmp( item1->GetName(), item2->GetName() ) < 0 )
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			return false;
		}

		//===========================================================================

		void DebugMenu::SortDirectory( List<DirectoryItem*>* directory )
		{
			List<DirectoryItem*>::Iterator iter = directory->CreateIterator();
			while ( iter.HasNext() )
			{			
				DirectoryItem* item = iter.Next();
				if ( item->GetType() == SUB_DIRECTORY_TYPE )
				{
					DirectorySubDir* subDirectoryItem = reinterpret_cast<DirectorySubDir*>( item );			
					SortDirectory( subDirectoryItem->GetDirectoryList() );

					subDirectoryItem->GetDirectoryList()->Sort( SortFunction );
				}
			}
		}

		//===========================================================================

		DebugMenu::DebugMenu()
		{
			mFont = NEW_PTR( "Font" ) Font;
			mFont->LoadOther( "borisblackbloxx");
			mFont->SetSize( 0.3f );

			mCurrentItemIndex = 0;

			char* nextToken;

			DebugVariable* head = DebugVariable::GetReferenceList();		
			while ( head != NULL )
			{	
				char directoryPath[256];
				System::StringCopy( directoryPath, 256, head->GetDirectory() );

				char* directoryName;
				directoryName = System::StringToken( directoryPath, "/", &nextToken );

				DirectorySubDir* currentDirectory = &mDirectory;
				while ( directoryName != NULL)
				{
					DirectorySubDir* directoryItem = FindDirectoryItem( *( currentDirectory->GetDirectoryList() ), directoryName );
					if ( directoryItem == NULL )
					{
						directoryItem = NEW_PTR( "Directory Item" ) DirectorySubDir;
						directoryItem->SetName( directoryName );
						directoryItem->SetPreviousDirectoryItem( currentDirectory );
						currentDirectory->GetDirectoryList()->InsertBack( directoryItem );
					}				

					currentDirectory = directoryItem;

					directoryName = System::StringToken( NULL, "/", &nextToken );
				}

				DirectoryVariable* newItem = NEW_PTR( "Variable Item" ) DirectoryVariable;
				newItem->SetVariable( head );
				newItem->SetName( head->GetName() );
				currentDirectory->GetDirectoryList()->InsertBack( newItem );

				head = head->GetNextVar();
			}		

			mStartItemIndex = 0;
			mEndItemIndex = NUM_DISPLAY_ITEMS;

			SortDirectory( mDirectory.GetDirectoryList() );
			mDirectory.GetDirectoryList()->Sort( SortFunction );

			mCurrentDirectoryItem = &mDirectory;
		}

		//===========================================================================

		DebugMenu::~DebugMenu()
		{
			DELETE_PTR( mFont );

			DeleteDirectory( mDirectory.GetDirectoryList() );
		}
	}
}