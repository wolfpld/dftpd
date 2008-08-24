#include <eikstart.h>
#include <aknapp.h>
#include <akndoc.h>
#include <aknappui.h>
#include <aknnavide.h>
#include <eikedwin.h>
#include <string>
#include "LogNull.hpp"
#include "Server.hpp"
#include "ServerPtr.hpp"
#include "SymbianNetwork.hpp"

Log* g_log = new LogNull;

// FtpAppView

class FtpAppView : public CCoeControl
{
public:
	FtpAppView( const TRect& aRect );
	virtual ~FtpAppView() {}
	
	void Draw( const TRect& aRect ) const { m_view->Draw( aRect ); };
	virtual void SizeChanged() { m_view->SetRect( Rect() ); }

	CEikEdwin* m_view;
};

FtpAppView::FtpAppView( const TRect& aRect )
{
	CreateWindowL();

	m_view = new CEikEdwin;
	m_view->SetContainerWindowL( *this );
	m_view->ConstructL( EEikEdwinDisplayOnly | EEikEdwinNoAutoSelection );
	m_view->CreateScrollBarFrameL()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

	SetRect( aRect );
	ActivateL();
}

// FtpAppUi

class FtpAppUi : public CAknAppUi, public CActive
{
public:
	FtpAppUi() : CActive( EPriorityStandard ), iAppView( NULL ), m_starter( NULL ) {}
	virtual ~FtpAppUi();

	void ConstructL();

	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	static TBool StartL( TAny* aThis );
	void StartL();

	void RunL();
	void DoCancel();

	FtpAppView* iAppView;
	ServerPtr m_server;
	CIdle* m_starter;
	RTimer m_timer;
};

FtpAppUi::~FtpAppUi()
{
	Cancel();

	if( m_starter )
	{
		m_starter->Cancel();
	}
	delete m_starter;
	delete iAppView;
}

void FtpAppUi::ConstructL()
{
	BaseConstructL( 0 );
	iAppView = new FtpAppView( ClientRect() );

	CActiveScheduler::Add( this );

	m_starter = CIdle::NewL( CActive::EPriorityLow );
	m_starter->Start( TCallBack( StartL, this ) );
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

TBool FtpAppUi::StartL( TAny* aThis )
{
	static_cast<FtpAppUi*>( aThis )->StartL();
	return EFalse;
}

void FtpAppUi::StartL()
{
	std::string ip = EstablishConnection();
	m_server = Server::Create( ip );

	CEikStatusPane* sp = iEikonEnv->AppUiFactory()->StatusPane();
	CAknNavigationControlContainer* iNaviPane = (CAknNavigationControlContainer*)sp->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) );
	iNaviPane->Pop();
	TPtrC8 ptr( reinterpret_cast<const TUint8*>( ip.c_str() ) );
	TBuf<16> naviLabel;
	naviLabel.FillZ();
	naviLabel.Copy( ptr );
	iNaviPane->PushL( *iNaviPane->CreateNavigationLabelL( naviLabel ) );

	m_timer.CreateLocal();
	m_timer.After( iStatus, 10 );
	SetActive();
}

void FtpAppUi::RunL()
{
	m_server->Tick();

	m_timer.After( iStatus, 10 );
	SetActive();
}

void FtpAppUi::DoCancel()
{
	m_timer.Close();
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
	virtual ~FtpApplication() { delete g_log; }
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
