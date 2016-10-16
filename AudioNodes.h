/***** AudioNodes.h *****/

#ifndef AUDIONODES_H_INC
#define AUDIONODES_H_INC

#include <AudioNode.h>

class AudioSourceNode : public AudioNode {
	public:
		AudioSourceNode() : AudioNode(0, 1, 2, 0){}
		
		void recieveInterleavedInput(BelaContext* ctx){
			//rt_printf("recieveInterleavedInput\n");
			for (int n=0; n<ctx->audioFrames; n++){
				for (int ch=0; ch<ctx->audioInChannels; ch++){
					//rt_printf("from %i to %i\n", n * ctx->audioInChannels + ch, ch * ctx->audioFrames + n);
					//rt_printf("before: %f to %f\n", ctx->audioIn[n * ctx->audioInChannels + ch], context.outputBuffer[ch * ctx->audioFrames + n]);
					context.outputBuffer[ch * ctx->audioFrames + n] = ctx->audioIn[n * ctx->audioInChannels + ch];
					//rt_printf("after: %f to %f\n", ctx->audioIn[n * ctx->audioInChannels + ch], context.outputBuffer[ch * ctx->audioFrames + n]);
				}
			}
			
			for (auto connection : outputConnections){
				connection.node->receiveInput(connection.input, &context);
			}
		}
};

class AudioDestinationNode : public AudioNode {
	public:
		AudioDestinationNode() : AudioNode(1, 0, 2, 0){}
		
		void setInterleavedOutput(BelaContext* ctx){
			//rt_printf("setInterleavedOutput\n");
			for (int n=0; n<ctx->audioFrames; n++){
				for (int ch=0; ch<ctx->audioOutChannels; ch++){
					//rt_printf("from %i to %i\n", ch * ctx->audioFrames + n, n * ctx->audioOutChannels + ch);
					//rt_printf("before: %f to %f\n", context.inputBuffer[ch * ctx->audioFrames + n], ctx->audioOut[n * ctx->audioOutChannels + ch]);
					ctx->audioOut[n * ctx->audioOutChannels + ch] = context.inputBuffer[ch * ctx->audioFrames + n];
					//rt_printf("after: %f to %f\n", context.inputBuffer[ch * ctx->audioFrames + n], ctx->audioOut[n * ctx->audioOutChannels + ch]);
				}
			}
		}
};

#endif