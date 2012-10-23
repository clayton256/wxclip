/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWindows sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: minimal.cpp,v 1.37.2.1 2000/05/17 11:02:53 JS Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include   <iostream>

#ifdef __GNUG__
    #pragma implementation "minimal.cpp"
    #pragma interface "minimal.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/checkbox.h>
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__) ||defined(__WXOSX_COCOA__)
    #include "clipboard.xpm"
#endif

#include <wx/dataobj.h>
#include <wx/clipbrd.h>
#include <wx/config.h>
#include <wx/wfstream.h>
#include <wx/taskbar.h>

const static int wxClip_MaxLabel = 14;
const static int wxClip_MaxText  = 2048;

#if defined(__WXGTK__)
const static int wxClip_Width  = 130;
const static int wxClip_Height = 280;
const static int wxClip_MenuHeight = 27;
#elif defined(__WXMSW__)
const static int wxClip_Width = 130;
const static int wxClip_Height = 270;
const static int wxClip_MenuHeight = 20;
#elif defined(__WXOSX_COCOA__)
const static int wxClip_Width = 130;
const static int wxClip_Height = 270;
const static int wxClip_MenuHeight = 0;
#else
#error need width and height
#endif



// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

class MyTaskBarIcon : public wxTaskBarIcon
{
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
    MyTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE)
    :   wxTaskBarIcon(iconType)
#else
    MyTaskBarIcon()
#endif
    {}
#if 0
    void OnLeftButtonDClick(wxTaskBarIconEvent&);
    void OnMenuRestore(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    void OnMenuSetNewIcon(wxCommandEvent&);
    void OnMenuCheckmark(wxCommandEvent&);
    void OnMenuUICheckmark(wxUpdateUIEvent&);
    void OnMenuSub(wxCommandEvent&);
    virtual wxMenu *CreatePopupMenu();
#endif
    DECLARE_EVENT_TABLE()
};





// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
    virtual int OnExit();
};


// A custom dialog
class MyDialog : public wxDialog
{
public:
    MyDialog(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
            const long style = wxDEFAULT_DIALOG_STYLE);

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
    bool TransferDataToWindow(void);
    bool TransferDataFromWindow(void);

    wxString    String_Label;
    wxString    String_Text;

    wxCheckBox *checkbox;

private:

    wxTextCtrl *TextCtrl_Label;
    wxTextCtrl *TextCtrl_Text;

   // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};



// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    virtual ~MyFrame();

    wxButton      *ButtonA;
    wxButton      *ButtonB;
    wxButton      *ButtonC;
    wxButton      *ButtonD;
    wxButton      *ButtonE;
    wxButton      *ButtonF;
    wxButton      *ButtonG;
    wxButton      *ButtonH;
    wxButton      *ButtonI;
    wxButton      *ButtonJ;

    wxBitmapButton      *ButtonConfigA;
    wxBitmapButton      *ButtonConfigB;
    wxBitmapButton      *ButtonConfigC;
    wxBitmapButton      *ButtonConfigD;
    wxBitmapButton      *ButtonConfigE;
    wxBitmapButton      *ButtonConfigF;
    wxBitmapButton      *ButtonConfigG;
    wxBitmapButton      *ButtonConfigH;
    wxBitmapButton      *ButtonConfigI;
    wxBitmapButton      *ButtonConfigJ;

    wxConfigBase  *pConfig;

