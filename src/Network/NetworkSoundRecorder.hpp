#ifndef DRAWATTACK_NETWORKSOUNDRECORDER_HPP
#define DRAWATTACK_NETWORKSOUNDRECORDER_HPP

#include <speex/speex.h>
#include <speex/speex_preprocess.h>
#include <cpp3ds/Audio/SoundRecorder.hpp>
#include <cstddef>
#include "../States/State.hpp"
#include <cpp3ds/Audio/Sound.hpp>
#include <cpp3ds/Audio/SoundBuffer.hpp>

namespace DrawAttack {

class NetworkSoundRecorder : public cpp3ds::SoundRecorder {
public:
	NetworkSoundRecorder(State::Context* context);
	~NetworkSoundRecorder();

private:
	virtual bool onStart();
	virtual bool onProcessSamples(const cpp3ds::Int16* samples, std::size_t sampleCount);
	virtual void onStop();

private:
	State::Context* m_context;
	std::vector<cpp3ds::Int16> m_samples;

	// Speex variables
	SpeexBits m_speexBits;
	SpeexPreprocessState* m_speexPreprocessState;
	void* m_speexState;
	int m_frameSize;
};

} // namespace DrawAttack

#endif // DRAWATTACK_NETWORKSOUNDRECORDER_HPP
