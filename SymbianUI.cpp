#include <eikstart.h>
#include <aknapp.h>
#include <akndoc.h>
#include <aknappui.h>
#include <aknnavide.h>
#include <eikedwin.h>
#include <aknmessagequerydialog.h>
#include <string>
#include "Log.hpp"
#include "LogNull.hpp"
#include "Server.hpp"
#include "ServerPtr.hpp"
#include "SymbianNetwork.hpp"
#include "AuthNone.hpp"
#include "AuthToken.hpp"
#include "Exceptions.hpp"
#include "resource/dftpd.hrh"
#include <dftpd.rsg>

Log* g_log = NULL;

// FtpAppView

class FtpAppView : public CCoeControl
{
public:
	FtpAppView( const TRect& aRect );
	virtual ~FtpAppView() {}
	
	void Draw( const TRect& aRect ) const { m_view->Draw( aRect ); };
	virtual void SizeChanged() { m_view->SetRect( Rect() ); }

	void Log( const std::string& text );

	CEikEdwin* m_view;
};

FtpAppView::FtpAppView( const TRect& aRect )
{
	CreateWindowL();

	m_view = new CEikEdwin;
	m_view->SetContainerWindowL( *this );
	m_view->ConstructL( EEikEdwinDisplayOnly | EEikEdwinNoAutoSelection );

	SetRect( aRect );
	ActivateL();
}

void FtpAppView::Log( const std::string& _text )
{
	std::string text = _text + '\f';

	TPtrC8 ptr( reinterpret_cast<const TUint8*>( text.c_str() ) );
	TBuf<512> log;
	log.FillZ();
	log.Copy( ptr );

	m_view->Text()->InsertL( m_view->Text()->DocumentLength(), log );
	m_view->HandleTextChangedL();
	m_view->SetCursorPosL( m_view->Text()->DocumentLength(), EFalse );
	m_view->NotifyNewFormatL();
}

// FtpAppUi

class FtpAppUi : public CAknAppUi, public CActive, public Log
{
public:
	FtpAppUi() : CActive( EPriorityStandard ), iAppView( NULL ), m_starter( NULL ), m_authenticationEnabled( true ) { g_log = this; }
	virtual ~FtpAppUi();

	void ConstructL();

	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	static TBool StartL( TAny* aThis );
	void StartL();

	void RunL();
	void DoCancel();

	void Print( const std::string& text );

	void ServerCrashed();

	FtpAppView* iAppView;
	ServerPtr m_server;
	CIdle* m_starter;
	RTimer m_timer;
	AuthPtr m_auth;

	bool m_authenticationEnabled;
};

FtpAppUi::~FtpAppUi()
{
	Cancel();

	g_log = new LogNull;

	if( m_starter )
	{
		m_starter->Cancel();
	}
	delete m_starter;
	delete iAppView;
}

void FtpAppUi::ConstructL()
{
	BaseConstructL( EAknEnableSkin );
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

	case EGenerateToken:
		if( m_server )
		{
			if( !m_authenticationEnabled )
			{
				g_log->Print( "Enabling authentication" );
				m_auth.reset( new AuthToken );
				m_server->SetAuth( m_auth );
				m_authenticationEnabled = true;
			}
			((AuthToken*)m_auth.get())->GenerateToken();
		}
		break;

	case EDisableAuthentication:
		if( m_authenticationEnabled && m_server )
		{
			g_log->Print( "Authentication disabled" );
			m_auth.reset( new AuthNone );
			m_server->SetAuth( m_auth );
			m_authenticationEnabled = false;
		}
		break;

	case EAbout:
		{
			CAknMessageQueryDialog* dlg = new CAknMessageQueryDialog;
			dlg->ExecuteLD( R_DFTPD_ABOUT );
		}
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
	std::string ip;

	try
	{
		m_auth.reset( new AuthToken );
		ip = EstablishConnection();
		m_server = Server::Create( m_auth, ip );
	}
	catch( ServerCrash& e )
	{
		ServerCrashed();
		return;
	}

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
	try
	{
		m_server->Tick();
	}
	catch( ServerCrash& e )
	{
		ServerCrashed();
		return;
	}

	m_timer.After( iStatus, 10 );
	SetActive();
}

void FtpAppUi::DoCancel()
{
	m_timer.Close();
}

void FtpAppUi::Print( const std::string& text )
{
	iAppView->Log( text );
}

void FtpAppUi::ServerCrashed()
{
	CEikStatusPane* sp = iEikonEnv->AppUiFactory()->StatusPane();
	CAknNavigationControlContainer* iNaviPane = (CAknNavigationControlContainer*)sp->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) );
	iNaviPane->Pop();
	_LIT( naviLabel, "Server crashed" );
	iNaviPane->PushL( *iNaviPane->CreateNavigationLabelL( naviLabel ) );

	m_server.reset();
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
