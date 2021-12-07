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
    try{
        group new_group(new players_tree());
        if(GroupID <= 0)
        {
            return INVALID_INPUT;
        }
        if(groups.findNodeWithKey(GroupID) == nullptr)
        {
            return FAILURE;
        }
        groups.insert(GroupID , new_group);
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }


}

StatusType PlayersManager::RemovePlayer(int PlayerID)
{
    try{
        if(PlayerID <= 0)
        {
            return INVALID_INPUT;
        }
        player current_player = players_by_id.findNodeWithKey(PlayerID)->getValue();
        if(current_player == nullptr)
        {
            return FAILURE;
        }
        Player_Key current_key = Player_Key(current_player->getID() ,current_player->getLevel());
        group current_group = current_player->getGroup();
        current_player->getGroup()->remove(current_key);

        if(current_group->isEmpty())
        {
            non_empty_groups.remove(current_player->getGroupID());
        }
        players.remove(current_key);
        players_by_id.remove(PlayerID);
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease)
{
    try{
        if(PlayerID <= 0 or LevelIncrease <= 0)
        {
            return INVALID_INPUT;
        }
        player current_player = players_by_id.findNodeWithKey(PlayerID)->getValue();
        if(current_player == nullptr)
        {
            return FAILURE;
        }
        uint32_t current_level = current_player->getLevel();
        uint32_t new_level = current_level+LevelIncrease;
        current_player->setLevel(current_level + LevelIncrease);

        players.remove(Player_Key(current_level , PlayerID));
        players.insert(Player_Key(new_level , PlayerID) , current_player);
        players_by_id.remove(PlayerID);
        players_by_id.insert(PlayerID, current_player);
        current_player->getGroup()->remove(Player_Key(current_level , PlayerID));
        current_player->getGroup()->insert(Player_Key(new_level  , PlayerID) , current_player);
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType PlayersManager::GetHighestLevel(int GroupID, int *PlayerID)
{
    try{
        if(GroupID == 0 or !PlayerID)
        {
            return INVALID_INPUT;
        }

        group current_group(&players);

        if(GroupID > 0)
        {
            current_group = groups.findNodeWithKey(GroupID)->getValue();
            if(current_group == nullptr)
            {
                return FAILURE;
            }
        }
        *PlayerID = (int)current_group->getRightMost()->getValue()->getID();
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
    delete[] player_arr;
    *Players = id_array;
    return SUCCESS;
}

StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers)
{
    try{
        if(!Players or !numOfPlayers or GroupID == 0)
        {
            return INVALID_INPUT;
        }
        group current_group(&players);
        if(GroupID > 0)
        {
            current_group = groups.findNodeWithKey(GroupID)->getValue();
            if(current_group == nullptr)
            {
                return FAILURE;
            }
        }
        int i = 0;
        player* player_arr = nullptr;
        player_arr = current_group->AVLToSortedArray(i);
        if(!player_arr)
        {
            return ALLOCATION_ERROR;
        }
        *numOfPlayers = i;
        int* id_array =(int*)malloc(i * sizeof (int));
        if(!id_array)
        {
            return ALLOCATION_ERROR;
        }
        for(int j = 0 ; j<i ; j++)
        {
            id_array[j] = (int)player_arr[j]->getID();
        }
        delete player_arr;
        *Players = id_array;
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}







