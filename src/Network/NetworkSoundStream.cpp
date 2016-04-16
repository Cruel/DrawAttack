#include <cpp3ds/System/Sleep.hpp>
#include <iostream>
#include <cpp3ds/System/Lock.hpp>
#include "NetworkSoundStream.hpp"

namespace DrawAttack {

NetworkSoundStream::NetworkSoundStream()
: m_offset(0)
, m_hasFinished(false)
{
	initialize(1, 8180);
	speex_bits_init(&m_speexBits);
	m_speexState = speex_decoder_init(&speex_nb_mode);

	int sampleRate = getSampleRate();
	speex_encoder_ctl(m_speexState, SPEEX_SET_SAMPLING_RATE, &sampleRate);
	speex_decoder_ctl(m_speexState, SPEEX_GET_FRAME_SIZE, &m_frameSize);
}

NetworkSoundStream::~NetworkSoundStream()
{
	m_hasFinished = true;
	speex_bits_destroy(&m_speexBits);
	speex_decoder_destroy(m_speexState);
}

bool NetworkSoundStream::onGetData(cpp3ds::SoundStream::Chunk &data)
{
	// We have reached the end of the buffer and all audio data have been played: we can stop playback
	if (m_hasFinished && (m_offset >= m_samples.size()))
		return false;

	// No new data has arrived since last update: wait until we get some
	while ((m_offset >= m_samples.size()) && !m_hasFinished){
		cpp3ds::sleep(cpp3ds::milliseconds(10));
	}

	if (m_hasFinished) {
		m_samples.clear();
		m_offset = 0;
		return false;
	}

	// Copy samples into a local buffer to avoid synchronization problems
	// (don't forget that we run in two separate threads)
	{
		cpp3ds::Lock lock(m_mutex);
		m_tempBuffer.assign(m_samples.begin() + m_offset, m_samples.end());
	}

	// Fill audio data to pass to the stream
	data.samples     = &m_tempBuffer[0];
	data.sampleCount = m_tempBuffer.size();

	// Update the playing offset
	m_offset += data.sampleCount;

	return true;
}

void NetworkSoundStream::onSeek(cpp3ds::Time timeOffset)
{
	std::cout << "onSeek" << std::endl;
}

void NetworkSoundStream::processEvent(const NetworkEvent& event)
{
	if (getStatus() != Status::Playing)
		play();

	std::cout << event.voice.dataLength << std::endl;

	if (event.voice.dataLength > 0)
	{
		char* data = reinterpret_cast<char*>(event.voice.data);
		spx_int16_t out[m_frameSize];
		int processedBytes = 0;

		while (processedBytes < event.voice.dataLength)
		{
			char *bytes = data + processedBytes;
			processedBytes++;

			speex_bits_read_from(&m_speexBits, data + processedBytes, *bytes);
			speex_decode_int(m_speexState, &m_speexBits, out);

			m_samples.insert(m_samples.end(), out, out + m_frameSize);

			processedBytes += *bytes;
		}
	}

	m_hasFinished = (event.type == NetworkEvent::VoiceEnd);
}

void NetworkSoundStream::stop()
{
	m_hasFinished = true;
	cpp3ds::SoundStream::stop();
}

void NetworkSoundStream::play()
{
	m_hasFinished = false;
	cpp3ds::SoundStream::play();
}

} // namespace DrawAttack
