// Copyright 2013-2015 bluemess
// This file is part of SS2 Blue Mod Manager, licensed under the MIT License.
// For details, see the License.txt file.

#include <Precompiled.hpp>

#include "ApplicationInfo.hpp"

#include "AboutDialog.hpp"

AboutDialog::AboutDialog(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxEmptyString)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* topBoxSizer1;
	topBoxSizer1 = new wxBoxSizer(wxVERTICAL);

	aboutBitmap_ = computeAboutBitmap();
	topBoxSizer1->Add(aboutBitmap_, 0, wxALIGN_CENTER_HORIZONTAL);

	aboutPanel_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	aboutPanel_->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

	wxBoxSizer* topBoxSizer11;
	topBoxSizer11 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* boxSizer111;
	boxSizer111 = new wxBoxSizer(wxVERTICAL);

	aboutPanelAddStaticText(boxSizer111, ApplicationInfo::name);
	aboutPanelAddStaticText(boxSizer111, "Version " + ApplicationInfo::version);
	aboutPanelAddStaticText(boxSizer111, ApplicationInfo::copyright);
	aboutPanelAddStaticText(boxSizer111, "");

	wxStaticText* st = aboutPanelAddStaticText(boxSizer111, "For everything that's System Shock related visit:",
		wxALIGN_CENTER_HORIZONTAL);

	wxHyperlinkCtrl* hyperlink1 = new wxHyperlinkCtrl(aboutPanel_, wxID_ANY, "www.systemshock.org", "www.systemshock.org",
		wxDefaultPosition, wxSize(-1, st->GetSize().GetHeight()), wxHL_DEFAULT_STYLE);

	hyperlink1->SetFont(st->GetFont());
	hyperlink1->SetHoverColour(hyperlink1->GetNormalColour());
	hyperlink1->SetVisitedColour(hyperlink1->GetNormalColour());

	boxSizer111->Add(hyperlink1, 0, wxALIGN_CENTER_HORIZONTAL);

	topBoxSizer11->Add(boxSizer111, 1, wxALL | wxEXPAND, 8);

	aboutPanel_->SetSizer(topBoxSizer11);
	aboutPanel_->Layout();
	topBoxSizer11->Fit(aboutPanel_);
	topBoxSizer1->Add(aboutPanel_, 1, wxEXPAND);

	licensePanel_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	licensePanel_->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	licensePanel_->Hide();

	wxBoxSizer* topBoxSizer12;
	topBoxSizer12 = new wxBoxSizer(wxVERTICAL);

	licenseTextCtrl_ = new wxTextCtrl(licensePanel_, wxID_ANY, wxEmptyString, wxDefaultPosition,
		wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
	licenseTextCtrl_->SetLabel(wxString(ApplicationInfo::license));
	topBoxSizer12->Add(licenseTextCtrl_, 1, wxALL | wxEXPAND, 8);

	licensePanel_->SetSizer(topBoxSizer12);
	licensePanel_->Layout();
	topBoxSizer12->Fit(licensePanel_);
	topBoxSizer1->Add(licensePanel_, 1, wxEXPAND);

	wxBoxSizer* boxSizer13;
	boxSizer13 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* boxSizer131 = new wxBoxSizer(wxVERTICAL);

	licenseHyperlink_ = new wxHyperlinkCtrl(this, wxID_ANY, "temp", "temp", wxDefaultPosition, wxDefaultSize,
		wxBORDER_NONE | wxHL_ALIGN_LEFT);

	licenseHyperlink_->SetHoverColour(licenseHyperlink_->GetNormalColour());
	licenseHyperlink_->SetVisitedColour(licenseHyperlink_->GetNormalColour());

	boxSizer131->Add(licenseHyperlink_, 0, wxALL | wxEXPAND, 4);

	boxSizer13->Add(boxSizer131, 1, wxEXPAND);

	wxBoxSizer* boxSizer132;
	boxSizer132 = new wxBoxSizer(wxVERTICAL);

	okButton_ = new wxButton(this, wxID_OK, "OK");
	boxSizer132->Add(okButton_, 0, wxALL, 4);

	boxSizer13->Add(boxSizer132, 0, wxEXPAND);

	topBoxSizer1->Add(boxSizer13, 0, wxALL | wxEXPAND, 4);

	SetSizer(topBoxSizer1);
	Layout();
	topBoxSizer1->Fit(this);

	Centre(wxBOTH);

	licensePanel_->SetPosition(aboutPanel_->GetPosition());
	licensePanel_->SetSize(aboutPanel_->GetSize());

	hyperlink1->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(AboutDialog::hyperlinkOnSetFocus), nullptr, this);

	licenseHyperlink_->Connect(wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler(AboutDialog::licenseHyperlinkOnHyperlink),
		nullptr, this);

	licenseHyperlink_->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(AboutDialog::hyperlinkOnSetFocus), nullptr, this);

	aboutPanel_->Hide();
	switchPanels();
}

