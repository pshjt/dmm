// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#pragma once

class TransparentStaticText : public wxStaticText
{
public:
	TransparentStaticText(
		wxWindow* parent,
		wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxStaticTextNameStr
	)
		: wxStaticText(parent, id, label, pos, size, style | wxTRANSPARENT_WINDOW, name)
	{
	}

	// ~TransparentStaticText() {}

	virtual WXHBRUSH MSWControlColor(WXHDC pDC, WXHWND WXUNUSED(hWnd))
	{
		wxColour fgColour = GetForegroundColour();
		::SetTextColor((HDC)pDC, RGB(fgColour.Red(), fgColour.Green(), fgColour.Blue()));
		::SetBkMode((HDC)pDC, TRANSPARENT);

		return (WXHBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
};