    // event handlers (these functions should _not_ be virtual)
    void OnButtonClick(wxCommandEvent& event);
    void OnButtonConfigClick(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnConfig(wxCommandEvent& event);
    void OnAbout( wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnHelp(wxCommandEvent& event);

    void DoPasteFromClipboard(wxString *text);
    void DoCopyToClipboard(wxString text);

    int id;

private:

    MyTaskBarIcon   *m_taskBarIcon;
#if defined(__WXOSX__) && wxOSX_USE_COCOA
    MyTaskBarIcon   *m_dockIcon;
#endif

    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    wxClipButtonA = 'A',
    wxClipButtonB,
    wxClipButtonC,
    wxClipButtonD,
    wxClipButtonE,
    wxClipButtonF,
    wxClipButtonG,
    wxClipButtonH,
    wxClipButtonI,
    wxClipButtonJ,
    wxClipButtonSentinal
};


enum
{
    wxClipButtonConfigA = 'a',
    wxClipButtonConfigB,
    wxClipButtonConfigC,
    wxClipButtonConfigD,
    wxClipButtonConfigE,
    wxClipButtonConfigF,
    wxClipButtonConfigG,
    wxClipButtonConfigH,
    wxClipButtonConfigI,
    wxClipButtonConfigJ,
    wxClipButtonConfigSentinal
};



enum
{
    Menu_MenuBar_Exit = 101,
    Menu_MenuBar_Config,
    Menu_MenuBar_Help,
    Menu_MenuBar_About,
    Menu_MenuBar_Sentinal
};


enum
{
    Button_Config_Okay,
    Dialog_Config,
    Text_Config_Label,
    Text_Config_Text,
    CheckBox_AsFile,
    Sentinal_config
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Menu_MenuBar_Exit,    MyFrame::OnQuit)
    EVT_MENU(Menu_MenuBar_Help,    MyFrame::OnHelp)
    EVT_MENU(Menu_MenuBar_Config,  MyFrame::OnConfig)
    EVT_MENU(Menu_MenuBar_About,   MyFrame::OnAbout)

    EVT_BUTTON(wxClipButtonA,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonB,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonC,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonD,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonE,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonF,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonG,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonH,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonI,  MyFrame::OnButtonClick)
    EVT_BUTTON(wxClipButtonJ,  MyFrame::OnButtonClick)

    EVT_BUTTON(wxClipButtonConfigA,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigB,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigC,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigD,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigE,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigF,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigG,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigH,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigI,  MyFrame::OnButtonConfigClick)
    EVT_BUTTON(wxClipButtonConfigJ,  MyFrame::OnButtonConfigClick)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyDialog, wxDialog)
    EVT_BUTTON(wxID_OK, MyDialog::OnOk)
    EVT_BUTTON(wxID_CANCEL, MyDialog::OnCancel)
    EVT_BUTTON(wxID_PASTE, MyDialog::OnPaste)
    EVT_BUTTON(wxID_CLEAR, MyDialog::OnClear)
    //EVT_CHECKBOX(CheckBox_AsFile, MyDialog::OnToggleClock)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
#if 0
    EVT_MENU(PU_RESTORE, MyTaskBarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT,    MyTaskBarIcon::OnMenuExit)
    EVT_MENU(PU_NEW_ICON,MyTaskBarIcon::OnMenuSetNewIcon)
    EVT_MENU(PU_CHECKMARK,MyTaskBarIcon::OnMenuCheckmark)
    EVT_UPDATE_UI(PU_CHECKMARK,MyTaskBarIcon::OnMenuUICheckmark)
    EVT_TASKBAR_LEFT_DCLICK  (MyTaskBarIcon::OnLeftButtonDClick)
    EVT_MENU(PU_SUB1, MyTaskBarIcon::OnMenuSub)
    EVT_MENU(PU_SUB2, MyTaskBarIcon::OnMenuSub)
#endif
END_EVENT_TABLE()



// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

MyFrame *frame;

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    if ( !wxTaskBarIcon::IsAvailable() )
    {
        wxMessageBox
        (
            "There appears to be no system tray support in your current environment.\
             This sample may not behave as expected.",
            "Warning",
            wxOK | wxICON_EXCLAMATION
        );
    }


    // create the main application window
    frame = new MyFrame(_T("Clipboard Storage"), wxPoint(50, 50), 
                                             wxSize(wxClip_Width, wxClip_Height));  

    SetVendorName(_T("Mark Clayton"));
    SetAppName(_T("wxclip")); 

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return true;
}

