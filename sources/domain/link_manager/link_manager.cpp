#include "link_manager.h"

// Internal
#include "mavlink_communicator.h"
#include "serial_link.h"
#include "udp_link.h"

using namespace domain;

class LinkManager::Impl
{
public:
    QList<AbstractLink*> links;
};

LinkManager::LinkManager(QObject* parent):
    QObject(parent),
    d(new Impl())
{}

LinkManager::~LinkManager()
{
    delete d;
}

QList<AbstractLink*> LinkManager::links() const
{
    return d->links;
}

void LinkManager::addNewSerialLink(const QString& portName, qint32 baudRate)
{
    auto link = new SerialLink(portName, baudRate);
    link->setObjectName(tr("Serial"));

    d->links.append(link);
    emit linksChanged(this->links());
}

void LinkManager::addNewUdpLink(int hostPort, const QString& address,
                                      int port)
{
    auto link = new UdpLink(hostPort, address, port);
    link->setObjectName(tr("UDP"));

    d->links.append(link);
    emit linksChanged(this->links());
}

void LinkManager::removeLink(AbstractLink* link)
{
    d->links.removeOne(link);
    emit linksChanged(this->links());
}