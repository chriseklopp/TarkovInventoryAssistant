#pragma once
/*
Interface used as a medium between the Core library and the TarkovInventoryAssistant server.
This will provide interfaces in a way that are friendly for interop with the C# code running the server.
*/
#include <Core/TCore.h>

class TWebInterface {

    TWebInterface();





    TCore m_core;
};