int MyApp::OnExit()
{
  // clean up: Set() returns the active config object as Get() does, but unlike
  // Get() it doesn't try to create one if there is none (definitely not what
  // we want here!)
  delete wxConfigBase::Set((wxConfigBase *) NULL);

  return 0;
}


// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size,
       wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER|wxMAXIMIZE_BOX))
{
    pConfig = wxConfigBase::Get(_T("wxclip"));

#if defined(__WXGTK__) || defined(__WXMOTIF__) ||defined(__WXOSX_COCOA__)
    // set the frame icon
#ifndef MYTASKBARICON
    SetIcon(wxICON(clipboard));
#endif
#endif
    wxString dir = "/Library/Application Support/wxclip/pixmaps/";

    wxString label;
    if(TRUE != pConfig->Read(_T("/A/Label"), &label)) label = _T("A");
    ButtonA = new wxButton(this, wxClipButtonA, label.Left(wxClip_MaxLabel), wxPoint(1, 1), wxSize(101,25));
    wxBitmap bitmapA = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigA = new wxBitmapButton(this, wxClipButtonConfigA, bitmapA, wxPoint(105, 1));

    if(TRUE != pConfig->Read(_T("/B/Label"), &label)) label = _T("B");
    ButtonB = new wxButton(this, wxClipButtonB, label.Left(wxClip_MaxLabel), wxPoint(1, 25), wxSize(101,25));
    wxBitmap bitmapB = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigB = new wxBitmapButton(this, wxClipButtonConfigB, bitmapB, wxPoint(105, 25));

    if(TRUE != pConfig->Read(_T("/C/Label"), &label)) label = _T("C");
    ButtonC = new wxButton(this, wxClipButtonC, label.Left(wxClip_MaxLabel), wxPoint(1, 50), wxSize(101,25));
    wxBitmap bitmapC = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigC = new wxBitmapButton(this, wxClipButtonConfigC, bitmapC, wxPoint(105, 50));

    if(TRUE != pConfig->Read(_T("/D/Label"), &label)) label = _T("D");
    ButtonD = new wxButton(this, wxClipButtonD, label.Left(wxClip_MaxLabel), wxPoint(1, 75), wxSize(101,25));
    wxBitmap bitmapD = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigD = new wxBitmapButton(this, wxClipButtonConfigD, bitmapD, wxPoint(105, 75));

    if(TRUE != pConfig->Read(_T("/E/Label"), &label)) label = _T("E");
    ButtonE = new wxButton(this, wxClipButtonE, label.Left(wxClip_MaxLabel), wxPoint(1,100), wxSize(101,25));
    wxBitmap bitmapE = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigE = new wxBitmapButton(this, wxClipButtonConfigE, bitmapE, wxPoint(105,100));

    if(TRUE != pConfig->Read(_T("/F/Label"), &label)) label = _T("F");
    ButtonF = new wxButton(this, wxClipButtonF, label.Left(wxClip_MaxLabel), wxPoint(1,125), wxSize(101,25));
    wxBitmap bitmapF = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigF = new wxBitmapButton(this, wxClipButtonConfigF, bitmapF, wxPoint(105,125));

    if(TRUE != pConfig->Read(_T("/G/Label"), &label)) label = _T("G");
    ButtonG = new wxButton(this, wxClipButtonG, label.Left(wxClip_MaxLabel), wxPoint(1,150), wxSize(101,25));
    wxBitmap bitmapG = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigG = new wxBitmapButton(this, wxClipButtonConfigG, bitmapG, wxPoint(105,150));

    if(TRUE != pConfig->Read(_T("/H/Label"), &label)) label = _T("H");
    ButtonH = new wxButton(this, wxClipButtonH, label.Left(wxClip_MaxLabel), wxPoint(1,175), wxSize(101,25));
    wxBitmap bitmapH = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigH = new wxBitmapButton(this, wxClipButtonConfigH, bitmapH, wxPoint(105,175));

    if(TRUE != pConfig->Read(_T("/I/Label"), &label)) label = _T("I");
    ButtonI = new wxButton(this, wxClipButtonI, label.Left(wxClip_MaxLabel), wxPoint(1,200), wxSize(101,25));
    wxBitmap bitmapI = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigI = new wxBitmapButton(this, wxClipButtonConfigI, bitmapI, wxPoint(105,200));

    if(TRUE != pConfig->Read(_T("/J/Label"), &label)) label = _T("J");
    ButtonJ = new wxButton(this, wxClipButtonJ, label.Left(wxClip_MaxLabel), wxPoint(1,225), wxSize(101,25));
    wxBitmap bitmapJ = wxBitmap(dir + _T("clipboard.bmp"), wxBITMAP_TYPE_BMP);
    ButtonConfigJ = new wxBitmapButton(this, wxClipButtonConfigJ, bitmapJ, wxPoint(105,225));

    pConfig->SetPath(_T("/MainFrame"));
    // restore frame position and size
    int x = pConfig->Read(_T("x"), 50),
        y = pConfig->Read(_T("y"), 50),
        w = pConfig->Read(_T("w"), wxClip_Width),
        h = pConfig->Read(_T("h"), wxClip_Height);
    Move(x, y);
    SetClientSize(w, h);

    wxMenu *menubarMenu = new wxMenu;
    menubarMenu->Append(Menu_MenuBar_Exit, _T("E&xit"), _T("Exit"));
    menubarMenu->Append(Menu_MenuBar_Config, _T("&Config"), _T("Set up Clipboards"));
    menubarMenu->Append(Menu_MenuBar_Help, _T("&Help"), _T("Help"));
    menubarMenu->Append(Menu_MenuBar_About, _T("&About"), _T("Product Info"));
    wxMenuBar* menuBar = new wxMenuBar( wxMB_DOCKABLE );
    menuBar->Append(menubarMenu, _T("&Options"));
    // associate the menu bar with the frame
    SetMenuBar(menuBar);
    //menuBar->GetHeight();

    m_taskBarIcon = new MyTaskBarIcon();
    if(NULL == m_taskBarIcon)
    {
        wxLogError("m_taskBarIcon==NULL");
    }
    // we should be able to show up to 128 characters on recent Windows versions
    // (and 64 on Win9x)
    if ( !m_taskBarIcon->SetIcon(wxICON(clipboard),
                                 "wxTaskBarIcon Sample\n"
                                 "With a very, very, very, very\n"
                                 "long tooltip whose length is\n"
                                 "greater than 64 characters.") )
    {
        wxLogError(wxT("Could not set icon."));
    }

#if defined(__WXOSX__) && wxOSX_USE_COCOA
    m_dockIcon = new MyTaskBarIcon(wxTBI_DOCK);
    if ( !m_dockIcon->SetIcon(wxICON(clipboard)) )
    {
        wxLogError(wxT("Could not set icon."));
    }
#endif

    return;
}


