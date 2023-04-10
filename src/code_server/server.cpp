
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
#include "lang/vm_pool.hpp"

#define INPORT  3540
#define OUTPORT 3541

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
        SocketServer    in (  INPORT, 5);
        SocketServer    out( OUTPORT, 5);

        while (1) {
            Socket* s_in  = in.Accept();
            Socket* s_out = out.Accept();

            discode::VM_Pool vms;

            while (1) {
                std::string r = s_in->ReceiveBytes();
                
                if (!r.empty()) {
                    auto lines = util::split(r, '\n');

                    for (auto const& line : lines)
                    {
                        if(line.empty()) continue;

                        std::cout << "Recieved: " << line << std::endl;
                        std::stringstream streamdata(line);
                        std::shared_ptr<json::JsonData> data = json::loadJsonData(streamdata);
                        
                        assert(data->type() == json::JSON_DATA_OBJECT); 
                        assert(data->at("Name")->type() == json::JSON_DATA_STRING);
                        
                        std::string command = data->at("Name")->asString();
                        std::cout << "Recieved " << command << " command" << std::endl;

                        if (command == "Load") {
                            
                            std::string server_id = data->at("Server_ID")->asString();
                            std::string server_name = data->at("Server_Name")->asString();
                            std::string channel_id = data->at("Channel_ID")->asString();
                            std::string channel_name = data->at("Channel_Name")->asString();
                            std::string message_id = data->at("Message_ID")->asString();
                            std::string code = data->at("Code")->asString();
                            
                            std::cout << "(" << server_name << ") #" << channel_name << '\n' << code << std::endl;

                            discode::loadVM_string(vms.get(s_out, server_id), channel_name, code, message_id);
                        }
                        else if (command == "Run") {

                            std::string server_id = data->at("Server_ID")->asString();
                            std::string server_name = data->at("Server_Name")->asString();
                            std::string channel_id = data->at("Channel_ID")->asString();
                            std::string channel_name = data->at("Channel_Name")->asString();
                            std::string function = data->at("Function")->asString();
                            json::JsonData * message = data->at("Message").get();

                            std::cout << "RUN (" << server_name << ") #" << channel_name << '.' << function << std::endl;

                            // Build message object
                            std::shared_ptr<discode::Object> channel_object = std::make_shared<discode::Object>();
                            channel_object->getMap()->insert(std::pair<std::string, std::shared_ptr<discode::Data>>("id", std::make_shared<discode::String>(channel_id)));
                            channel_object->getMap()->insert(std::pair<std::string, std::shared_ptr<discode::Data>>("name", std::make_shared<discode::String>(channel_name)));

                            std::shared_ptr<discode::Object> server_object = std::make_shared<discode::Object>();
                            server_object->getMap()->insert(std::pair<std::string, std::shared_ptr<discode::Data>>("id", std::make_shared<discode::String>(server_id)));
                            server_object->getMap()->insert(std::pair<std::string, std::shared_ptr<discode::Data>>("name", std::make_shared<discode::String>(server_name)));

                            std::shared_ptr<discode::Object> message_object = std::make_shared<discode::Object>();
                            message_object->getMap()->insert(std::pair<std::string, std::shared_ptr<discode::Data>>("channel", channel_object));
                            message_object->getMap()->insert(std::pair<std::string, std::shared_ptr<discode::Data>>("server", server_object));

                            vms.get(s_out, server_id)->init(function, message_object);

                        }
                        else if (command == "ParseTree") {

                            std::string code = data->at("Code")->asString();

                            discode::analyze_string(code);

                        }
                    }
                }

                vms.stepall();
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


