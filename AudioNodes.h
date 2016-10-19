/***** AudioNodes.h *****/

#ifndef AUDIONODES_H_INC
#define AUDIONODES_H_INC

#include <AudioNode.h>
#include <Bela.h>

class AudioSourceNode : public AudioNode {
	public:
		AudioSourceNode(int id) : AudioNode(id, 0, 1, 2, EXPLICIT_CCM){}
		
		void receiveInterleavedInput(BelaContext* ctx){
			for (int n=0; n<ctx->audioFrames; n++){
				for (int ch=0; ch<ctx->audioInChannels; ch++){
					outputBuffer[0][ch][n] = ctx->audioIn[n * ctx->audioInChannels + ch];
				}
			}
			
			for (const auto connection : outputConnections){
				connection.node->receiveInput(this, &connection);
			}
		}
};

class AudioDestinationNode : public AudioNode {
	public:
		AudioDestinationNode(int id) : AudioNode(id, 1, 0, 2, EXPLICIT_CCM){}
		
		void setInterleavedOutput(BelaContext* ctx){
			for (int n=0; n<ctx->audioFrames; n++){
				for (int ch=0; ch<ctx->audioOutChannels; ch++){
					ctx->audioOut[n * ctx->audioOutChannels + ch] = inputBuffer[0][ch][n];
				}
			}
		}
};

class ChannelSplitterNode : public AudioNode {
	public:
		ChannelSplitterNode(int id, int channels) : AudioNode(id, 1, 1, channels, MAX_CCM){
			changeNumChannels(channels);
			printf("created %i\n", ID);
		}
		
		void render(){
			for (int ch = 0; ch<inputChannels; ch++){
				std::copy(inputBuffer[0][ch].begin(), inputBuffer[0][ch].end(), outputBuffer[ch][0].begin());
			}
		}
		
	protected:
		virtual void changeNumChannels(int numChannels){
			
			inputChannels = numChannels;
			outputChannels = 1;
			
			inputs = 1;
			outputs = numChannels;
			
			printf("%i ChannelSplitterNode change outputs & input channels to %i\n", ID, numChannels);
			createBuffers();
		}
};
class ChannelMergerNode : public AudioNode {
	public:
		ChannelMergerNode(int id, int channels) : AudioNode(id, 1, 1, channels, EXPLICIT_CCM){
			changeNumChannels(channels);
		}
		
		void render(){
			for (int ch = 0; ch < outputChannels; ch++){
				std::copy(inputBuffer[ch][0].begin(), inputBuffer[ch][0].end(), outputBuffer[0][ch].begin());
			}
		}
		
	protected:
		virtual void changeNumChannels(int numChannels){
			
			inputChannels = 1;
			outputChannels = numChannels;
			
			inputs = numChannels;
			outputs = 1;
			
			printf("%i ChannelMergerNode change inputs & output channels to %i\n", ID, numChannels);
			createBuffers();
		}
};

class GainNode : public AudioNode {
	public:
		GainNode(int id, float gain) : AudioNode(id, 1, 1, 1, MAX_CCM), gain(gain){}
		
		void render(){
			for (int i = 0; i<inputs; i++){
				for (int ch = 0; ch<inputChannels; ch++){
					for (int f = 0; f<audioFrames; f++){
						outputBuffer[i][ch][f] = inputBuffer[i][ch][f] * gain;
					}
				}
			}
		}
		
	private:
		float gain;
};

#endif