MyFrame::~MyFrame()
{
    delete m_taskBarIcon;
#if defined(__WXCOCOA__)
    delete m_dockIcon;
#endif

  // save the frame position
  int x, y, w, h;
  GetClientSize(&w, &h);
  GetPosition(&x, &y);
  pConfig->Write(_T("/MainFrame/x"), (long) x);
  pConfig->Write(_T("/MainFrame/y"), (long) y);
  pConfig->Write(_T("/MainFrame/w"), (long) w);
  pConfig->Write(_T("/MainFrame/h"), (long) h+wxClip_MenuHeight);
}



// event handlers
void MyFrame::OnSize(wxSizeEvent& event)
{
    SetClientSize(GetClientSize());

    event.Skip();
}




void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}


void MyFrame::OnConfig(wxCommandEvent& WXUNUSED(event))
{
    wxMessageDialog dialog(this, _T("There is nothing to configure yet"), _T("wxClip Configuration"), wxOK );
    dialog.ShowModal();
}

void MyFrame::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
    wxMessageDialog dialog(this, _T("wxClip by Mark Clayton\nmark@brown-pelican.com"), _T("Product Information"), wxOK );
    dialog.ShowModal();
}


void MyFrame::OnHelp( wxCommandEvent& WXUNUSED(event) )
{
    wxMessageDialog dialog(this, _T("No Help yet"), _T("wxClip Help"), wxOK );
    dialog.ShowModal();
}



