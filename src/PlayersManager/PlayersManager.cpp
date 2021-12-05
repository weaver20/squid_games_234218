#include "PlayersManager.h"

PlayersManager::PlayersManager(): groups(group_tree()),
non_empty_groups(group_tree()), players(players_tree()) ,
players_by_id(players_by_id_tree()){}

/*StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level) {
    try{
        group gr = groups.findNodeWithKey(GroupID)->getValue();
        player p(new Player())
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}*/

StatusType PlayersManager::AddGroup(int GroupID)
{
    group new_group(new Group(GroupID));
    groups.insert(GroupID , new_group);
    return SUCCESS;
}

StatusType PlayersManager::RemovePlayer(int PlayerID)
{
     player current_player = players_by_id.findNodeWithKey(PlayerID)->getValue();
     players.remove(Player_Key(current_player->getID() ,current_player->getLevel() ));
     current_player->getGroup()->getGroupPlayers().remove(Player_Key(current_player->getID() ,current_player->getLevel()));
     players_by_id.remove(PlayerID);
     return SUCCESS;
}

StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease)
{
    player current_player = players_by_id.findNodeWithKey(PlayerID)->getValue();
    uint32_t current_level = current_player->getLevel();
    uint32_t new_level = current_level+LevelIncrease;
    current_player->setLevel(current_level + LevelIncrease);
    players.remove(Player_Key(current_level , PlayerID));
    players.insert(Player_Key(new_level , PlayerID) , current_player);
    players_by_id.remove(PlayerID);
    players_by_id.insert(PlayerID, current_player);
    current_player->getGroup()->getGroupPlayers().remove(Player_Key(current_level , PlayerID));
    current_player->getGroup()->getGroupPlayers().insert(Player_Key(new_level  , PlayerID) , current_player);
    return SUCCESS;
}

StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers)
{
    std::shared_ptr<players_tree>  current_group(&players);
    if(GroupID > 0)
    {
        current_group = non_empty_groups.findNodeWithKey(GroupID)->getValue()->getGroupPlayers();
    }
}








