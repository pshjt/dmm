#pragma once

class AboutDialog : public wxDialog
{
public:
	AboutDialog(wxWindow* parent);
	// ~AboutDialog() {}

private:
	void hyperlinkOnSetFocus(wxFocusEvent& event);
	void licenseHyperlinkOnHyperlink(wxHyperlinkEvent& event);
	wxStaticText* aboutPanelAddStaticText(wxBoxSizer* boxSizer, std::string text, int flag = 0);
	void switchPanels();
	wxStaticBitmap* computeAboutBitmap();

	wxStaticBitmap* aboutBitmap_;
	wxPanel* aboutPanel_;
	wxPanel* licensePanel_;
	wxTextCtrl* licenseTextCtrl_;
	wxHyperlinkCtrl* licenseHyperlink_;
	wxButton* okButton_;
};