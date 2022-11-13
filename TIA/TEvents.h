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

        TEvent(TEventEnum evtType, std::string data, int status=1) : 
            eventType(evtType),
            m_data(data),
            m_status(status){};


        TEventEnum getType() {return eventType;}
        std::string getData() { return m_data; }
        int getStatus() { return m_status; }
    private:

        TEventEnum eventType;
        std::string m_data;
        int m_status;
    };



    class TObserver {

    public:
        virtual void TEventReceived(TEvent e) = 0;
        virtual ~TObserver() {};
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