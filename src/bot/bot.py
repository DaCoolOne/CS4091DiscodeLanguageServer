import socket
import json
import time
import asyncio

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

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client.connect(("localhost", 3540))
client.setblocking(False)

bot_is_busy = False

# =================== Server Communication =================== #

def nonBlockSend(port: int, message):
    # Sends data packets to the server, to reduce rendundant code.

    client.send((json.dumps(message) + '\n').encode(encoding='utf8'))
    print('Sent message:', message)


def load(server_id: int, server_name: str, channel_id: int, channel_name:str, message_id: int, code: str):
    # Load a new codeblock into the server

    message = {
        "Name": "Load",
        "Server_ID": str(server_id),
        "Server_Name": server_name,
        "Channel_ID": str(channel_id),
        "Channel_Name": channel_name,
        "Message_ID": str(message_id),
        "Code": code
    }

    nonBlockSend(3540, message)


def run(func_name: str, server_id: int, server_name: str, channel_id: int, channel_name: str, message_id: int, arguments = {}):
    # Tell the server to run a command, probably triggered by a user's slash command

    print ("Trying to run function", func_name)
    message = {
        "Name": "Run",
        "Server_ID": str(server_id),
        "Server_Name": server_name,
        "Channel_ID": str(channel_id),
        "Channel_Name": channel_name,
        "Message_ID": str(message_id),
        "Function": func_name,
        "Message": {  },
    }

    nonBlockSend(3540, message)
    print ("Run request sent...")


async def add_func(Server_id, Function_name, arguments):
    names = [command.name for command in bot.commands]
    if Function_name not in names :
        @bot.slash_command(name=Function_name, guild_ids = [Server_id])
        @guild_only()
        async def temp(ctx):
            interaction = await ctx.respond("Sending command to server...")
            original_response = await interaction.original_response()
            message_id = original_response.id
            run((ctx.command.name), ctx.guild_id, ctx.guild.name, ctx.channel_id, ctx.channel.name, message_id)
            await interaction.edit_original_response(content="The server is running your command!", delete_after=1.5)

    await bot.sync_commands(force = True, guild_ids=[Server_id])


async def send_message(channel_id, output):
    channel = bot.get_channel(int(channel_id))
    await channel.send(output)

async def reply_message(message_id, output):
    message = bot.get_message(int(message_id))
    
    if message is not None :
        await message.reply(output)


async def handle_message(message: dict):
    if message != None :
        print("Got message", message)
        if message['Name'] == 'Add Func' :
            # Server says a function compiled and we're good to let users run it

            await add_func(message['Server_ID'], message['Function_name'], message['Arguments'])
            # ID the code was sent in, the name of the function, and the arguments it takes
        elif message['Name'] == 'Send Message' :
            # Server says we need to send output

            print("Sending message", message['Message'], "to", message['Channel_ID'])
            await send_message(message['Channel_ID'], message['Message'])
            print("Message should be sent!")
            # The ID of the message that requested the server run the command, and the desired output in response to that.
        
        elif message['Name'] == 'Error' :
            # Server says something went wrong either at runtime or during compiling

            print("Sending error", message['Error'], "to", message['Message_id'])
            await reply_message(message['Message_id'], message['Error'])
            print("Error should be sent.")



# ======================== Bot  Setup ======================== #

intents = discord.Intents.default()
intents.guild_messages = True
intents.message_content = True

bot = discord.Bot(intents = intents)

@bot.event
async def on_ready():
    global bot_is_busy
    for guild in bot.guilds :
        for channel in guild.text_channels :
            if channel.category is not None and channel.category.name.upper() == "DISCODE-CODE" :
                async for message in channel.history(oldest_first=True) :
                    if message.content.startswith("```") :
                        load(guild.id, guild.name, channel.id, channel.name, message.id, message.content)
                    await asyncio.sleep(.1)
    print(f"{bot.user} is ready and online!")


@bot.event
async def on_message(message):

    # Where was this message sent?
    category = bot.get_channel(message.channel.category_id)

    # Don't freak out if the channel has no category...
    if (category == None) :
        cat_name = "NONE"
        # Just give a default value that won't set off the command
    else :
        cat_name = category.name

    # Check if the message is code in the appropriate channel
    if message.content.startswith("```") and cat_name.upper() == "DISCODE-CODE" :
        server_id = message.guild.id
        server_name = message.guild.name
        channel_id = message.channel.id
        channel_name = message.channel.name
        message_id = message.id
        code = message.content[3:-3]
        load(server_id, server_name, channel_id, channel_name, message_id, code)



# =========================== MAIN =========================== #

async def main_loop():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("localhost", 3541))
    s.setblocking(False)

    try : 
        while True :
            try :
                message = s.recv(4096)
                msg = message.decode(encoding='utf8')
                for line in msg.splitlines():
                    obj = json.loads(line)
                    await handle_message(obj)
            except BlockingIOError :
                pass
            except Exception as e :
                print('Error: ', e)
                raise e
            await asyncio.sleep(.01)
    except Exception as e :
        print('Error: ', e)
    finally :
        s.close()
        client.close()
            

if __name__ == "__main__" :
    # Process server inputs as a thread on their own,
    # as bot.run seems to be a constant loop
    asyncio.get_event_loop().create_task(main_loop())

    bot.run(token)