//
// PROJECT:         Aspia Remote Desktop
// FILE:            network/network_channel_proxy.cc
// LICENSE:         Mozilla Public License Version 2.0
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "network/network_channel_proxy.h"

namespace aspia {

NetworkChannelProxy::NetworkChannelProxy(NetworkChannel* channel) :
    channel_(channel)
{
    // Nothing
}

void NetworkChannelProxy::WillDestroyCurrentChannel()
{
    std::lock_guard<std::mutex> lock(channel_lock_);
    channel_ = nullptr;
}

bool NetworkChannelProxy::Disconnect()
{
    std::lock_guard<std::mutex> lock(channel_lock_);

    if (!channel_)
        return false;

    channel_->Disconnect();
    return true;
}

bool NetworkChannelProxy::IsConnected() const
{
    std::lock_guard<std::mutex> lock(channel_lock_);

    if (!channel_)
        return false;

    return channel_->IsConnected();
}

bool NetworkChannelProxy::Send(const IOBuffer& buffer)
{
    std::lock_guard<std::mutex> lock(channel_lock_);

    if (channel_)
    {
        channel_->Send(buffer);
        return true;
    }

    return false;
}

bool NetworkChannelProxy::SendAsync(IOBuffer buffer)
{
    std::lock_guard<std::mutex> lock(channel_lock_);

    if (!channel_)
        return false;

    channel_->SendAsync(std::move(buffer));
    return true;
}

} // namespace aspia
