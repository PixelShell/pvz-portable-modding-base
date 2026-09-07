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