void
MyFrame::OnButtonClick(wxCommandEvent& event)
{
    wxString text;
    bool     asFile;

    switch(event.GetId())
    {
    case wxClipButtonA:
        pConfig->Read(_T("/A/Text"), &text);
        pConfig->Read(_T("/A/AsFile"), &asFile);
        break;
    case wxClipButtonB:
        pConfig->Read(_T("/B/Text"), &text);
        pConfig->Read(_T("/B/AsFile"), &asFile);
        break;
    case wxClipButtonC:
        pConfig->Read(_T("/C/Text"), &text);
        pConfig->Read(_T("/C/AsFile"), &asFile);
        break;
    case wxClipButtonD:
        pConfig->Read(_T("/D/Text"), &text);
        pConfig->Read(_T("/D/AsFile"), &asFile);
        break;
    case wxClipButtonE:
        pConfig->Read(_T("/E/Text"), &text);
        pConfig->Read(_T("/E/AsFile"), &asFile);
        break;
    case wxClipButtonF:
        pConfig->Read(_T("/F/Text"), &text);
        pConfig->Read(_T("/F/AsFile"), &asFile);
        break;
    case wxClipButtonG:
        pConfig->Read(_T("/G/Text"), &text);
        pConfig->Read(_T("/G/AsFile"), &asFile);
        break;
    case wxClipButtonH:
        pConfig->Read(_T("/H/Text"), &text);
        pConfig->Read(_T("/H/AsFile"), &asFile);
        break;
    case wxClipButtonI:
        pConfig->Read(_T("/I/Text"), &text);
        pConfig->Read(_T("/I/AsFile"), &asFile);
        break;
    case wxClipButtonJ:
        pConfig->Read(_T("/J/Text"), &text);
        pConfig->Read(_T("/J/AsFile"), &asFile);
        break;
    default:
        wxASSERT(wxClipButtonSentinal >= event.GetId());
        break;
    }

    if(true == asFile)
    {
        const char * name = text.ToUTF8();
        {
            wxMessageDialog dialog(this, name, _T("wxClip Error"), wxOK );
            dialog.ShowModal();
        }
        wxFileName fname = wxFileName(name);
        if (true == fname.FileExists()) //wxFile::Exists(name))
        {
            if(true == wxFile::Access(name, wxFile::read))
            {
                wxFile f(name, wxFile::read);
                if(TRUE == f.IsOpened())
                {
                    off_t c = f.Length();
                    wxChar *a = new wxChar[c+1];
                    off_t l = f.Read(a, c);
                    wxASSERT(c==l);
                    f.Close();
                    text = a;
                    text.c_str();
                    delete a;
                }
                else
                {
                    wxMessageDialog dialog(this, _T("File open failed"), _T("wxClip Error"), wxOK );
                    dialog.ShowModal();
                }
            }
            else
            {
                wxMessageDialog dialog(this, _T("File is not readable"), _T("wxClip Error"), wxOK );
                dialog.ShowModal();
            }
        }
        else
        {
            wxMessageDialog dialog(this, _T("File does not exist"), _T("wxClip Error"), wxOK );
            dialog.ShowModal();
        }
    }
    DoCopyToClipboard(text);

    return;
}

