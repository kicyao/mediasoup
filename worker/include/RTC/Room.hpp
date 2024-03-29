#ifndef MS_RTC_ROOM_HPP
#define MS_RTC_ROOM_HPP

#include "common.hpp"
#include "RTC/RtpDictionaries.hpp"
#include "RTC/Peer.hpp"
#include "RTC/RtpReceiver.hpp"
#include "RTC/RtpSender.hpp"
#include "RTC/RtpPacket.hpp"
#include "RTC/RTCP/ReceiverReport.hpp"
#include "RTC/RTCP/SenderReport.hpp"
#include "RTC/RTCP/Feedback.hpp"
#include "RTC/RTCP/FeedbackRtpTmmb.hpp"
#include "RTC/RTCP/Sdes.hpp"
#include "Channel/Request.hpp"
#include "Channel/Notifier.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <json/json.h>

namespace RTC
{
	class Room :
		public RTC::Peer::Listener
	{
	public:
		class Listener
		{
		public:
			virtual void onRoomClosed(RTC::Room* room) = 0;
		};

	private:
		static uint8_t rtcpBuffer[];

	public:
		static void ClassInit();

	private:
		static RTC::RtpCapabilities supportedRtpCapabilities;
		static std::vector<uint8_t> availablePayloadTypes;

	public:
		Room(Listener* listener, Channel::Notifier* notifier, uint32_t roomId, Json::Value& data);

	private:
		virtual ~Room();

	public:
		void Destroy();
		Json::Value toJson() const;
		void HandleRequest(Channel::Request* request);
		const RTC::RtpCapabilities& GetCapabilities() const;

	private:
		RTC::Peer* GetPeerFromRequest(Channel::Request* request, uint32_t* peerId = nullptr) const;
		void SetCapabilities(std::vector<RTC::RtpCodecParameters>& mediaCodecs);

	/* Pure virtual methods inherited from RTC::Peer::Listener. */
	public:
		virtual void onPeerClosed(RTC::Peer* peer) override;
		virtual void onPeerCapabilities(RTC::Peer* peer, RTC::RtpCapabilities* capabilities) override;
		virtual void onPeerRtpReceiverParameters(RTC::Peer* peer, RTC::RtpReceiver* rtpReceiver) override;
		virtual void onPeerRtpReceiverClosed(RTC::Peer* peer, RTC::RtpReceiver* rtpReceiver) override;
		virtual void onPeerRtpSenderClosed(RTC::Peer* peer, RTC::RtpSender* rtpSender) override;
		virtual void onPeerRtpPacket(RTC::Peer* peer, RTC::RtpReceiver* rtpReceiver, RTC::RtpPacket* packet) override;
		virtual void onPeerRtcpReceiverReport(RTC::Peer* peer, RTC::RtpSender* rtpSender, RTC::RTCP::ReceiverReport* report) override;
		virtual void onPeerRtcpFeedback(RTC::Peer* peer, RTC::RtpSender* rtpSender, RTC::RTCP::FeedbackPsPacket* packet) override;
		virtual void onPeerRtcpFeedback(RTC::Peer* peer, RTC::RtpSender* rtpSender, RTC::RTCP::FeedbackRtpPacket* packet) override;
		virtual void onPeerRtcpTmmbr(RTC::Peer* peer, RTC::RtpSender* rtpSender, const RTC::RTCP::FeedbackRtpTmmbrItem* item) override;
		virtual void onPeerRtcpTmmbn(RTC::Peer* peer, RTC::RtpReceiver* rtpReceiver, const RTC::RTCP::FeedbackRtpTmmbnItem* item) override;
		virtual void onPeerRtcpSenderReport(RTC::Peer* peer, RTC::RtpReceiver* rtpReceiver, RTC::RTCP::SenderReport* report) override;
		virtual void onFullFrameRequired(RTC::Peer* peer, RTC::RtpSender* rtpSender) override;

	public:
		// Passed by argument.
		uint32_t roomId;

	private:
		// Passed by argument.
		Listener* listener = nullptr;
		Channel::Notifier* notifier = nullptr;
		// Others.
		RTC::RtpCapabilities capabilities;
		std::unordered_map<uint32_t, RTC::Peer*> peers;
		std::unordered_map<RTC::RtpReceiver*, std::unordered_set<RTC::RtpSender*>> mapRtpReceiverRtpSenders;
		std::unordered_map<RTC::RtpSender*, RTC::RtpReceiver*> mapRtpSenderRtpReceiver;
	};

	/* Inline static methods. */

	inline
	const RTC::RtpCapabilities& Room::GetCapabilities() const
	{
		return this->capabilities;
	}
}

#endif
