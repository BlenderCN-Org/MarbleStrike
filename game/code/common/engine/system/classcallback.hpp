#ifndef CLASS_CALLBACK_HPP
#define CLASS_CALLBACK_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ClassCallback
{
public:
    virtual ~ClassCallback()
    {    
    }

	//
	// Two possible functions to call member function. virtual cause derived
	// classes will use a pointer to an object and a pointer to a member function
	// to make the function call.
	//
	virtual void operator()( void* ) = 0;  // call using operator
	virtual void Call( void* ) = 0;        // call using function	
};

//===========================================================================

template <class ClassType> 
class ClassCallbackSpecific : public ClassCallback
{
public:

	// Constructor - Takes pointer to an object and pointer to a member and stores
	// them in two private variables
	//
	ClassCallbackSpecific( ClassType* object, void( ClassType::*callback )( void* ) )
	{ 
		mObject = object;  
		mCallback = callback; 
	}

	virtual void operator()( void* userData )
	{ 
		( *mObject.*mCallback )( userData );
	}

	virtual void Call( void* userData )
	{ 
		( *mObject.*mCallback )( userData );
	}

	ClassType* GetObjectThis()
	{
		return mObject;
	}

	void( ClassType::*GetCallback() )( void* )
	{
		return mCallback;
	}

private:

	void ( ClassType::*mCallback )( void* );
	ClassType* mObject;

};

#endif
