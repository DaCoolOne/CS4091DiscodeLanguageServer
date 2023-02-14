const Canvas = require("canvas")
const av = {
	size: 256
}

const background = "https://i.imgur.com/8Mfpq7z.png"
const dim = {
	height: 668,
	width: 665,
	margin: 50
}


const generateImage = async (member) => {
	let username = member.user.username
	let discrim = member.user.discriminator
	let avatarURL = member.user.displayAvatarURL({format: "png", dynamic: false, size: av.size})
}