void
MyFrame::OnButtonConfigClick(wxCommandEvent& event)
{
    id = event.GetId();

    MyDialog dialog(this, _T("Storage Setup dialog"), wxPoint(100, 100), wxSize(400, 300));

    if(wxID_OK == dialog.ShowModal())
    {
        wxString label;
        switch(event.GetId() - ('a' - 'A'))
        {
        case wxClipButtonA:
            if(TRUE != pConfig->Read(_T("/A/Label"), &label)) label = _T("A");
            ButtonA->SetLabel(label);
            break;
        case wxClipButtonB:
            if(TRUE != pConfig->Read(_T("/B/Label"), &label)) label = _T("B");
            ButtonB->SetLabel(label);
            break;
        case wxClipButtonC:
            if(TRUE != pConfig->Read(_T("/C/Label"), &label)) label = _T("C");
            ButtonC->SetLabel(label);
            break;
        case wxClipButtonD:
            if(TRUE != pConfig->Read(_T("/D/Label"), &label)) label = _T("D");
            ButtonD->SetLabel(label);
            break;
        case wxClipButtonE:
            if(TRUE != pConfig->Read(_T("/E/Label"), &label)) label = _T("E");
            ButtonE->SetLabel(label);
            break;
        case wxClipButtonF:
            if(TRUE != pConfig->Read(_T("/F/Label"), &label)) label = _T("F");
            ButtonF->SetLabel(label);
            break;
        case wxClipButtonG:
            if(TRUE != pConfig->Read(_T("/G/Label"), &label)) label = _T("G");
            ButtonG->SetLabel(label);
            break;
        case wxClipButtonH:
            if(TRUE != pConfig->Read(_T("/H/Label"), &label)) label = _T("H");
            ButtonH->SetLabel(label);
            break;
        case wxClipButtonI:
            if(TRUE != pConfig->Read(_T("/I/Label"), &label)) label = _T("I");
            ButtonI->SetLabel(label);
            break;
        case wxClipButtonJ:
            if(TRUE != pConfig->Read(_T("/J/Label"), &label)) label = _T("J");
            ButtonJ->SetLabel(label);
            break;
        default:
            wxASSERT(wxClipButtonSentinal >= event.GetId());
            break;
        }
    }
    else
    {
        ;
    }

    return;
}

void MyFrame::DoCopyToClipboard(wxString text)
{
    // On X11, we want to get the data from the primary selection instead
    // of the normal clipboard (which isn't normal under X11 at all). This
    // call has no effect under MSW.
    wxTheClipboard->UsePrimarySelection();

    if (text.IsEmpty())
    {
        //cout << "No text to copy.\n";

        return;
    }

    if (!wxTheClipboard->Open())
    {
        //cout << "Error opening the clipboard.\n";

        return;
    }
    else
    {
        //cout << "Successfully opened the clipboard.\n";
    }

    wxTextDataObject *data = new wxTextDataObject( text );

    if (!wxTheClipboard->SetData( data ))
    {
        //cout << "Error while copying to the clipboard.\n";
    }
    else
    {
        //cout << "Successfully copied data to the clipboard.\n";
    }

    wxTheClipboard->Close();

    //cout << "Closed the clipboard.\n";
}



void MyFrame::DoPasteFromClipboard(wxString *text)
{
    // On X11, we want to get the data from the primary selection instead
    // of the normal clipboard (which isn't normal under X11 at all). This
    // call has no effect under MSW.
    wxTheClipboard->UsePrimarySelection();

    if (!wxTheClipboard->Open())
    {
        //cout << "Error opening the clipboard.\n";
        return;
    }
    else
    {
        //cout << "Successfully opened the clipboard.\n";
    }

    wxTextDataObject data;

    if (wxTheClipboard->IsSupported( data.GetFormat() ))
    {
        //cout << "Clipboard supports requested format.\n";

        if (wxTheClipboard->GetData( data ))
        {
           // cout << "Successfully retrieved data from the clipboard.\n";
            *text << data.GetText() << _T("\n");
            //cout << *text;
        }
        else
        {
            //cout << "Error getting data from the clipboard.\n";
        }
    }
    else
    {
        //cout << "Clipboard doesn't support requested format.\n";
    }

    wxTheClipboard->Close();

    //cout << "Closed the clipboard.\n";
}




