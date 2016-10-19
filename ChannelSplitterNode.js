/***** ChannelSplitterNode.js *****/
var AudioNode = require('./AudioNode');

class ChannelSplitterNode extends AudioNode {
	
	constructor(context, ID, outputs){
		
		super(context, ID, 1, (outputs || 2), (outputs || 2), 'max');
		
	}
	
}

module.exports = ChannelSplitterNode;