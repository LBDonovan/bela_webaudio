/***** AudioDestinationNode.js *****/
var AudioNode = require('./AudioNode');

class AudioDestinationNode extends AudioNode {
	
	constructor(context, ID){
		
		super(context, ID, 1, 0, 2, 'explicit');
		
		Object.defineProperty(this, 'maxChannelCount', {
		    value: 2,
		    writable: false
		});
		
	}
	
}

module.exports = AudioDestinationNode;