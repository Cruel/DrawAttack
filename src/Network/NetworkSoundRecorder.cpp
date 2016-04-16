#include <cpp3ds/Network/IpAddress.hpp>
#include <cpp3ds/Network/TcpSocket.hpp>
#include <cpp3ds/Audio/SoundRecorder.hpp>
#include <cpp3ds/Network/Packet.hpp>
#include "NetworkSoundRecorder.hpp"

namespace DrawAttack {

NetworkSoundRecorder::NetworkSoundRecorder(State::Context* context)
: m_context(context)
, m_frameSize(160)
{
	speex_bits_init(&m_speexBits);
	m_speexState = speex_encoder_init(&speex_nb_mode);

	int quality = 6;
	speex_encoder_ctl(m_speexState, SPEEX_SET_QUALITY, &quality);

	int enabled = 1;
	int target = 16000;
	m_speexPreprocessState = speex_preprocess_state_init(m_frameSize, 8180);
	speex_preprocess_ctl(m_speexPreprocessState, SPEEX_PREPROCESS_SET_AGC, &enabled);
	speex_preprocess_ctl(m_speexPreprocessState, SPEEX_PREPROCESS_SET_AGC_TARGET, &target);

	setProcessingInterval(cpp3ds::milliseconds(100));
}

NetworkSoundRecorder::~NetworkSoundRecorder()
{
	speex_preprocess_state_destroy(m_speexPreprocessState);
	speex_encoder_destroy(m_speexState);
	speex_bits_destroy(&m_speexBits);
}

bool NetworkSoundRecorder::onStart()
{
	int sampleRate = getSampleRate();
	speex_encoder_ctl(m_speexState, SPEEX_SET_SAMPLING_RATE, &sampleRate);
	speex_encoder_ctl(m_speexState, SPEEX_GET_FRAME_SIZE, &m_frameSize);

	m_samples.clear();

	return true;
}

bool NetworkSoundRecorder::onProcessSamples(const cpp3ds::Int16 *samples, std::size_t sampleCount)
{
	m_samples.insert(m_samples.end(), samples, samples + sampleCount);

	std::vector<char> encodedSamples;
	char out[m_frameSize*2];
	int size = m_samples.size();
	int i = 0;

	while (size >= m_frameSize)
	{
		spx_int16_t* audioFrame = &m_samples[0] + i * m_frameSize;

		speex_preprocess_run(m_speexPreprocessState, audioFrame);

		speex_bits_reset(&m_speexBits);
		speex_encode_int(m_speexState, audioFrame, &m_speexBits);

		char bytes = speex_bits_write(&m_speexBits, out, sizeof(out));

		encodedSamples.push_back(bytes);
		encodedSamples.insert(encodedSamples.end(), out, out + bytes);

		i++;
		size -= m_frameSize;
	}

	std::vector<cpp3ds::Int16>(m_samples.end() - size, m_samples.end()).swap(m_samples);

	std::cout << "size: " << sampleCount * sizeof(cpp3ds::Int16) << std::endl;

	m_context->client.sendVoiceData(m_context->name.toAnsiString(), &encodedSamples[0], encodedSamples.size());
	return true;
}

void NetworkSoundRecorder::onStop()
{
	m_context->client.sendVoiceEnd(m_context->name.toAnsiString());
}

} // namespace DrawAttack
