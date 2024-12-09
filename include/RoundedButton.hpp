#ifndef ROUNDEDBUTTON_HPP
#define ROUNDEDBUTTON_HPP

#include <wx/wx.h>
#include <wx/log.h>
#include <wx/file.h>
#include <wx/txtstrm.h>
#include <wx/textfile.h>
#include <wx/dcclient.h>

// Define the RoundedButton class
class RoundedButton : public wxPanel {
public:
    RoundedButton(wxWindow* parent, int id, const wxString& label, const wxColour& nTextColor = wxColour("#dedede"), 
                  const wxColour& hTextColor = wxColour("#656565"), const wxSize& size = wxSize(370, 40))
        : wxPanel(parent, id, wxDefaultPosition, size, wxBORDER_NONE), label(label), normalTextColor(nTextColor), 
          hoverTextColor(hTextColor), normalBgColor(wxColour("#757575")), hoverBgColor(wxColour("#aaaaaa")), 
          normalBorderColor(wxColour("#aaaaaa")), hoverBorderColor(wxColour("#ffffff")), isHovered(false) {
        Bind(wxEVT_PAINT, &RoundedButton::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &RoundedButton::OnClick, this);
        Bind(wxEVT_ENTER_WINDOW, &RoundedButton::OnMouseEnter, this);  // Handle mouse enter
        Bind(wxEVT_LEAVE_WINDOW, &RoundedButton::OnMouseLeave, this);  // Handle mouse leave
    }

private:
    wxString label;
    wxColour normalTextColor;       // Default text color
    wxColour hoverTextColor;        // Hover text color
    wxColour normalBgColor;         // Default background color
    wxColour hoverBgColor;          // Hover background color
    wxColour normalBorderColor;     // Default border color
    wxColour hoverBorderColor;      // Hover border color
    bool isHovered;                 // Track hover state

    void OnPaint(wxPaintEvent& event) {
        wxPaintDC dc(this);

        // Set rounded rectangle background and border
        wxBrush brush(isHovered ? hoverBgColor : normalBgColor);  // Background color based on hover state
        wxPen pen(isHovered ? hoverBorderColor : normalBorderColor, 1);  // Border color based on hover state with line thickness 1
        dc.SetBrush(brush);
        dc.SetPen(pen);
        dc.DrawRoundedRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight(), 12);  // Rounded corners radius: 12

        // Draw button text
        dc.SetTextForeground(isHovered ? hoverTextColor : normalTextColor);  // Text color based on hover state
        dc.SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        wxSize textSize = dc.GetTextExtent(label);
        dc.DrawText(label, (GetSize().GetWidth() - textSize.GetWidth()) / 2,
                    (GetSize().GetHeight() - textSize.GetHeight()) / 2);
    }

    void OnClick(wxMouseEvent& event) {
        wxCommandEvent clickEvent(wxEVT_BUTTON, GetId());
        clickEvent.SetEventObject(this);
        GetParent()->ProcessWindowEvent(clickEvent);  // Forward the click event to the parent
    }

    void OnMouseEnter(wxMouseEvent& event) {
        SetCursor(wxCursor(wxCURSOR_HAND));  // Change cursor to hand when hovering
        isHovered = true;  // Update hover state
        Refresh();         // Redraw the button to apply the new color
    }

    void OnMouseLeave(wxMouseEvent& event) {
        SetCursor(wxCursor(wxCURSOR_ARROW));  // Reset cursor to default arrow when leaving
        isHovered = false;  // Update hover state
        Refresh();          // Redraw the button to apply the new color
    }
};

#endif  // ROUNDEDBUTTON_HPP
