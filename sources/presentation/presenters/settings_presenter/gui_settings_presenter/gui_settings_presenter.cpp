#include "gui_settings_presenter.h"

// Qt
#include <QDebug>

// Internal
#include "settings_provider.h"

#include "translation_manager.h"

using namespace presentation;

class GuiSettingsPresenter::Impl
{
public:
    domain::TranslationManager manager;
};

GuiSettingsPresenter::GuiSettingsPresenter(QObject* parent):
    BasePresenter(parent),
    d(new Impl())
{}

GuiSettingsPresenter::~GuiSettingsPresenter()
{}

void GuiSettingsPresenter::updateView()
{
    this->setViewProperty(PROPERTY(fullscreen),
                          settings::Provider::value(settings::gui::fullscreen));

    const QStringList& locales = d->manager.avalibleLocales();
    int index = locales.indexOf(d->manager.currentLocale());
    this->setViewProperty(PROPERTY(localeIndex), index);

    this->setViewProperty(PROPERTY(uiSize),
                          settings::Provider::value(settings::gui::uiSize));
    this->setViewProperty(PROPERTY(paletteStyle),
                          settings::Provider::value(settings::gui::paletteStyle));
    this->setViewProperty(PROPERTY(rollInverted),
                          settings::Provider::value(settings::gui::fdRollInverted));
    this->setViewProperty(PROPERTY(speedStep),
                          settings::Provider::value(settings::gui::fdSpeedStep));
    this->setViewProperty(PROPERTY(speedUnits),
                          settings::Provider::value(settings::gui::fdSpeedUnits));
    this->setViewProperty(PROPERTY(altitudeStep),
                          settings::Provider::value(settings::gui::fdAltitudeStep));
    this->setViewProperty(PROPERTY(relativeAltitude),
                          settings::Provider::value(settings::gui::fdRelativeAltitude));

    this->setViewProperty(PROPERTY(changed), false);
}

void GuiSettingsPresenter::save()
{
    settings::Provider::setValue(settings::gui::fullscreen,
                                 this->viewProperty(PROPERTY(fullscreen)).toBool());

    const QStringList& locales = d->manager.avalibleLocales();
    QString locale = locales.value(this->viewProperty(PROPERTY(localeIndex)).toInt());
    d->manager.setCurrentLocale(locale);

    settings::Provider::setValue(settings::gui::uiSize,
                                 this->viewProperty(PROPERTY(uiSize)).toInt());
    settings::Provider::setValue(settings::gui::paletteStyle,
                                 this->viewProperty(PROPERTY(paletteStyle)));
    settings::Provider::setValue(settings::gui::fdRollInverted,
                                 this->viewProperty(PROPERTY(rollInverted)));
    settings::Provider::setValue(settings::gui::fdSpeedStep,
                                 this->viewProperty(PROPERTY(speedStep)));
    settings::Provider::setValue(settings::gui::fdSpeedUnits,
                                 this->viewProperty(PROPERTY(speedUnits)));
    settings::Provider::setValue(settings::gui::fdAltitudeStep,
                                 this->viewProperty(PROPERTY(altitudeStep)));
    settings::Provider::setValue(settings::gui::fdRelativeAltitude,
                                 this->viewProperty(PROPERTY(relativeAltitude)));

    this->setViewProperty(PROPERTY(changed), false);
}

void GuiSettingsPresenter::connectView(QObject* view)
{
    this->setViewProperty(PROPERTY(locales), QVariant::fromValue(
                              d->manager.avalibleLocales()));

    connect(view, SIGNAL(save()), this, SLOT(save()));
    connect(view, SIGNAL(restore()), this, SLOT(updateView()));

    this->updateView();
}


