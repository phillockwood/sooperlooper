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

#include <wx/wx.h>
#include <wx/file.h>
#include <wx/filename.h>

#include <iostream>

#include "looper_panel.hpp"
#include "pix_button.hpp"
#include "loop_control.hpp"
#include "time_panel.hpp"
#include "slider_bar.hpp"

using namespace SooperLooperGui;
using namespace std;


enum {
	ID_UndoButton = 8000,
	ID_RedoButton,
	ID_RecordButton,
	ID_OverdubButton,
	ID_MultiplyButton,
	ID_InsertButton,
	ID_ReplaceButton,
	ID_TapButton,
	ID_ReverseButton,
	ID_MuteButton,
	ID_RateButton,

	ID_ThreshControl,
	ID_FeedbackControl,
	ID_DryControl,
	ID_WetControl,
	ID_ScratchControl,
	ID_RateControl,

	ID_QuantizeCheck,
	ID_RoundCheck
      


};

BEGIN_EVENT_TABLE(LooperPanel, wxPanel)
// 	EVT_COMMAND_SCROLL (ID_ThreshControl, LooperPanel::slider_events)
// 	EVT_COMMAND_SCROLL (ID_FeedbackControl, LooperPanel::slider_events)
// 	EVT_COMMAND_SCROLL (ID_DryControl, LooperPanel::slider_events)
// 	EVT_COMMAND_SCROLL (ID_WetControl, LooperPanel::slider_events)
// 	EVT_COMMAND_SCROLL (ID_ScratchControl, LooperPanel::slider_events)
// 	EVT_COMMAND_SCROLL (ID_RateControl, LooperPanel::slider_events)

	EVT_CHECKBOX (ID_QuantizeCheck, LooperPanel::check_events)
	EVT_CHECKBOX (ID_RoundCheck, LooperPanel::check_events)
	
END_EVENT_TABLE()

