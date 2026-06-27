#ifndef MUSIC_LYRIC_AGENT_AGENT_H_
#define MUSIC_LYRIC_AGENT_AGENT_H_

#include "agent/agent.pb.h"

namespace music_lyric_model {
	using namespace lyric;

	/**
	 * Creates an AgentItem.
	 */
	lyric::AgentItem makeAgentItem();

	/**
	 * Creates a LineAgent, a line's reference to an agent.
	 */
	lyric::LineAgent makeLineAgent();
}

#endif
