#ifndef ABSTRACT_VEHICLE_H
#define ABSTRACT_VEHICLE_H

// Qt
#include <QObject>

// Internal
#include "command.h"

namespace domain
{
    class Mission;

    class AbstractVehicle: public QObject
    {
        Q_OBJECT

        Q_PROPERTY(uint8_t vehicleId READ vehicleId CONSTANT)
        Q_PROPERTY(int type READ type CONSTANT)

    public:
        enum Type
        {
            UnknownType = 0
        };

        AbstractVehicle(uint8_t vehicleId, int type, QObject* parent = nullptr);
        ~AbstractVehicle() override;

        uint8_t vehicleId() const;
        int type() const;
        Mission* assignedMission() const;

    public slots:
        void assignMission(Mission* mission);
        void unassignMission();

    signals:
        void assignedMissionChanged(Mission* mission);

        void executeCommand(Command command, const QVariantList& args);

    private:
        const uint8_t m_vehicleId;
        const int m_type;

        Mission* m_assignedMission;
    };
}

#endif // ABSTRACT_VEHICLE_H
