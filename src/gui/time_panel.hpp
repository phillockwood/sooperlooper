/*
** Copyright (C) 2004 Jesse Chappell <jesse@essej.net>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**  
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**  
*/

#ifndef __sooperlooper_gui_time_panel__
#define __sooperlooper_gui_time_panel__


#include <wx/wx.h>


#include <sigc++/sigc++.h>


namespace SooperLooperGui {

class LoopControl;
	
class TimePanel
	: public wxPanel, public SigC::Object
{
  public:
	
	// ctor(s)
	TimePanel (LoopControl * control, wxWindow * parent, wxWindowID id=-1,  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~TimePanel();

	void set_index(int ind) { _index = ind; }
	int get_index() { return _index; }


	void OnSize (wxSizeEvent &ev);
	void OnPaint (wxPaintEvent &ev);

	bool update_time();
	
  protected:

	void init();
	
	void format_time(wxString & timestr, float val);
	void draw_area(wxDC & dc);
	

	LoopControl * _loop_control;

	int _index;

	wxString _pos_str;
	wxString _tot_str;
	wxString _cyc_str;
	wxString _state_str;
	wxString _rem_str;

	wxFont  _pos_font;
	wxColour _pos_color;

	wxFont  _state_font;
	wxColour _state_color;

	wxFont  _time_font;
	wxColour _time_color;

	wxFont  _legend_font;
	wxColour _legend_color;

	int _width, _height;
	
  private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
	
};


};

#endif