LooperPanel::LooperPanel(LoopControl * control, wxWindow * parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
	: wxPanel(parent, id, pos, size), _loop_control(control), _index(0)
{
	init();
}

LooperPanel::~LooperPanel()
{

}


void
LooperPanel::init()
{
	SetBackgroundColour (*wxBLACK);
	SetThemeEnabled(false);

	wxBoxSizer * mainSizer = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer * colsizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * rowsizer;
	PixButton * bitbutt;

 	_undo_button = bitbutt = new PixButton(this, ID_UndoButton);
	load_bitmaps (bitbutt, wxT("undo"));
 	colsizer->Add (bitbutt, 0, wxTOP, 5);

 	_redo_button = bitbutt = new PixButton(this, ID_RedoButton);
	load_bitmaps (bitbutt, wxT("redo"));
 	colsizer->Add (bitbutt, 0, wxTOP, 5);
	
	mainSizer->Add (colsizer, 0, wxEXPAND|wxBOTTOM|wxLEFT, 5);

	
	colsizer = new wxBoxSizer(wxVERTICAL);

 	_record_button = bitbutt = new PixButton(this, ID_RecordButton);
	load_bitmaps (bitbutt, wxT("record"));
	
 	colsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);

 	_overdub_button = bitbutt = new PixButton(this, ID_OverdubButton);
	load_bitmaps (bitbutt, wxT("overdub"));
 	colsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);

 	_multiply_button = bitbutt = new PixButton(this, ID_MultiplyButton);
	load_bitmaps (bitbutt, wxT("multiply"));
 	colsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);
	
	mainSizer->Add (colsizer, 0, wxEXPAND|wxBOTTOM, 5);


	colsizer = new wxBoxSizer(wxVERTICAL);
	rowsizer = new wxBoxSizer(wxHORIZONTAL);

	SliderBar *slider;
	wxFont sliderFont = *wxSMALL_FONT;
	
	_thresh_control = slider = new SliderBar(this, ID_ThreshControl, 0.0f, 100.0f, 0.0f);
	slider->set_units(wxT("%"));
	slider->set_label(wxT("rec thresh"));
	slider->SetFont(sliderFont);
	slider->value_changed.connect (bind (slot (*this, &LooperPanel::slider_events), (int) slider->GetId()));
	colsizer->Add (slider, 0, wxEXPAND|wxTOP|wxLEFT, 5);

	_feedback_control = slider = new SliderBar(this, ID_FeedbackControl, 0.0f, 100.0f, 0.0f);
	slider->set_units(wxT("%"));
	slider->set_label(wxT("feedback"));
	slider->SetFont(sliderFont);
	slider->value_changed.connect (bind (slot (*this, &LooperPanel::slider_events), (int) slider->GetId()));
	colsizer->Add (slider, 0, wxEXPAND|wxTOP|wxLEFT, 5);

	colsizer->Add (20, 5, 0, wxEXPAND);
	
 	_replace_button = bitbutt = new PixButton(this, ID_ReplaceButton);
	load_bitmaps (bitbutt, wxT("replace"));
 	rowsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);

 	_tap_button = bitbutt = new PixButton(this, ID_TapButton);
	load_bitmaps (bitbutt, wxT("tap"));
 	rowsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);

	colsizer->Add (rowsizer, 0);

	rowsizer = new wxBoxSizer(wxHORIZONTAL);
	
 	_insert_button = bitbutt = new PixButton(this, ID_InsertButton);
	load_bitmaps (bitbutt, wxT("insert"));
 	rowsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);

 	_reverse_button = bitbutt = new PixButton(this, ID_ReverseButton);
	load_bitmaps (bitbutt, wxT("reverse"));
 	rowsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);

	colsizer->Add (rowsizer, 0);
	
	mainSizer->Add (colsizer, 0, wxEXPAND|wxBOTTOM, 5);
	

	// time area
	colsizer = new wxBoxSizer(wxVERTICAL);

	_time_panel = new TimePanel(_loop_control, this, -1, wxDefaultPosition, wxSize(210, 60));
	_time_panel->set_index (_index);
	
	colsizer->Add (_time_panel, 0, wxLEFT|wxTOP, 5);

	colsizer->Add (20, -1, 1);

	_dry_control = slider = new SliderBar(this, ID_DryControl, 0.0f, 100.0f, 0.0f);
	slider->set_units(wxT("%"));
	slider->set_label(wxT("dry"));
	slider->SetFont(sliderFont);
	slider->value_changed.connect (bind (slot (*this, &LooperPanel::slider_events), (int) slider->GetId()));
	colsizer->Add (slider, 0, wxEXPAND|wxTOP|wxLEFT, 4);

	_wet_control = slider = new SliderBar(this, ID_WetControl, 0.0f, 100.0f, 0.0f);
	slider->set_units(wxT("%"));
	slider->set_label(wxT("wet"));
	slider->SetFont(sliderFont);
	slider->value_changed.connect (bind (slot (*this, &LooperPanel::slider_events), (int) slider->GetId()));
	colsizer->Add (slider, 0, wxEXPAND|wxTOP|wxLEFT, 4);
	
	
	mainSizer->Add (colsizer, 0, wxEXPAND|wxBOTTOM, 5);


	//

	colsizer = new wxBoxSizer(wxVERTICAL);
	rowsizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * lilcolsizer = new wxBoxSizer(wxVERTICAL);
	
	_quantize_check = new wxCheckBox(this, ID_QuantizeCheck, "quantize");
	_quantize_check->SetFont(sliderFont);
	_quantize_check->SetBackgroundColour(wxColour(90,90,90));
	_quantize_check->SetForegroundColour(*wxWHITE);
	lilcolsizer->Add (_quantize_check, 0, wxEXPAND);

	_round_check = new wxCheckBox(this, ID_RoundCheck, "round");
	_round_check->SetFont(sliderFont);
	_round_check->SetBackgroundColour(wxColour(90,90,90));
	_round_check->SetForegroundColour(*wxWHITE);
	lilcolsizer->Add (_round_check, 0, wxEXPAND);

	rowsizer->Add(lilcolsizer, 1, wxTOP|wxLEFT, 5);

 	_mute_button = bitbutt = new PixButton(this, ID_MuteButton);
	load_bitmaps (bitbutt, wxT("mute"));
 	rowsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);
	
	colsizer->Add (rowsizer, 0, wxEXPAND);

	colsizer->Add (20,-1, 1);
	
	// rate stuff
	rowsizer = new wxBoxSizer(wxHORIZONTAL);
 	_rate_button = bitbutt = new PixButton(this, ID_RateButton);
	load_bitmaps (bitbutt, wxT("rate"));
 	rowsizer->Add (bitbutt, 0, wxTOP|wxLEFT, 5);

	// rate control
	_rate_control = slider = new SliderBar(this, ID_RateControl, -4.0f, 4.0f, 1.0f);
	slider->set_units(wxT(""));
	slider->set_label(wxT("rate"));
	slider->set_style (SliderBar::CenterStyle);
	slider->SetFont(sliderFont);
	slider->value_changed.connect (bind (slot (*this, &LooperPanel::slider_events), (int) slider->GetId()));
	rowsizer->Add (slider, 1, wxEXPAND|wxTOP|wxLEFT, 5);
	
	colsizer->Add (rowsizer, 0, wxEXPAND);

	
	_scratch_control = slider = new SliderBar(this, ID_ScratchControl, 0.0f, 1.0f, 0.0f);
	slider->set_units(wxT(""));
	slider->set_label(wxT("scratch"));
	slider->set_style (SliderBar::CenterStyle);
	slider->SetFont(sliderFont);
	slider->value_changed.connect (bind (slot (*this, &LooperPanel::slider_events), (int) slider->GetId()));
	colsizer->Add (slider, 0, wxEXPAND|wxTOP|wxLEFT, 5);
	
	mainSizer->Add (colsizer, 1, wxEXPAND|wxBOTTOM|wxRIGHT, 5);

	
	bind_events();
	
	
	this->SetAutoLayout( true );     // tell dialog to use sizer
	this->SetSizer( mainSizer );      // actually set the sizer
	mainSizer->Fit( this );            // set size to minimum size as calculated by the sizer
	mainSizer->SetSizeHints( this );   // set size hints to honour mininum size

}


