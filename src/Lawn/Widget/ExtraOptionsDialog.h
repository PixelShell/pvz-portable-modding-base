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

#ifndef __EXTRAOPTIONSDIALOG_H__
#define __EXTRAOPTIONSDIALOG_H__

#include "widget/Dialog.h"
#include "widget/SliderListener.h"
#include "widget/CheckboxListener.h"

class LawnApp;
class LawnStoneButton;
class NewLawnButton;
namespace Sexy
{
    class Slider;
    class Checkbox;
};

class ExtraOptionsDialog : public Sexy::Dialog, public Sexy::SliderListener, public Sexy::CheckboxListener
{
protected:
    enum
    {
        ExtraOptionsDialog_Debug,
        ExtraOptionsDialog_AutoCollect,
        ExtraOptionsDialog_Healthbar,
        ExtraOptionsDialog_Back,
    };

public:
    LawnApp*				mApp;
    Sexy::Checkbox*			mDebugCheckbox;
    Sexy::Checkbox*			mAutoCollectionCheckbox;
    Sexy::Checkbox*			mHealthbarCheckbox;
    NewLawnButton*		    mBackButton;
    bool                    mFromGameSelector;
public:
    ExtraOptionsDialog(LawnApp* theApp, bool theFromGameSelector);
    ~ExtraOptionsDialog() override;

    int						GetPreferredHeight(int theWidth) override;
    void					AddedToManager(Sexy::WidgetManager* theWidgetManager) override;
    void					RemovedFromManager(Sexy::WidgetManager* theWidgetManager) override;
    void					Resize(int theX, int theY, int theWidth, int theHeight) override;
    void					Draw(Sexy::Graphics* g) override;
    void					SliderVal(int theId, double theVal) override;
    void					CheckboxChecked(int theId, bool checked) override;
    void					ButtonPress(int theId) override;
    void					ButtonDepress(int theId) override;
    void					KeyDown(Sexy::KeyCode theKey) override;
};

#endif
