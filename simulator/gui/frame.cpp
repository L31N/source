#include "frame.hpp"

Frame::Frame() : wxFrame(0, wxID_ANY, wxT("Counter"))
{

	this->SetBackgroundColour(wxColor(240,240,240));
	this->SetMaxSize(wxSize(410,210));
	this->SetMinSize(wxSize(410,210));

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    panelField = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 400,200 ), wxTAB_TRAVERSAL );
	panelField->SetMinSize( wxSize( 400,200 ) );
	panelField->SetMaxSize( wxSize( 400,200 ) );
    panelField->SetBackgroundColour(wxColor(20,200,0));

	sizer->Add( panelField, 1, wxEXPAND | wxALL, 5 );

	this->SetSizer(sizer);
	this->Layout();

    botPosition = new wxPoint(10,10);



    panelField->Connect( wxEVT_MOTION, wxMouseEventHandler( Frame::onMouseMove ), NULL, this );
    panelField->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( Frame::onMouseLeftDown ), NULL, this );
    panelField->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( Frame::onMouseLeftUp ), NULL, this );

    panelField->Connect( wxEVT_PAINT, wxPaintEventHandler( Frame::renderField ), NULL, this );

}

Frame::~Frame()
{

}


void Frame::renderField(wxPaintEvent& event)
{
    wxPaintDC dc(panelField);
    dc.DrawCircle(botPosition->x,botPosition->y,10);
}

void Frame::setBot(wxMouseEvent& event)
{
    wxPaintDC dc(panelField);
    *botPosition = event.GetLogicalPosition(dc);
    panelField->Refresh();
}


void Frame::onMouseMove(wxMouseEvent& event)
{
    if(mouseDown)
    {
        setBot(event);
    }
}

void Frame::onMouseLeftDown(wxMouseEvent& event)
{
    mouseDown = true;
    setBot(event);
}

void Frame::onMouseLeftUp(wxMouseEvent& event)
{
    mouseDown = false;
    setBot(event);
}

