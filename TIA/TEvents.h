#include <vector>
#include <string>

namespace TEvent {

    enum class TEventEnum {
        ImageAdded = 0,
        ImageDeleted = 1,
        CatalogChanged = 2,
        ImageActivated = 3,
        ImageDeactivated = 4,
        ImagesCleared = 5,
        AllImagesDeactivated = 6
    };

    class TEvent {

    public:

        TEvent(TEventEnum evtType, std::string data) : eventType(evtType), m_data(data) {};


        TEventEnum getType() {return eventType;}
        std::string getData() { return m_data; }
    private:

        TEventEnum eventType;
        std::string m_data;
    };



    class TObserver {

    public:
        virtual void TEventReceived(TEvent e) = 0;

    };

    class TSubject {


    public:
        void registerTObserver(TObserver* obs);

        void removeTObserver(TObserver* obs);

        void notifyTObservers(TEvent e);

    protected:
        std::vector<TObserver*> m_TObservers;
    };

}