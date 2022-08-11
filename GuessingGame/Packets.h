#pragma once

enum PacketHeaderTypes
{
	PHT_Invalid = 0,
	PHT_IsCorrect,
	PHT_PlayerGuess,
	PHT_Player
};

struct GamePacket
{
	GamePacket() {}
	PacketHeaderTypes Type = PHT_Invalid;
};

/* Packet for player is correct */
struct IsCorrectPacket : public GamePacket
{
	IsCorrectPacket()
	{
		Type = PHT_IsCorrect;
	}

	int playerId = 0;
	int winnerId = -1;
	int correctNumber;
	bool isCorrect = false;
};

/* Packet with player information */
struct PlayerPacket : public GamePacket
{
	PlayerPacket()
	{
		Type = PHT_Player;
	}

	int playerId = 0;
	string msg = "";
};

/* Packet to send a player's guess */
struct PlayerGuessPacket : public GamePacket
{
	PlayerGuessPacket()
	{
		Type = PHT_PlayerGuess;
	}

	int playerId = 0;
	int playerGuess = 0;
};