void
LooperPanel::bind_events()
{
	_undo_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("undo")));
	_undo_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("undo")));

	_redo_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("redo")));
	_redo_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("redo")));

	_record_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("record")));
	_record_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("record")));

	_overdub_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("overdub")));
	_overdub_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("overdub")));

	_multiply_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("multiply")));
	_multiply_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("multiply")));

	_replace_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("replace")));
	_replace_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("replace")));

	_insert_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("insert")));
	_insert_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("insert")));

	_tap_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("tap")));
	_tap_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("tap")));

	_reverse_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("reverse")));
	_reverse_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("reverse")));

	_mute_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("mute")));
	_mute_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("mute")));

	_rate_button->pressed.connect (bind (slot (*this, &LooperPanel::pressed_events), wxString("rate")));
	_rate_button->released.connect (bind (slot (*this, &LooperPanel::released_events), wxString("rate")));
	
}

wxString
LooperPanel::get_pixmap_path (const wxString & namebase)
{
	wxString filename;
	
	if (wxFile::Exists(wxString::Format("%s%s", PIXMAPDIR, namebase.c_str()))) {
		filename = wxString::Format("%s%s", PIXMAPDIR, namebase.c_str());
	}
	else if (wxFile::Exists(wxString::Format("pixmaps%c%s", wxFileName::GetPathSeparator(), namebase.c_str()))) {
		filename = wxString::Format("pixmaps%c%s", wxFileName::GetPathSeparator(), namebase.c_str());
	}
	else if (wxFile::Exists (namebase)) {
		filename = namebase;
	}
	
	return filename;
}

