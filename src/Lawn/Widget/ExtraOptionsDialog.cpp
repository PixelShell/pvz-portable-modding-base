/*
 * Copyright (C) 2026 Zhou Qiankang <wszqkzqk@qq.com>
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * This file is part of PvZ-Portable.
 *
 * PvZ-Portable is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PvZ-Portable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with PvZ-Portable. If not, see <https://www.gnu.org/licenses/>.
 */

#include "../Board.h"
#include "GameButton.h"
#include "../Cutscene.h"
#include "AlmanacDialog.h"
#include "../LawnCommon.h"
#include "../../LawnApp.h"
#include "../System/Music.h"
#include "../../Resources.h"
#include "ExtraOptionsDialog.h"
#include "../../ConstEnums.h"
#include "widget/Checkbox.h"

using namespace Sexy;

ExtraOptionsDialog::ExtraOptionsDialog(LawnApp* theApp, bool theFromGameSelector) :
	Dialog(nullptr, nullptr, Dialogs::DIALOG_NEWOPTIONS, true, "Options", "", "", Dialog::BUTTONS_NONE)
{
	mApp = theApp;
	mFromGameSelector = theFromGameSelector;
	SetColor(Dialog::COLOR_BUTTON_TEXT, Color(255, 255, 100));
	
	mDebugCheckbox = MakeNewCheckbox(ExtraOptionsDialog::ExtraOptionsDialog_Debug, this, theApp->mDebugKeysEnabled);
	mAutoCollectionCheckbox = MakeNewCheckbox(ExtraOptionsDialog::ExtraOptionsDialog_AutoCollect, this, theApp->mAutoCollect);
	mHealthbarCheckbox = MakeNewCheckbox(ExtraOptionsDialog::ExtraOptionsDialog_Healthbar, this, theApp->mHealthbarEnabled);
	mBackButton = MakeNewButton(
		ExtraOptionsDialog::ExtraOptionsDialog_Back,
		this,
		"[BACK]",
		nullptr,
		IMAGE_OPTIONS_BACKTOGAMEBUTTON0,
		IMAGE_OPTIONS_BACKTOGAMEBUTTON0,
		IMAGE_OPTIONS_BACKTOGAMEBUTTON2
	);
	mBackButton->mTranslateX = 0;
	mBackButton->mTranslateY = 0;
	mBackButton->mTextOffsetX = -2;
	mBackButton->mTextOffsetY = -5;
	mBackButton->mTextDownOffsetX = 0;
	mBackButton->mTextDownOffsetY = 1;
	mBackButton->SetFont(FONT_DWARVENTODCRAFT36GREENINSET);
	mBackButton->SetColor(ButtonWidget::COLOR_LABEL, Color::White);
	mBackButton->SetColor(ButtonWidget::COLOR_LABEL_HILITE, Color::White);
	mBackButton->mHiliteFont = FONT_DWARVENTODCRAFT36BRIGHTGREENINSET;
	
}

ExtraOptionsDialog::~ExtraOptionsDialog()
{
	delete mBackButton;
	delete mDebugCheckbox;
	delete mAutoCollectionCheckbox;
	delete mHealthbarCheckbox;
}

int ExtraOptionsDialog::GetPreferredHeight(int theWidth)
{
	(void)theWidth;
	return IMAGE_OPTIONS_MENUBACK->mWidth;
}

void ExtraOptionsDialog::AddedToManager(Sexy::WidgetManager* theWidgetManager)
{
	Dialog::AddedToManager(theWidgetManager);
	AddWidget(mBackButton);
	AddWidget(mDebugCheckbox);
	AddWidget(mAutoCollectionCheckbox);
	AddWidget(mHealthbarCheckbox);
}

void ExtraOptionsDialog::RemovedFromManager(Sexy::WidgetManager* theWidgetManager)
{
	Dialog::RemovedFromManager(theWidgetManager);
	RemoveWidget(mBackButton);
	RemoveWidget(mDebugCheckbox);
	RemoveWidget(mAutoCollectionCheckbox);
	RemoveWidget(mHealthbarCheckbox);
}

void ExtraOptionsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Dialog::Resize(theX, theY, theWidth, theHeight);
	mDebugCheckbox->Resize(100, 186, 135, 40);
	mAutoCollectionCheckbox->Resize(mDebugCheckbox->mX, mDebugCheckbox->mY + 45, 135, 40);
	mHealthbarCheckbox->Resize(mDebugCheckbox->mX, mAutoCollectionCheckbox->mY + 45, 135, 40);
	mBackButton->Resize(30, 381, mBackButton->mWidth, mBackButton->mHeight);
}

void ExtraOptionsDialog::Draw(Sexy::Graphics* g)
{
	g->DrawImage(IMAGE_OPTIONS_MENUBACK, 0, 0);

	Sexy::Color aTextColor(107, 109, 145);

	int aSliderLabelsX = mApp->GetInteger("OPTION_DLG_SLIDER_LABELS_OFFSET_X", 186);
	float aFontScale = static_cast<float>(mApp->GetDouble("OPTION_DLG_LABEL_FONT_SCALE", 1.0));
	if (aFontScale != 1.0f)
		g->SetScale(aFontScale, aFontScale, 0.0f, 0.0f);

	PvzpDrawString(g, mApp->GetString("OPTIONS_DEBUG", "Debug Mode"), mDebugCheckbox->mX+45, mDebugCheckbox->mY+25, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	PvzpDrawString(g, mApp->GetString("OPTIONS_AUTOCOLLECTION", "Sun & Coin Auto"), mAutoCollectionCheckbox->mX+45, mAutoCollectionCheckbox->mY+15, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	PvzpDrawString(g, mApp->GetString("OPTIONS_AUTOCOLLECTION", "Collection"), mAutoCollectionCheckbox->mX+45, mAutoCollectionCheckbox->mY+35, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);
	PvzpDrawString(g, mApp->GetString("OPTIONS_HEALTHBAR", "Healthbars"), mHealthbarCheckbox->mX+45, mHealthbarCheckbox->mY+25, FONT_DWARVENTODCRAFT18, aTextColor, DrawStringJustification::DS_ALIGN_LEFT);

	if (aFontScale != 1.0f)
		g->SetScale(1.0f, 1.0f, 0.0f, 0.0f);
}

void ExtraOptionsDialog::SliderVal(int theId, double theVal)
{
}

void ExtraOptionsDialog::CheckboxChecked(int theId, bool checked)
{
	switch (theId)
	{
		case ExtraOptionsDialog::ExtraOptionsDialog_Debug:
			mApp->mDebugKeysEnabled = !mApp->mDebugKeysEnabled;
			mApp->mCheatKeys = !mApp->mCheatKeys;
			break;

		case ExtraOptionsDialog::ExtraOptionsDialog_AutoCollect:
			mApp->mAutoCollect = !mApp->mAutoCollect;
			break;
		case ExtraOptionsDialog::ExtraOptionsDialog_Healthbar:
			mApp->mHealthbarEnabled = !mApp->mHealthbarEnabled;
			break;
	}
}

void ExtraOptionsDialog::KeyDown(Sexy::KeyCode theKey)
{
	if (mApp->mBoard)
	{
		mApp->mBoard->DoTypingCheck(theKey);
	}

	if (theKey == KeyCode::KEYCODE_SPACE || theKey == KeyCode::KEYCODE_RETURN)
	{
		Dialog::ButtonDepress(Dialog::ID_OK);
	}
	else if (theKey == KeyCode::KEYCODE_ESCAPE)
	{
		Dialog::ButtonDepress(Dialog::ID_CANCEL);
	}
}

void ExtraOptionsDialog::ButtonPress(int theId)
{
	(void)theId;
	mApp->PlaySample(SOUND_GRAVEBUTTON);
}

void ExtraOptionsDialog::ButtonDepress(int theId)
{
	Dialog::ButtonDepress(theId);

	switch (theId)
	{
	case (ExtraOptionsDialog::ExtraOptionsDialog_Back):
		mApp->KillDialog(Dialogs::DIALOG_EXTRAOPTIONS);
		mApp->DoNewOptions(mFromGameSelector);
		break;
	}
}
