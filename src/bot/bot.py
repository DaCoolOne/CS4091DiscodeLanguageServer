import socket
import json
import select
import threading
import time

import os
import dotenv
# Keep sensitive bot info in a secure file

import errno
import discord
from discord.ext import commands
from discord import guild_only
# Specifically, this refers to the library installed by running:
# pip install py-cord
# Documentation for Pycord is very easy to locate.

dotenv.load_dotenv()
token = str(os.getenv("TOKEN"))

# =================== Server Communication =================== #

def nonBlockSend(port: int, message):

    time.sleep(1)
    # Sends data packets to the server, to reduce rendundant code.
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    client.connect(("localhost", port))
    client.setblocking(False)

    client.send((json.dumps(message) + '\n').encode(encoding='utf8'))
    client.close()


def load(server_id: int, channel_id: int, message_id: int, code: str):
    # Load a new codeblock into the server

    message = {
        "Name": "Load",
        "Server_id": str(server_id),
        "Channel_id": str(channel_id),
        "Code": code
    }

    nonBlockSend(3540, message)


def run(func_name: str, message_id: int, arguments = {}):
    # Tell the server to run a command, probably triggered by a user's slash command

    message = {
        "Name": "Run",
        "Function": func_name,
        "Message_id": message_id,
        "Arguments": arguments
    }

    nonBlockSend(3540, message)


def add_func(Server_id, Function_name, arguments):
    exec(f"""
        @bot.slash_command(name={Function_name}, guild_ids = [{Server_id}])
        @guild_only()
        async def {Function_name}(ctx):
            run({Function_name}, ctx.message.id)
        """)


async def send_message(channel_id, output):
    channel = bot.get_channel(channel_id)
    await channel.message.send(output)


def handler(message: dict):
    if message != None :
        print(message)
        if message["Name"] == 'Add Func' :
            # Server says a function compiled and we're good to let users run it
            print("Adding function ", message["Function_name"])
            add_func(int(message["Server_id"]), message["Function_name"], message["Arguments"])
            # ID the code was sent in, the name of the function, and the arguments it takes
        elif message["Name"] == 'Send Message' :
            # Server says we need to send output
            send_message(int(message["Channel_id"]), message["Output"])
            # The ID of the message that requested the server run the command, and the desired output in response to that.



# ======================== Bot  Setup ======================== #

intents = discord.Intents.default()
intents.guild_messages = True
intents.message_content = True

bot = discord.Bot(intents = intents)

@bot.event
async def on_ready():
    print(f"{bot.user} is ready and online!")


@bot.event
async def on_message(message):

    # Where was this message sent?
    category = bot.get_channel(message.channel.category_id)

    # Don't freak out if the channel has no category...
    if (category == None) :
        cat_name = "none"
        # Just give a default value that won't set off the command
    else :
        cat_name = category.name

    # Check if the message is code in the appropriate channel
    if message.content.startswith("```") and cat_name == "DISCODE-CODE" :
        server_id = message.guild.id
        channel_id = message.channel.id
        message_id = message.id
        code = message.content[3:-3]
        load(server_id, channel_id, message_id, code)



# =========================== MAIN =========================== #

def main_loop():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("localhost", 3541))
    s.setblocking(False)

    try : 
        while True :
            try :
                message = s.recv(4096)
                obj = json.loads(message.decode(encoding='utf8'))
                print("Message: ", obj)
                handle_message(obj)
            except BlockingIOError :
                pass
            except Exception as e :
                print('Error: ', e)
                raise e
                break
            time.sleep(.01)
    except Exception as e :
        print('Error: ', e)
    finally :
        s.close()
            

if __name__ == "__main__" :
    # Process server inputs as a thread on their own,
    # as bot.run seems to be a constant loop

    main_thread = threading.Thread(target=main_loop)
    main_thread.daemon = True
    main_thread.start()


    bot.run(token)