bool
LooperPanel::load_bitmaps (PixButton * butt, wxString namebase)
{
	
	butt->set_normal_bitmap (wxBitmap(get_pixmap_path(namebase + wxT("_normal.xpm"))));
 	butt->set_selected_bitmap (wxBitmap(get_pixmap_path(namebase + wxT("_selected.xpm"))));
 	butt->set_focus_bitmap (wxBitmap(get_pixmap_path(namebase + wxT("_focus.xpm"))));
 	butt->set_disabled_bitmap (wxBitmap(get_pixmap_path(namebase + wxT("_disabled.xpm"))));

	return true;
}


void
LooperPanel::update_controls()
{
	// get recent controls from loop control
	float val;
	
	if (_loop_control->is_updated(_index, "feedback")) {
		_loop_control->get_value(_index, "feedback", val);
		_feedback_control->set_value ((val * 100.0f));
	}
	if (_loop_control->is_updated(_index, "rec_thresh")) {
		_loop_control->get_value(_index, "rec_thresh", val);
		_thresh_control->set_value ((val * 100.0f));
	}
	if (_loop_control->is_updated(_index, "dry")) {
		_loop_control->get_value(_index, "dry", val);
		_dry_control->set_value ((val * 100.0f));
	}
	if (_loop_control->is_updated(_index, "wet")) {
		_loop_control->get_value(_index, "wet", val);
		_wet_control->set_value (val * 100.0f);
	}
	if (_loop_control->is_updated(_index, "rate")) {
		_loop_control->get_value(_index, "rate", val);
		_rate_control->set_value (val);
	}
	if (_loop_control->is_updated(_index, "scratch_pos")) {
		_loop_control->get_value(_index, "scratch_pos", val);
		_scratch_control->set_value (val);
	}
	if (_loop_control->is_updated(_index, "quantize")) {
		_loop_control->get_value(_index, "quantize", val);
		_quantize_check->SetValue (val > 0.0);
	}
	if (_loop_control->is_updated(_index, "round")) {
		_loop_control->get_value(_index, "round", val);
		_round_check->SetValue (val > 0.0);
	}

	if (_time_panel->update_time()) {
		_time_panel->Refresh(true);
	}
}


void
LooperPanel::pressed_events (wxString cmd)
{
	_loop_control->post_down_event (_index, cmd);
}

void
LooperPanel::released_events (wxString cmd)
{
	_loop_control->post_up_event (_index, cmd);

}

void
LooperPanel::slider_events(float val, int id)
{
	wxString ctrl;
	
	switch(id)
	{
	case ID_ThreshControl:
		ctrl = "rec_thresh";
		val = val / 100.0f;
		break;
	case ID_FeedbackControl:
		ctrl = "feedback";
		val = _feedback_control->get_value() / 100.0f;
		break;
	case ID_DryControl:
		ctrl = "dry";
		val = _dry_control->get_value() / 100.0f;
		break;
	case ID_WetControl:
		ctrl = "wet";
		val = _wet_control->get_value() / 100.0f;
		break;
	case ID_ScratchControl:
		ctrl = "scratch_pos";
		val = _scratch_control->get_value();
		break;
	case ID_RateControl:
		ctrl = "rate";
		val = (_rate_control->get_value() / 100.0f) * 4.0f;
		break;
	default:
		break;
	}

	if (!ctrl.empty()) {
		post_control_event (ctrl, val);
	}
}

void
LooperPanel::check_events(wxCommandEvent &ev)
{
	int id = ev.GetId();

	switch (id) {
	case ID_RoundCheck:
		post_control_event ("round", _round_check->GetValue() ? 1.0f: 0.0f);
		break;
	case ID_QuantizeCheck:
		post_control_event ("quantize", _quantize_check->GetValue() ? 1.0f: 0.0f);
		break;
	default:
		break;
	}

}

void
LooperPanel::post_control_event (wxString ctrl, float val)
{
	_loop_control->post_ctrl_change (_index, ctrl, val);
}