// ----------------------------------------------------------------------------
// MyDialog
// ----------------------------------------------------------------------------
MyDialog::MyDialog( wxWindow *parent, const wxString& title,
                    const wxPoint& pos, const wxSize& size, const long WXUNUSED(style) ) :
    wxDialog(parent, Dialog_Config, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP), checkbox(NULL)
{
    wxString string;
    wxButton *ok = new wxButton(this, wxID_OK, _T("OK"), wxPoint(25, 210), wxSize(80, 30));
    (void)new wxButton(this, wxID_CANCEL, _T("Cancel"), wxPoint(115, 210), wxSize(80, 30));
    (void)new wxButton(this, wxID_CLEAR, _T("Clear"), wxPoint(208, 210), wxSize(80, 30));
    (void)new wxButton(this, wxID_PASTE, _T("Paste"), wxPoint(295, 210), wxSize(80, 30));
    
    TextCtrl_Label = new wxTextCtrl(this, Text_Config_Label, _T(""), wxPoint(10, 10), wxSize(120, -1), 0, wxTextValidator(wxFILTER_NONE, &String_Label));
    TextCtrl_Text  = new wxTextCtrl(this, Text_Config_Text,  _T(""), wxPoint(10, 50), wxSize(380, 140), wxTE_MULTILINE|wxHSCROLL, wxTextValidator(wxFILTER_NONE, &String_Text));

    checkbox = new wxCheckBox(this, CheckBox_AsFile, _T("Interprete As Filename"), wxPoint(160, 10));
    checkbox->SetValue(FALSE);

    ok->SetFocus();
    ok->SetDefault();

    return;
}

void MyDialog::OnClear(wxCommandEvent& WXUNUSED(event))
{
    TextCtrl_Text->SetValue(_T(""));

    return;
}

void MyDialog::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    wxString text;
    frame->DoPasteFromClipboard(&text);
    TextCtrl_Text->SetValue(text);
    //cout << text;

    return;
}

void MyDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
    //TransferDataFromWindow();
    String_Label = TextCtrl_Label->GetValue();
    String_Text  = TextCtrl_Text->GetValue();

    long asFile = checkbox->GetValue()?TRUE:FALSE;

    switch(frame->id)
    {
    case wxClipButtonConfigA:
        frame->pConfig->Write(_T("/A/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/A/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/A/AsFile"), asFile);
        break;
    case wxClipButtonConfigB:
        frame->pConfig->Write(_T("/B/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/B/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/B/AsFile"), asFile);
        break;
    case wxClipButtonConfigC:
        frame->pConfig->Write(_T("/C/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/C/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/C/AsFile"), asFile);
        break;
    case wxClipButtonConfigD:
        frame->pConfig->Write(_T("/D/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/D/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/D/AsFile"), asFile);
        break;
    case wxClipButtonConfigE:
        frame->pConfig->Write(_T("/E/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/E/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/E/AsFile"), asFile);
        break;
    case wxClipButtonConfigF:
        frame->pConfig->Write(_T("/F/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/F/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/F/AsFile"), asFile);
        break;
    case wxClipButtonConfigG:
        frame->pConfig->Write(_T("/G/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/G/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/G/AsFile"), asFile);
        break;
    case wxClipButtonConfigH:
        frame->pConfig->Write(_T("/H/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/H/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/H/AsFile"), asFile);
        break;
    case wxClipButtonConfigI:
        frame->pConfig->Write(_T("/I/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/I/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/I/AsFile"), asFile);
        break;
    case wxClipButtonConfigJ:
        frame->pConfig->Write(_T("/J/Label"), String_Label.Left(wxClip_MaxLabel));
        frame->pConfig->Write(_T("/J/Text"),  String_Text.Left(wxClip_MaxText));
        frame->pConfig->Write(_T("/J/AsFile"), asFile);
        break;
    default:
        wxASSERT(wxClipButtonConfigSentinal >= frame->id);
        break;
    }

    EndModal(wxID_OK);
}


