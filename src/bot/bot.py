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

sync_flag = False

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
        "Args": arguments,
    }

    nonBlockSend(3540, message)
    print ("Run request sent...")


async def add_func(Server_id, Function_name, arguments):
    global sync_flag
    names = [command.name for command in bot.commands]
    if Function_name not in names :
        # Used below to define arguments for the slash command
        arg_str = ""

        # Used below to access the values of these arguments and add them to a
        # dictionary, to be passed to run().
        dict_def_str = ""

        for i in arguments :
            if i != "msg" :
                arg_str += f", {i}: discord.commands.Option(str, description = '', required = False, default = '')"
                dict_def_str += f"    args[\"{i}\"] = {i}\n"
        print("Trying to add command...")
        cmd = f"""@bot.slash_command(name="{Function_name}", guild_ids = [{Server_id}])
@guild_only()
async def temp(ctx{arg_str}):
    interaction = await ctx.respond("Sending command to server...")
    original_response = await interaction.original_response()
    message_id = original_response.id
    args = {{}}
{dict_def_str}
    run((ctx.command.name), ctx.guild_id, ctx.guild.name, ctx.channel_id, ctx.channel.name, message_id, args)
    await interaction.edit_original_response(content="The server is running your command!", delete_after=1.5)
"""
        print(cmd)
        exec(cmd)
        sync_flag = True


async def send_message(channel_id, output):
    channel = bot.get_channel(int(channel_id))
    await channel.send(output)

async def reply_message(message_id, channel_id, output):
    message = bot.get_message(int(message_id))
    
    if message is None :
        channel = bot.get_channel(channel_id)
        # Try to get it a potentially slower way.
        if channel is None:
            channel = await bot.fetch_channel(channel_id)

        message = await channel.fetch_message(message_id)

    if message is not None:
        await message.reply(output)
    else:
        print("Could not reply to message", message_id)
        





async def handle_message(message: dict):
    if message != None :
        print("Got message", message)
        if message['Name'] == 'Add Func' :
            # Server says a function compiled and we're good to let users run it

            await add_func(message['Server_ID'], message['Function_name'], message['Arguments'])
            # ID the code was sent in, the name of the function, and the arguments it takes
        elif message['Name'] == 'Send Message' :
            # Server says we need to send output

            if len(message['Message']) > 0:
                print("Sending message", message['Message'], "to", message['Channel_ID'])
                await send_message(message['Channel_ID'], message['Message'])
                print("Message should be sent!")
                # The ID of the message that requested the server run the command, and the desired output in response to that.
        
        elif message['Name'] == "role":
            for guild in bot.guilds:
                if str(guild.name) == message['Server_ID']:
                    print(message['User'])
                    if message['User'] == "":
                        role = discord.utils.get(guild.roles, name=message['Role'])
                        if role != None:
                            await send_message(message['Channel_ID'], "Cannot create role [" + message['Role'] +"] as it already exists.")
                        else:
                            await guild.create_role(name = message['Role'])
                    else:
                        for user in guild.members:
                            if str(user) == message['User']:
                                try:
                                    role = discord.utils.get(guild.roles, name=message['Role'])
                                    await user.add_roles(role)
                                    break
                                except:
                                    await send_message(message['Channel_ID'], "The role you entered does not exist.")
                                    break
        
        elif message['Name'] == 'Error' :
            # Server says something went wrong either at runtime or during compiling

            print("Sending error", message['Error'], "to", message['Message_id'])
            await reply_message(message['Message_id'], message["Channel_id"], message['Error'])
            print("Error should be sent.")



# ======================== Bot  Setup ======================== #

intents = discord.Intents.default()
intents.guild_messages = True
intents.message_content = True

bot = discord.Bot(intents = intents)

@bot.event
async def on_ready():
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
    if (category is None) :
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


@bot.event
async def on_raw_message_edit(payload):
    print(payload.data)
    channel = bot.get_channel(int(payload.data["channel_id"]))
    category = bot.get_channel(channel.category_id)

    if (category is None) :
        cat_name = "NONE"
    else :
        cat_name = category.name
    
    print(cat_name)

    try :
        if (str(payload.data["content"]).startswith("```") and cat_name.upper() == "DISCODE-CODE" ) :
            server_id = int(payload.data["guild_id"])
            server_name = bot.get_guild(server_id).name
            channel_id = int(payload.data["channel_id"])
            channel_name = channel.name
            message_id = int(payload.data["id"])
            code = payload.data["content"][3:-3]
            load(server_id, server_name, channel_id, channel_name, message_id, code)
    except Exception as e:
        print(e)
        


# =========================== MAIN =========================== #

async def sync_loop():
    global sync_flag
    await bot.wait_until_ready()

    while (True):
        if (sync_flag):
            sync_flag = False
            print("Syncing...")
            await bot.sync_commands()

        else:
            pass
        await asyncio.sleep(.01)


async def main_loop():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("localhost", 3541))
    s.setblocking(False)

    try : 
        while True :
            try :
                message = s.recv(4096)
                print("MSG RCV")
                msg = message.decode(encoding='utf8')
                print(msg)
                for line in msg.splitlines():
                    print("Proc line")
                    print(line)
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
    # asyncio.get_event_loop().create_task(sync_loop())

    main_thread = asyncio.get_event_loop().create_task(main_loop())
    sync_thread = asyncio.get_event_loop().create_task(sync_loop())

    bot.run(token)