
#include <cassert>

#include <iostream>
#include "socket/Socket.h"
#include <process.h>
#include <string>
#include <sstream>

#include "utils/json.hpp"

#define PORT 3540

/*
unsigned __stdcall Answer(void* a) {
    Socket* s = (Socket*) a;

    try {
        while (1) {
            std::string r = s->ReceiveLine();
            std::cout << r << std::endl;
            if (r.empty()) break;
            std::stringstream streamdata(r);
            std::shared_ptr<json::JsonData> data = json::loadJsonData(streamdata);
            std::cout << data->type() << std::endl;
            s->SendLine(r);
        }
    }
    catch (std::logic_error e) {
        std::cout << e.what() << std::endl;
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
*/

int main(int argc, char* argv[]) {
    SocketServer in(PORT,5);

    while (1) {
        Socket* s = in.Accept();

        while (1) {
            std::string r = s->ReceiveLine();
            std::cout << r << std::endl;
            
            if (r.empty()) break;
            std::stringstream streamdata(r);
            std::shared_ptr<json::JsonData> data = json::loadJsonData(streamdata);
            
            assert(data->type() == json::JSON_DATA_OBJECT); 
            assert(data->at("Name")->type() == json::JSON_DATA_STRING);
            
            std::string command = data->at("Name")->asString();
            std::cout << "Recieved " << command << " command" << std::endl;

            if (command == "Load") {
                
                std::string server_id = data->at("Server_id")->asString();
                std::string channel_id = data->at("Channel_id")->asString();
                std::string code = data->at("Code")->asString();
                
                std::cout << "(" << server_id << ") #" << channel_id << '\n' << code << std::endl;

            }
            else if (command == "Run") {

                s->SendLine(r);

            }
        }
    }
    
    return 0;
}

