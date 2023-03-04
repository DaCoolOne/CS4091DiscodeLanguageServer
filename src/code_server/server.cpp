
#include <cassert>

#include <iostream>
#include "socket/Socket.hpp"
#include <process.h>
#include <string>
#include <sstream>

#include "utils/json.hpp"
#include "flex/parser.hpp"
#include "lang/function.hpp"
#include "lang/instruction.hpp"
#include "lang/vm.hpp"

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

void unittest() {
    /*
    discode::Function myFunc;
    myFunc.pushInstruction(new discode::InstructionUJump(0));
    myFunc.pushInstruction(new discode::InstructionUJump(3));
    myFunc.pushInstruction(new discode::InstructionPush(new discode::Number(5)));
    myFunc.pushInstruction(new discode::InstructionPush(new discode::Number(4)));
    myFunc.pushInstruction(new discode::InstructionFunctionCall(2));
    myFunc.pushInstruction(new discode::InstructionCJump(2));
    myFunc.pushInstruction(new discode::InstructionUJump(1));
    std::cout << myFunc.repr() << std::endl;
    */
}

int main(int argc, char* argv[]) {

    unittest();

    bool single = false;
    for(int i = 0; i < argc; i ++) {
        if (std::string(argv[i]) == "--single-inst") {
            single = true;
        }
    }

    try {
        SocketServer in(PORT,5);

        while (1) {
            Socket* s = in.Accept();

            discode::VM vm;

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

                    discode::loadVM_string(&vm, channel_id, code);
                }
                else if (command == "Run") {

                    std::string server_id = data->at("Server_id")->asString();
                    std::string function = data->at("Function")->asString();
                    json::JsonData * message = data->at("Message").get();

                    // Build message object
                    std::shared_ptr<discode::Object> message_object = std::make_shared<discode::Object>();
                    message_object->getMap()->insert(std::pair<std::string, std::shared_ptr<discode::Data>>("channel", std::make_shared<discode::String>("default")));

                    vm.init(function, message_object);
                    vm.run(100); // Limit ourselves to 100 steps

                    discode::Error * err = vm.getError();
                    if(err != nullptr) {
                        std::cout << "VM exited with error: " << err->what() << std::endl;
                    }
                    else {
                        std::cout << "VM exited successfully" << std::endl;
                    }
                }
                else if (command == "ParseTree") {

                    std::string code = data->at("Code")->asString();

                    discode::analyze_string(code);

                }
            }

            if (single) {
                std::cout << "Connection closed, shutting down" << std::endl;
                break;
            }
        }
    }
    catch(char const* msg) {
        std::cerr << "Error: " << msg << std::endl;
        return 3;
    }
    catch(std::logic_error e) {
        std::cerr << "Process halted, logic error: " << e.what() << std::endl;
        return 2;
    }
    catch(std::exception e) {
        std::cerr << "Process halted, exception: " << e.what() << std::endl;
        return 1;
    }
    catch(...) {
        std::cerr << "An unknown error occured" << std::endl;
        return -1;
    }

    return 0;
}


