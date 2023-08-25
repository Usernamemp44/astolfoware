#include "Styles.h"
#include "CDraw.h"
#include "CDrawManager.h"

int DefaultStyle::ControlPanel(int x, int y, int w, int h, const char* name)
{
	gDraw.OutlineRect(x, y, w, h, SColor(50, 50, 55));

	int txtMargin = 0;

	int textW = 0, textH = 15;
	if (name)
		gInts.Surface->GetTextSize(gFonts.calibri_light_small, ToWchar(name).c_str(), textW, textH);
	gDraw.DrawRect(x + 1, y + 1, w - 2, textH + txtMargin, SColor(25));
	if (name)
		gDraw.DrawString(x + ((w / 2) - (textW / 2)), y, SColor(150), name, gFonts.calibri_light_small);

	gDraw.DrawLine(x, y + textH + txtMargin, x + w, y + textH + txtMargin, SColor(50, 50, 55));
	x += 1, y += textH + txtMargin + 1, w -= 2, h -= textH + txtMargin + 2;

	gDraw.DrawRect(x, y, w, h, SColor(30, 30, 33));

	return textH + txtMargin + 1;
}

int DefaultStyle::DialogButton(int x, int y, int w, const char* text, bool mouseOver)
{
	gDraw.DrawRect(x, y, w, 15, SColor(42, 42, 48));
	gDraw.OutlineRect(x, y, w, 15, SColor(60));

	gDraw.DrawString(x + 3, y, mouseOver ? SColor(90, 150, 225) : SColor(125, 125, 130), text, gFonts.verdana_bold);

	return 15;
}

void DefaultStyle::Dialog(int x, int y, int w, int h)
{
	gDraw.OutlineRect(x, y, w, h, SColor(58, 58, 70));
	gDraw.DrawRect(x + 1, y + 1, w - 2, h - 2, SColor(36, 36, 40));
}

#define TOPBAR 25
int DefaultStyle::TopBar(int x, int y, int w, const char* title)
{
	// Dark topbar
	gDraw.DrawRect(x, y, w, TOPBAR, SColor(32, 34, 37));

	if (title)
		gDraw.DrawString(x + 10, y, SColor(120), title, gFonts.segoe);

	return TOPBAR;
}

void WindowStyle::WaterMark(const char* name, int x, int y, bool background)
{
	static float hue = 0;

	static SColor red(255, 105, 180);
	static SColor light(200);

	int tw = 0, th = 0;
	gInts.Surface->GetTextSize(gFonts.cambria, ToWchar("Astolfoware").c_str(), tw, th);

	gDrawManager.DrawString(x + 14, y + 4 + 2 * 30, Color::Rainbow(), "Astolfoware", gFonts.vgasyst);
	gDrawManager.DrawString(x + 14, y + 4 + 2 * 50, Color::Rainbow(), "Step up the game with a boykisser cheat :3", gFonts.vgasyst);
}

void WindowStyle::About(int x, int y, int w, int h)
{
	int CenterX = x + (w / 2);

	int dcHeight, dcWidth, iHeight, iWidth;
	gInts.Surface->GetTextSize(gFonts.arial, L"i like to paste :3 -Reality\n", iWidth, iHeight);
	gDraw.DrawString(CenterX - (iWidth / 2), y + 5, SColor(160), L"Credits: Reality, Username.mp4, and all of the contributers!\n");

	gInts.Surface->GetTextSize(gFonts.calibri_light, L"Astolfoware is a NaCL paste, but made by a boykisser! :3", dcWidth, dcHeight);
}