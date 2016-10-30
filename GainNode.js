/***** GainNode.js *****/
var AudioNode = require('./AudioNode');

class GainNode extends AudioNode {
	
	constructor(context, ID, gainParam){
		
		super(context, ID, 1, 1, 1, 'max');
		
		Object.defineProperty(this, 'gain', {
		    value: gainParam,
		    writable: false
		});
		
	}
	
}

module.exports = GainNode;