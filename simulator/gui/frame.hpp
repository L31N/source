#ifndef FRAME_HPP
#define FRAME_HPP

#include <wx/wx.h>

class Frame : public wxFrame
{
	public:
		Frame();
		~Frame();
	private:
        wxPanel* panelField;

		wxButton* buttonInc;

        wxPoint* botPosition;

        bool mouseDown;

        void setBot(wxMouseEvent& event);

        void renderField(wxPaintEvent& event);
        void onMouseMove(wxMouseEvent& event);
        void onMouseLeftDown(wxMouseEvent& event);
        void onMouseLeftUp(wxMouseEvent& event);
};

#endif
