#include <eikstart.h>
#include <aknapp.h>
#include <akndoc.h>
#include <aknappui.h>
#include <coecntrl.h>
#include "LogNull.hpp"

Log* g_log = new LogNull;

// FtpAppView

class FtpAppView : public CCoeControl
{
public:
	FtpAppView( const TRect& aRect );
	virtual ~FtpAppView() {}
	
	void Draw( const TRect& aRect ) const;
	virtual void SizeChanged() {}
};

FtpAppView::FtpAppView( const TRect& aRect )
{
	CreateWindowL();
	SetRect( aRect );
	ActivateL();
}

void FtpAppView::Draw( const TRect& aRect ) const
{
	CWindowGc& gc = SystemGc();
	gc.SetPenStyle( CGraphicsContext::ENullPen );
	gc.SetBrushColor( KRgbWhite );
	gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
	gc.Clear( Rect() );
}

// FtpAppUi

class FtpAppUi : public CAknAppUi
{
public:
	FtpAppUi() : iAppView( NULL ) {}
	virtual ~FtpAppUi() { delete iAppView; }

	void ConstructL();

	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	FtpAppView* iAppView;
};

void FtpAppUi::ConstructL()
{
	BaseConstructL( 0 );
	iAppView = new FtpAppView( ClientRect() );
}

void FtpAppUi::HandleCommandL( TInt aCommand )
{
	switch( aCommand )
	{
	case EEikCmdExit:
	case EAknSoftkeyExit:
		Exit();
		break;

	default:
		break;
	}
}

void FtpAppUi::HandleResourceChangeL( TInt aType )
{
	CAknAppUi::HandleResourceChangeL( aType );
	if( aType == KEikDynamicLayoutVariantSwitch && iAppView != NULL )
	{
		iAppView->SetRect( ClientRect() );
	}
}

// FtpDocument

class FtpDocument : public CAknDocument
{
public:
	FtpDocument( CEikApplication& aApp ) : CAknDocument( aApp ) {}
	CEikAppUi* CreateAppUiL() { return static_cast<CEikAppUi*>( new FtpAppUi ); }
};

// FtpApplication

const TUid FtpUid = { 0xA0102039 };

class FtpApplication : public CAknApplication
{
public:
	TUid AppDllUid() const { return FtpUid; }
	CApaDocument* CreateDocumentL() { return static_cast<CApaDocument*>( new FtpDocument( *this ) ); }
};

// Startup

LOCAL_C CApaApplication* NewApplication()
{
	return new FtpApplication;
}

GLDEF_C TInt E32Main()
{
	return EikStart::RunApplication( NewApplication );
}
