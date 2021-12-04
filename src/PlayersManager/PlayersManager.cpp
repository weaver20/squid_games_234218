#include "PlayersManager.h"

PlayersManager::PlayersManager(): groups(group_tree()),
non_empty_groups(group_tree()), players(players_tree()) {}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level) {
    try{
        group gr = groups.findNodeWithKey(GroupID)->getValue();
        player p(new Player())
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}








