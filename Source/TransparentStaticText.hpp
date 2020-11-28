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

	WXHBRUSH MSWControlColor(WXHDC pDC, WXHWND WXUNUSED(hWnd)) override
	{
		wxColour fgColour = GetForegroundColour();
		SetTextColor(static_cast<HDC>(pDC), RGB(fgColour.Red(), fgColour.Green(), fgColour.Blue()));
		SetBkMode(static_cast<HDC>(pDC), TRANSPARENT);

		return static_cast<WXHBRUSH>(GetStockObject(HOLLOW_BRUSH));
	}
};