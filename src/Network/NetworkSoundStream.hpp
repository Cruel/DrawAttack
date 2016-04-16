#ifndef DRAWATTACK_NETWORKSOUNDSTREAM_HPP
#define DRAWATTACK_NETWORKSOUNDSTREAM_HPP

#include <cpp3ds/Audio/SoundStream.hpp>
#include "NetworkEvents.hpp"
#include <speex/speex.h>

namespace DrawAttack {

class NetworkSoundStream : public cpp3ds::SoundStream {
public:
	NetworkSoundStream();
	~NetworkSoundStream();
	void processEvent(const NetworkEvent& event);
	void play();
	void stop();

private:
	virtual bool onGetData(cpp3ds::SoundStream::Chunk& data);
	virtual void onSeek(cpp3ds::Time timeOffset);

private:
	cpp3ds::Mutex          m_mutex;
	std::vector<cpp3ds::Int16> m_samples;
	std::vector<cpp3ds::Int16> m_tempBuffer;
	std::size_t            m_offset;
	bool                   m_hasFinished;

	// Speex variables
	SpeexBits m_speexBits;
	void* m_speexState;
	int m_frameSize;
};

} // namespace DrawAttack

#endif // DRAWATTACK_NETWORKSOUNDSTREAM_HPP
