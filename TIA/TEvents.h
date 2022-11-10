#include <vector>


namespace TEvent {

    enum class TEventEnum {
        ImageAdded = 0,
        ImageDeleted = 1,
        CatalogChanged = 2
    };


    class TObserver {

    public:
        virtual void TEventReceived(TEventEnum e) = 0;

    };

    class TSubject {


    public:
        void registerTObserver(TObserver* obs);

        void removeTObserver(TObserver* obs);

        void notifyTObservers(TEventEnum e);

    protected:
        std::vector<TObserver*> m_TObservers;
    };

}