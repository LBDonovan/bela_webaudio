/***** AudioNode.h *****/

#ifndef AUDIONODE_H_INC
#define AUDIONODE_H_INC

#include <vector>

struct AudioNodeContext{
	int numInputs;
	int numOutputs;
	int numChannels;
	int numAudioFrames;
	int sampleRate;
	float* inputBuffer;
	float* outputBuffer;
};

struct OutputConnection;

class AudioNode{
	public:
		AudioNode(int inputs, int outputs, int channelCount, int channelCountMode);
		
		void connectTo(int output, AudioNode* node, int input);
		
		void receiveInput(int input, AudioNodeContext* context);
		
		void resetInputs();
		
	protected:
		
		AudioNodeContext context;
		
		virtual void render(AudioNodeContext* context){};
		
		void addInputConnection();
		
		int inputConnections;
		int inputConnectionsReceived;
		
		std::vector<OutputConnection> outputConnections;
		
	private:
		
		/*int channelCount;
		int channelCountMode;*/
		
		int inputWidth;
		int inputBufferLength;
		//int outputBufferLength;
		
		void createBuffers();
		void process();
};

struct OutputConnection{
	int output;
	AudioNode* node;
	int input;
};

#endif