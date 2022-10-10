#include "TImageReader.h"
#include "TScreenCapture.h"
#include "TDataCatalog.h"
/*
Contains the main processing loop and runs on its own thread.
TODO: Add more details
*/

class TCore {


public:
    void init();
    void run();

private:

    TImageReader m_imageReader;
    TDataCatalog m_catalog;
    int frame;


};