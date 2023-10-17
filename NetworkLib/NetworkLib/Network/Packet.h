#pragma once

#include "Global.h"

#define MAX_PACKET_SIZE 4096
#define MAX_PACKET_ID	100

typedef enum
{
	PacketTypeMin = 0,

	CS_RegistPacket_Req_Tag,
	SC_RegistPacket_Ans_Tag,

	CS_LoginPacket_Req_Tag,
	SC_LoginPacket_Ans_Tag,

	CS_MessagePacket_Req_Tag,
	SC_MessagePacket_Ans_Tag,

	PacketTypeMax,
} packetType;

class Stream {
public:
	template<typename Ty_>
	static std::string putBinary(const Ty_& dataFormat);

	template<typename Ty_>
	static void getBinary(Ty_& dataFormat, std::string binary);
};

template<typename Ty_>
std::string Stream::putBinary(const Ty_& dataFormat)
{
	std::stringstream ss;
	boost::archive::binary_oarchive oa(ss);
	oa << dataFormat;
	return ss.str();
}


template <typename Ty_>
void Stream::getBinary(Ty_& dataFormat, std::string binary)
{
	std::stringstream ss(binary);
	boost::archive::binary_iarchive ia(ss);
	ia >> dataFormat;
}

#define InitMessage(X) setPacketData(packetType##::X##_Tag)

#pragma pack(push, 2)

class Packet
{
public:
	Packet():packetId(packetType::PacketTypeMin)
	{}

	packetType	packetId;

protected:
	void setPacketData(packetType pPacketId)
	{
		packetId = pPacketId;
	}
};

class CS_RegistPacket_Req : public Packet
{
public:
	CS_RegistPacket_Req()
	{
		InitMessage(CS_RegistPacket_Req);
		ZeroMemory(requestId, sizeof(requestId));
		ZeroMemory(requestPw, sizeof(requestPw));
		ZeroMemory(requestNick, sizeof(requestNick));
	}

	CS_RegistPacket_Req(const char* pId, const char* pPw, const char* pNick)
	{
		InitMessage(CS_RegistPacket_Req);
		strcpy_s(requestId, 30, pId);
		strcpy_s(requestPw, 30, pPw);
		strcpy_s(requestNick, 30, pNick);
	}

	char requestId[30];
	char requestPw[30];
	char requestNick[30];

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& requestId;
		ar& requestPw;
		ar& requestNick;
	};
	friend class Stream;
};

class SC_RegistPacket_Ans : public Packet
{
public:
	SC_RegistPacket_Ans()
	{
		InitMessage(SC_RegistPacket_Ans);
		isSuccess = false;
	}

	SC_RegistPacket_Ans(bool pSuccess)
	{
		InitMessage(SC_RegistPacket_Ans);
		isSuccess = pSuccess;
	}

	bool isSuccess;

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& isSuccess;
	};
	friend class Stream;
};

class CS_LoginPacket_Req : public Packet
{
public:
	CS_LoginPacket_Req()
	{
		InitMessage(CS_LoginPacket_Req);
		ZeroMemory(requestId, sizeof(requestId));
		ZeroMemory(requestPw, sizeof(requestPw));
	}

	CS_LoginPacket_Req(const char* pId, const char* pPw)
	{
		InitMessage(CS_LoginPacket_Req);
		strcpy_s(requestId, 30, pId);
		strcpy_s(requestPw, 30, pPw);
	}

	char requestId[30];
	char requestPw[30];

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& requestId;
		ar& requestPw;
	};
	friend class Stream;
};

class SC_LoginPacket_Ans : public Packet
{
public:
	SC_LoginPacket_Ans()
	{
		InitMessage(SC_LoginPacket_Ans);
		isSuccess = false;
	}

	SC_LoginPacket_Ans(bool pSuccess)
	{
		InitMessage(SC_LoginPacket_Ans);
		isSuccess = pSuccess;
	}

	bool isSuccess;

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& isSuccess;
	};
	friend class Stream;
};

class CS_MessagePacket_Req : public Packet
{
public:
	CS_MessagePacket_Req()
	{
		InitMessage(CS_MessagePacket_Req);
		ZeroMemory(requestNick, sizeof(requestNick));
		ZeroMemory(requestMsg, sizeof(requestMsg));
	}

	CS_MessagePacket_Req(const char* pNick, const char* pMsg)
	{
		InitMessage(CS_MessagePacket_Req);
		strcpy_s(requestNick, 30, pNick);
		strcpy_s(requestMsg, 1024, pMsg);
	}

	char requestNick[30];
	char requestMsg[1024];

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& requestNick;
		ar& requestMsg;
	};
	friend class Stream;
};

class SC_MessagePacket_Ans : public Packet
{
public:
	SC_MessagePacket_Ans()
	{
		InitMessage(SC_MessagePacket_Ans);
		ZeroMemory(requestNick, sizeof(requestNick));
		ZeroMemory(requestMsg, sizeof(requestMsg));
	}

	SC_MessagePacket_Ans(const char* pNick, const char* pMsg)
	{
		InitMessage(SC_MessagePacket_Ans);
		strcpy_s(requestNick, 30, pNick);
		strcpy_s(requestMsg, 1024, pMsg);
	}

	char requestNick[30];
	char requestMsg[1024];

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& requestNick;
		ar& requestMsg;
	};
	friend class Stream;
};

#pragma pack(pop)
