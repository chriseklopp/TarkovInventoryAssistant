#include <DataSupport/TEvents.h>
#include <algorithm>

namespace TEvent {
   


    void TSubject::registerTObserver(TObserver* obs){
        m_TObservers.push_back(obs);
    }

    void TSubject::removeTObserver(TObserver* obs){
        auto itr = std::find(m_TObservers.begin(), m_TObservers.end(), obs);
        if (itr != m_TObservers.end())
            m_TObservers.erase(itr);
    }

    void TSubject::notifyTObservers(TEvent e){
        for (TObserver* obs : m_TObservers) {
            obs->TEventReceived(e);
        }
    }
}