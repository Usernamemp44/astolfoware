#pragma once
#include "Util.h"
#include "Misc.h"
#include "Client.h"
#include <thread>
#include "Math.h"
#include <chrono>
bool isPlayerOnFriendsList(int index)
{
	player_info_t pInfo;

	if (!gInts.Engine->GetPlayerInfo(index, &pInfo))
		return false;

	for (int i = 0; i < gInts.steamfriends->GetFriendCount(k_EFriendFlagImmediate); i++)
	{
		CSteamID friendID = gInts.steamfriends->GetFriendByIndex(i, k_EFriendFlagImmediate);

		// this makes the assumption that it is an individual account and it is in the public universe ( the most common type )
		CSteamID pInfoFriendID{ pInfo.friendsID, k_EUniversePublic, k_EAccountTypeIndividual };

		if (friendID == pInfoFriendID)
			return true;
	}
	return false;
}