void AboutDialog::hyperlinkOnSetFocus(wxFocusEvent& event)
{
	okButton_->SetFocus();

	event.Skip();
}

void AboutDialog::licenseHyperlinkOnHyperlink(wxHyperlinkEvent& WXUNUSED(event))
{
	switchPanels();
}

wxStaticText* AboutDialog::aboutPanelAddStaticText(wxBoxSizer* boxSizer, std::string text, int flag /*= 0*/)
{
	wxStaticText* staticText = new wxStaticText(aboutPanel_, wxID_ANY, wxString(text));
	staticText->Wrap(-1);
	boxSizer->Add(staticText, 0, flag);

	return staticText;
}

void AboutDialog::switchPanels()
{
	if (!aboutPanel_->IsShown())
	{
		SetTitle("About");
		licenseHyperlink_->SetLabel("License");
		aboutPanel_->Show();
		licensePanel_->Hide();

		okButton_->SetFocus();
	}
	else
	{
		SetTitle("License");
		licenseHyperlink_->SetLabel("Hide license");
		licensePanel_->Show();
		aboutPanel_->Hide();

		licenseTextCtrl_->SetInsertionPoint(0);
		licenseTextCtrl_->SetFocus();
	}
}

wxStaticBitmap* AboutDialog::computeAboutBitmap()
{
	const wxString label(ApplicationInfo::name);
	const wxColour textColor(0, 0, 0);
	const wxColour outlineColor(40, 250, 250);

	const int marginWidth = 7;
	const int marginHeight = 3;
	const int outlineSize = 1;

	const int maxWidth = 240;
	const int maxHeight = 30;

	wxBitmap aboutBitmap("About", wxBITMAP_TYPE_BMP_RESOURCE);
	aboutBitmap.SetMask(nullptr);

	wxMemoryDC mdc;
	mdc.SelectObject(aboutBitmap);

	wxFont font(1, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	int bitmapWidth, bitmapHeight;
	mdc.GetSize(&bitmapWidth, &bitmapHeight);

	int textMaxWidth = maxWidth - marginWidth - (2 * outlineSize);
	int textMaxHeight = maxHeight - marginHeight - (2 * outlineSize);

	int textWidth = 0;
	int textHeight = 0;
	int fontPointSize = 0;
	while (textWidth <= textMaxWidth && textHeight <= textMaxHeight)
	{
		++fontPointSize;
		font.SetPointSize(fontPointSize + 1);
		mdc.GetTextExtent(label, &textWidth, &textHeight, nullptr, nullptr, &font);
	}

	font.SetPointSize(fontPointSize);
	mdc.SetFont(font);
	mdc.GetTextExtent(label, &textWidth, &textHeight);

	int posX = bitmapWidth - textWidth - (2 * outlineSize) - marginWidth - 1;
	int posY = bitmapHeight - textHeight - (2 * outlineSize) - marginHeight - 1;

	mdc.SetTextForeground(outlineColor);

	for (int y = 0; y <= 2 * outlineSize; ++y)
		for (int x = 0; x <= 2 * outlineSize; ++x)
			mdc.DrawText(label, posX + x, posY + y);

	mdc.SetTextForeground(textColor);
	mdc.DrawText(label, posX + outlineSize, posY + outlineSize);

	return new wxStaticBitmap(this, wxID_ANY, aboutBitmap);
}