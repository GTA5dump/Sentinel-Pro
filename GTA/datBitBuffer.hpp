#pragma once
#include "../Common.hpp"
#include "../Pointers/Pointers.hpp"

#pragma pack(push, 1)
namespace rage
{
	class datBitBuffer
	{
	public:
		datBitBuffer(void* data, std::uint32_t size)
		{
			m_Data = data;
			m_BitOffset = 0;
			m_MaxBit = size * 8;
			m_BitsRead = 0;
			m_CurrentBit = 0;
			m_HighestBitsRead = 0;
			m_FlagBits = 0;
		}

		std::uint32_t GetPosition()
		{
			return m_BitsRead;
		}

		bool Seek(std::uint32_t bits)
		{
			if (bits >= 0)
			{
				const std::uint32_t Length = (m_FlagBits & 1) ? m_MaxBit : m_CurrentBit;
				if (bits <= Length)
					m_BitsRead = bits;
			}
			return false;
		}

		bool WriteBool(bool integer)
		{
			return Sentinel::Pointers::pWriteBitBufBool(this, integer, 1);
		}

		bool ReadBool(bool* integer)
		{
			return Sentinel::Pointers::pReadBitBufBool(this, integer, 1);
		}

		bool ReadPeerId(std::uint64_t* integer)
		{
			return this->ReadQWord(integer, 0x40);
		}

		std::uint64_t ReadBits(size_t numBits)
		{
			const auto TotalBits = (m_FlagBits & 1) ? m_MaxBit : m_CurrentBit;
			if ((m_FlagBits & 2) || m_BitsRead + numBits > TotalBits)
				return 0;
			const auto BufferPos = m_BitsRead + m_BitOffset;
			const auto InitialBit = BufferPos & 0b111;
			const auto Start = &((std::uint8_t*)m_Data)[BufferPos / 8];
			const auto Next = &Start[1];
			auto Result = (Start[0] << InitialBit) & 0xff;
			for (auto i = 0; i < ((numBits - 1) / 8); i++)
			{
				Result <<= 8;
				Result |= Next[i] << InitialBit;
			}
			if (InitialBit)
				Result |= Next[0] >> (8 - InitialBit);
			m_BitsRead += static_cast<std::uint32_t>(numBits);
			if (m_BitsRead > m_HighestBitsRead)
				m_HighestBitsRead = m_BitsRead;
			return Result >> ((8 - numBits) % 8);
		}

		int GetDataLength()
		{
			const int LeftOver = (m_CurrentBit % 8) ? 1 : 0;
			return (m_CurrentBit / 8) + LeftOver;
		}

		bool ReadString(char* string, int bits)
		{
			return Sentinel::Pointers::pReadBitBufString(this, string, bits);
		}

		bool WriteByte(std::uint8_t integer, int bits)
		{
			return Sentinel::Pointers::pWriteBitBufDWORD(this, integer, bits);
		}

		bool ReadByte(std::uint8_t* integer, int bits)
		{
			std::uint32_t Value;
			if (Sentinel::Pointers::pReadBitBufDWORD(this, &Value, bits))
			{
				*integer = Value;
				return true;
			}
			return false;
		}

		bool WriteWord(std::uint16_t integer, int bits)
		{
			return Sentinel::Pointers::pWriteBitBufDWORD(this, integer, bits);
		}

		bool ReadWord(std::uint16_t* integer, int bits)
		{
			std::uint32_t Value;
			if (Sentinel::Pointers::pReadBitBufDWORD(this, &Value, bits))
			{
				*integer = Value;
				return true;
			}
			return false;
		}

		bool WriteDword(std::uint32_t integer, int bits)
		{
			return Sentinel::Pointers::pWriteBitBufDWORD(this, integer, bits);
		}

		bool ReadDword(std::uint32_t* integer, int bits)
		{
			return Sentinel::Pointers::pReadBitBufDWORD(this, integer, bits);
		}

		bool WriteInt32(std::int32_t integer, int bits)
		{
			return Sentinel::Pointers::pWriteBitBufInt32(this, integer, bits);
		}

		bool ReadInt32(std::int32_t* integer, int bits)
		{
			std::int32_t V8;
			std::int32_t V9;
			if (ReadDword((std::uint32_t*)&V8, 1u) && ReadDword((std::uint32_t*)&V9, bits - 1))
			{
				*integer = V8 + (V9 ^ -V8);
				return true;
			}
			return false;
		}

		bool WriteQWord(std::uint64_t integer, int bits)
		{
			return Sentinel::Pointers::pWriteBitBufQWORD(this, integer, bits);
		}

		bool ReadQWord(std::uint64_t* integer, int bits)
		{
			if (bits <= 32)
			{
				std::uint32_t V10;
				if (ReadDword(&V10, bits))
				{
					*integer = V10;
					return true;
				}
			}
			else
			{
				std::uint32_t V10, V11;
				if (ReadDword(&V11, 32u) && ReadDword(&V10, bits - 32u))
				{
					*integer = V11 | ((std::uint64_t)V10 << 32);
					return true;
				}
			}
			return false;
		}

		bool WriteInt64(std::int64_t integer, int bits)
		{
			return Sentinel::Pointers::pWriteBitBufInt64(this, integer, bits);
		}

		bool ReadInt64(std::int64_t* integer, int bits)
		{
			std::uint32_t V8;
			std::uint64_t V9;
			if (ReadDword(&V8, 1u) && ReadQWord(&V9, bits - 1))
			{
				*integer = V8 + (V9 ^ -(std::int64_t)V8);
				return true;
			}
			return false;
		}

