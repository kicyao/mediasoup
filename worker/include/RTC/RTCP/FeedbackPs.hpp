#ifndef MS_RTC_RTCP_FEEDBACK_PS_HPP
#define MS_RTC_RTCP_FEEDBACK_PS_HPP

#include "common.hpp"
#include "RTC/RTCP/Feedback.hpp"
#include <vector>

namespace RTC { namespace RTCP
{
	template<typename Item> class FeedbackPsItemsPacket
		: public FeedbackPsPacket
	{
	public:
		typedef typename std::vector<Item*>::iterator Iterator;

	public:
		static FeedbackPsItemsPacket<Item>* Parse(const uint8_t* data, size_t len);

	public:
		// Parsed Report. Points to an external data.
		explicit FeedbackPsItemsPacket(CommonHeader* commonHeader);
		explicit FeedbackPsItemsPacket(uint32_t sender_ssrc, uint32_t media_ssrc = 0);
		virtual ~FeedbackPsItemsPacket();

		void AddItem(Item* item);
		Iterator Begin();
		Iterator End();

	/* Pure virtual methods inherited from Packet. */
	public:
		virtual void Dump() const override;
		virtual size_t Serialize(uint8_t* buffer) override;
		virtual size_t GetSize() const override;

	private:
		std::vector<Item*> items;
	};

	/* Inline instance methods. */

	template<typename Item>
	FeedbackPsItemsPacket<Item>::FeedbackPsItemsPacket(CommonHeader* commonHeader):
		FeedbackPsPacket(commonHeader)
	{}

	template<typename Item>
	FeedbackPsItemsPacket<Item>::FeedbackPsItemsPacket(uint32_t sender_ssrc, uint32_t media_ssrc):
		FeedbackPsPacket(Item::MessageType, sender_ssrc, media_ssrc)
	{}

	template<typename Item>
	FeedbackPsItemsPacket<Item>::~FeedbackPsItemsPacket()
	{
		for (auto item : this->items)
		{
			delete item;
		}
	}

	template<typename Item>
	size_t FeedbackPsItemsPacket<Item>::GetSize() const
	{
		size_t size = FeedbackPsPacket::GetSize();

		for (auto item : this->items)
		{
			size += item->GetSize();
		}

		return size;
	}

	template<typename Item>
	void FeedbackPsItemsPacket<Item>::AddItem(Item* item)
	{
		this->items.push_back(item);
	}

	template<typename Item>
	typename FeedbackPsItemsPacket<Item>::Iterator FeedbackPsItemsPacket<Item>::Begin()
	{
		return this->items.begin();
	}

	template<typename Item>
	typename FeedbackPsItemsPacket<Item>::Iterator FeedbackPsItemsPacket<Item>::End()
	{
		return this->items.end();
	}
}}

#endif
