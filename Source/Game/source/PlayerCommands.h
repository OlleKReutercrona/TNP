#pragma once


enum class ePlayerCommands
{
	Error,
	Move,
	Interact,
	SpawnFlower,
	DestroyFlower,
	SetColor,

	Count
};

struct PlayerCommandData 
{
	ePlayerCommands playerCommand;
	int ID;
};