		bool WriteArray(void* array, int size)
		{
			return Sentinel::Pointers::pWriteBitBufArray(this, array, size, 0);
		}

		bool ReadArray(PVOID array, int size)
		{
			return Sentinel::Pointers::pReadBitBufArray(this, array, size, 0);
		}

		template<typename T>
		T Read(int length)
		{
			static_assert(sizeof(T) <= 4, "Maximum of 32 bit read");

			std::uint32_t Value = 0;
			ReadDword(&Value, length);

			return T(Value);
		}

		template<typename T>
		T ReadSigned(int length)
		{
			static_assert(sizeof(T) <= 4, "Maximum of 32 bit read");

			int Value = 0;
			ReadInt32(&Value, length);

			return T(Value);
		}

		template<typename T>
		void Write(T data, int length)
		{
			static_assert(sizeof(T) <= 8, "Maximum of 64 bit write");

			WriteQWord((std::uint64_t)data, length);
		}

		template<typename T>
		void WriteSigned(int length, T data)
		{
			const int Sign = data < 0;
			const int SignEx = (data < 0) ? 0xFFFFFFFF : 0;
			const int Data = (data ^ SignEx);

			Write<int>(1, Sign);
			Write<int>(length - 1, Data);
		}

		float ReadFloat(int length, float divisor)
		{
			const auto Integer = Read<int>(length);

			const float Max = (1 << length) - 1;
			return ((float)Integer / Max) * divisor;
		}

		void WriteFloat(int length, float divisor, float value)
		{
			const float Max = (1 << length) - 1;
			const int Integer = (int)((value / divisor) * Max);

			Write<int>(length, Integer);
		}

		float ReadSignedFloat(int length, float divisor)
		{
			const auto Integer = ReadSigned<int>(length);

			const float Max = (1 << (length - 1)) - 1;
			return ((float)Integer / Max) * divisor;
		}

		void WriteSignedFloat(int length, float divisor, float value)
		{
			const float Max = (1 << (length - 1)) - 1;
			const int Integer = (int)((value / divisor) * Max);

			WriteSigned<int>(length, Integer);
		}

	public:
		void* m_Data;						// 0x0000
		std::uint32_t m_BitOffset;			// 0x0008
		std::uint32_t m_MaxBit;				// 0x000C
		std::uint32_t m_BitsRead;			// 0x0010
		std::uint32_t m_CurrentBit;			// 0x0014
		std::uint32_t m_HighestBitsRead;	// 0x0018
		std::uint8_t m_FlagBits;			// 0x001C
	};

	class netGameEvent
	{
	public:
		virtual ~netGameEvent() = default;

		virtual const char* get_name()
		{
			return 0;
		};
		virtual bool is_in_scope(netPlayer* player)
		{
			return 0;
		};
		virtual bool time_to_resend(std::uint32_t time)
		{
			return 0;
		};
		virtual bool can_change_scope()
		{
			return 0;
		};

		virtual void prepare_data(datBitBuffer* buffer, netPlayer* source_player, netPlayer* target_player) {};
		virtual void handle_data(datBitBuffer* buffer, netPlayer* source_player, netPlayer* target_player) {};

		virtual bool decide(netPlayer* source_player, netPlayer* target_player)
		{
			return 0;
		};

		virtual void prepare_reply(datBitBuffer* buffer, netPlayer* reply_player) {};
		virtual void handle_reply(datBitBuffer* buffer, netPlayer* souce_player) {};

		virtual void prepare_extra_data(datBitBuffer* buffer, bool is_reply, netPlayer* player, netPlayer* player2) {};
		virtual void handle_extra_data(datBitBuffer* buffer, bool is_reply, netPlayer* player, netPlayer* player2) {};

	private:
		virtual void unk_0x60() {};
		virtual void unk_0x68() {};
		virtual void unk_0x70() {};
		virtual void unk_0x78() {};

	public:
		virtual bool operator==(netGameEvent const& other)
		{
			return 0;
		};
		virtual bool operator!=(netGameEvent const& other)
		{
			return 0;
		};

		virtual bool must_persist()
		{
			return 0;
		};
		virtual bool must_persist_when_out_of_scope()
		{
			return 0;
		};
		virtual bool has_timed_out()
		{
			return 0;
		};

	public:
		std::uint16_t m_id;   // 0x08
		bool m_requires_reply;// 0x0A
	private:
		char m_padding1[0x05];// 0x0B
	public:
		netPlayer* m_source_player; // 0x10
		netPlayer* m_target_player; // 0x18
		std::uint32_t m_resend_time;// 0x20
	private:
		std::uint16_t m_0x24;// 0x24
		std::uint8_t m_0x26; // 0x26
		std::uint8_t m_0x27; // 0x27
		std::uint32_t m_0x28;// 0x28
		char m_padding2[0x04];
	};
}

class CScriptedGameEvent : public rage::netGameEvent
{
public:
	char m_padding[0x40];     // 0x30
	std::int64_t m_args[54];  // 0x70
	std::uint32_t m_bitset;   // 0x220
	std::uint32_t m_args_size;// 0x224
};

class CNetworkIncrementStatEvent : public rage::netGameEvent
{
public:
	std::uint32_t m_stat;// 0x30
	std::uint32_t m_amount;// 0x34
};
#pragma pack(pop)