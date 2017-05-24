#include "vehicles_presenter.h"

// Qt
#include <QVariant>

// Internal
#include "db_facade.h"
#include "vehicle_description.h"

#include "description_vehicle_presenter.h"

using namespace presentation;

class VehiclesPresenter::Impl
{
public:
    db::DbFacade* facade;

    QList<DescriptionVehiclePresenter*> vehiclePresenters;
};

VehiclesPresenter::VehiclesPresenter(db::DbFacade* facade, QObject* parent):
    BasePresenter(parent),
    d(new Impl())
{
    d->facade = facade;

    connect(facade, &db::DbFacade::vehicleAdded,
            this, &VehiclesPresenter::onVehicleAdded);
    connect(facade, &db::DbFacade::vehicleRemoved,
            this, &VehiclesPresenter::onVehicleRemoved);

    for (const db::VehicleDescriptionPtr& description:
         facade->vehicles())
    {
        d->vehiclePresenters.append(new DescriptionVehiclePresenter(
                                        facade, description, this));
    }
}

VehiclesPresenter::~VehiclesPresenter()
{}

void VehiclesPresenter::connectView(QObject* view)
{
    connect(view, SIGNAL(addVehicle()), this, SLOT(onAddVehicle()));

    this->updateVehicles();
}

void VehiclesPresenter::onVehicleAdded(
        const db::VehicleDescriptionPtr& vehicle)
{
    d->vehiclePresenters.append(new DescriptionVehiclePresenter(
                                    d->facade, vehicle, this));
    this->updateVehicles();
}

void VehiclesPresenter::onVehicleRemoved(
        const db::VehicleDescriptionPtr& vehicle)
{
    for (DescriptionVehiclePresenter* vehiclePresenter: d->vehiclePresenters)
    {
        if (vehiclePresenter->vehicle() != vehicle) continue;

        d->vehiclePresenters.removeOne(vehiclePresenter);
        delete vehiclePresenter;
        this->updateVehicles();
        return;
    }
}

void VehiclesPresenter::updateVehicles()
{
    QList<QObject*> objectList;
    for (DescriptionVehiclePresenter* vehiclePresenter: d->vehiclePresenters)
    {
        objectList.append(vehiclePresenter);
    }

    this->setViewProperty(PROPERTY(vehicles), QVariant::fromValue(objectList));
}

void VehiclesPresenter::onAddVehicle()
{
    auto description = db::VehicleDescriptionPtr::create();

    description->setName(tr("New Vehicle"));

    d->facade->save(description);
}