void MyDialog::OnCancel(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}


void MyDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
}


bool MyDialog::TransferDataToWindow(void)
{
    wxString str;
    bool     asFile = FALSE;

    switch(frame->id)
    {
    case wxClipButtonConfigA:
        if(TRUE != frame->pConfig->Read(_T("/A/Label"), &String_Label)) String_Label = _T("A");
        frame->pConfig->Read(_T("/A/Text"),  &String_Text);
        frame->pConfig->Read(_T("/A/AsFile"), &asFile);
        break;
    case wxClipButtonConfigB:
        if(TRUE != frame->pConfig->Read(_T("/B/Label"), &String_Label)) String_Label = _T("B");
        frame->pConfig->Read(_T("/B/Text"),  &String_Text);
        frame->pConfig->Read(_T("/B/AsFile"), &asFile);
        break;
    case wxClipButtonConfigC:
        if(TRUE != frame->pConfig->Read(_T("/C/Label"), &String_Label)) String_Label = _T("C");
        frame->pConfig->Read(_T("/C/Text"),  &String_Text);
        frame->pConfig->Read(_T("/C/AsFile"), &asFile);
        break;
    case wxClipButtonConfigD:
        if(TRUE != frame->pConfig->Read(_T("/D/Label"), &String_Label)) String_Label = _T("D");
        frame->pConfig->Read(_T("/D/Text"),  &String_Text);
        frame->pConfig->Read(_T("/D/AsFile"), &asFile);
        break;
    case wxClipButtonConfigE:
        if(TRUE != frame->pConfig->Read(_T("/E/Label"), &String_Label)) String_Label = _T("E");
        frame->pConfig->Read(_T("/E/Text"),  &String_Text);
        frame->pConfig->Read(_T("/E/AsFile"), &asFile);
        break;
    case wxClipButtonConfigF:
        if(TRUE != frame->pConfig->Read(_T("/F/Label"), &String_Label)) String_Label = _T("F");
        frame->pConfig->Read(_T("/F/Text"),  &String_Text);
        frame->pConfig->Read(_T("/F/AsFile"), &asFile);
        break;
    case wxClipButtonConfigG:
        if(TRUE != frame->pConfig->Read(_T("/G/Label"), &String_Label)) String_Label = _T("G");
        frame->pConfig->Read(_T("/G/Text"),  &String_Text);
        frame->pConfig->Read(_T("/G/AsFile"), &asFile);
        break;
    case wxClipButtonConfigH:
        if(TRUE != frame->pConfig->Read(_T("/H/Label"), &String_Label)) String_Label = _T("H");
        frame->pConfig->Read(_T("/H/Text"),  &String_Text);
        frame->pConfig->Read(_T("/H/AsFile"), &asFile);
        break;
    case wxClipButtonConfigI:
        if(TRUE != frame->pConfig->Read(_T("/I/Label"), &String_Label)) String_Label = _T("I");
        frame->pConfig->Read(_T("/I/Text"),  &String_Text);
        frame->pConfig->Read(_T("/I/AsFile"), &asFile);
        break;
    case wxClipButtonConfigJ:
        if(TRUE != frame->pConfig->Read(_T("/J/Label"), &String_Label)) String_Label = _T("J");
        frame->pConfig->Read(_T("/J/Text"),  &String_Text);
        frame->pConfig->Read(_T("/J/AsFile"), &asFile);
        break;
    default:
        wxASSERT(wxClipButtonConfigSentinal >= frame->id);
        break;
    }

    TextCtrl_Label->SetValue(String_Label);
    TextCtrl_Text->SetValue(String_Text);

    checkbox->SetValue(asFile);

    return TRUE;
}

bool MyDialog::TransferDataFromWindow(void)
{
    wxString str;

    wxMessageBox(_T("TransferDataFromWindow"), _T("Got Here"), wxICON_HAND | wxOK, this);

    return TRUE;
}



