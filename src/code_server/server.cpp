
#include <iostream>
#include "socket/Socket.h"
#include <process.h>
#include <string>
#include <sstream>

#include "utils/json.hpp"

#define PORT 3540

unsigned __stdcall Answer(void* a) {
    Socket* s = (Socket*) a;

    while (1) {
        std::string r = s->ReceiveLine();
        std::cout << r << std::endl;
        if (r.empty()) break;
        std::stringstream streamdata(r);
        std::shared_ptr<json::JsonData> data = json::loadJsonData(streamdata);
        std::cout << data->type() << std::endl;
        s->SendLine(r);
    }

    delete s;

    return 0;
}

int main(int argc, char* argv[]) {
    SocketServer in(PORT,5);

    while (1) {
        Socket* s=in.Accept();

        unsigned ret;
        _beginthreadex(0,0,Answer,(void*) s,0,&ret);
    }
    
    return 0;
}