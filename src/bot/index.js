const Discord = require("discord.js")

const { MessageEmbed } = require('discord.js')

const TOKEN = "MTA3NDU4MTEyODgzODY0MzczMw.GQXDiO.lrYx6elt5JIpbkacs_L4CcsQuzbTsVFOzzcw2g"



const client = new Discord.Client({
	intents: [
		"GUILDS",
		"GUILD_MESSAGES"
	]
})


client.on("ready", () => {
	console.log(`Logged in as ${client.user.tag}`)
})

client.on("messageCreate", (message) => {
	if (message.content.toLowerCase().startsWith('$echo ')){
		message.channel.send(message.content.slice(6));
	} 

	else if (message.content.toLowerCase().startsWith('$hello')){
		message.channel.send('hello world! nice to meet you!');
	} 
})

client.login(TOKEN)