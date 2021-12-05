#ifndef SQUIDGAMES_PLAYERSMANAGER_H
#define SQUIDGAMES_PLAYERSMANAGER_H

#include "../utils/Auxiliaries.h"


class PlayersManager {
private:
    group_tree groups;
    group_tree non_empty_groups;
    players_tree players;
    players_by_id_tree players_by_id;
public:
    /****** Default class functions ******/
    PlayersManager();
    ~PlayersManager() = default;
    PlayersManager& operator=(const PlayersManager& other) = delete;
    PlayersManager(const PlayersManager& other) = delete;

    /****** Type functions ******/
    StatusType AddGroup(int GroupID);
    StatusType AddPlayer(int PlayerID, int GroupID, int Level);
    StatusType RemovePlayer(int PlayerID);
    StatusType ReplaceGroup(int GroupID, int ReplacementID);
    StatusType IncreaseLevel(int PlayerID, int LevelIncrease);
    StatusType GetHighestLevel(int GroupID, int *PlayerID);
    StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers);
    StatusType GetGroupsHighestLevel (int numOfGroups, int **Players);
};


#endif //SQUIDGAMES_PLAYERSMANAGER_H
