#include "library1.h"
#include "../src/PlayersManager/PlayersManager.h"

void *Init() {
    //PlayersManager *DS;
    try {
        PlayersManager *DS = new PlayersManager();
        return (void *) DS;
    }
    catch (std::bad_alloc &) {
        return nullptr;
    }

}

StatusType AddGroup(void *DS, int GroupID) {
    if (not DS) {
        return INVALID_INPUT;
    }
    return (StatusType) ((PlayersManager*)DS)->AddGroup(GroupID);
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level) {
    if (not DS) {
        return INVALID_INPUT;
    }
    return (StatusType) ((PlayersManager*)DS)->AddPlayer(PlayerID, GroupID, Level);
}

StatusType RemovePlayer(void *DS, int PlayerID) {
    if (not DS) {
        return INVALID_INPUT;
    }
    return (StatusType) ((PlayersManager*)DS)->RemovePlayer(PlayerID);
}

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID) {
    if (not DS) {
        return INVALID_INPUT;
    }
    return (StatusType) ((PlayersManager*)DS)->ReplaceGroup(GroupID, ReplacementID);
}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease) {
    if (not DS) {
        return INVALID_INPUT;
    }
    return (StatusType) ((PlayersManager*)DS)->IncreaseLevel(PlayerID, LevelIncrease);
}

StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID) {
    if (not DS) {
        return INVALID_INPUT;
    }
    return (StatusType) ((PlayersManager*)DS)->GetHighestLevel(GroupID, PlayerID);
}

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers) {
    if (not DS) {
        return INVALID_INPUT;
    }
    return (StatusType) ((PlayersManager*)DS)->GetAllPlayersByLevel(GroupID, Players, numOfPlayers);
}

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players) {
    if (not DS) {
        return INVALID_INPUT;
    }
    return (StatusType) ((PlayersManager*)DS)->GetGroupsHighestLevel(numOfGroups, Players);
}

void Quit(void** DS) {
    if(*DS != NULL) {
        delete (PlayersManager *) (*DS);
    }
    *DS = NULL;
}















