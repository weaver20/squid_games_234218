#include "PlayersManager.h"

PlayersManager::PlayersManager(): groups(group_tree()),
                                  non_empty_groups(group_tree()), players_by_level(players_tree()) ,
                                  players_by_id(players_by_id_tree()){}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level) {
    if(PlayerID <= 0 or GroupID <= 0 or Level < 0) {
        return INVALID_INPUT;
    }
    try{
        if (not groups.findNodeWithKey(GroupID or players_by_id.findNodeWithKey(PlayerID))) {
            return FAILURE;
        }
        group gr = groups.findNodeWithKey(GroupID)->getValue();
        player p(new Player(PlayerID, gr, Level));
        Player_Key key(PlayerID, Level);
        players_by_id.insert(PlayerID, p);
        players_by_level.insert(key, p);
        gr->insert(key, p);

        /* Handling non empty groups */
        if (not non_empty_groups.findNodeWithKey(GroupID)) {
            non_empty_groups.insert(GroupID, gr);
        }
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

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
        players_by_level.remove(current_key);
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

        players_by_level.remove(Player_Key(current_level , PlayerID));
        players_by_level.insert(Player_Key(new_level , PlayerID) , current_player);
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

        group current_group(&players_by_level);

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
}

StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers)
{
    try{
        if(not Players or not numOfPlayers or GroupID == 0)
        {
            return INVALID_INPUT;
        }
        group current_group(&players_by_level);
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
StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID) {
  if (ReplacementID <= 0 or GroupID <= 0 or ReplacementID == GroupID) {
      return INVALID_INPUT;
  }
  try {
       if (not groups.findNodeWithKey(GroupID) or not groups.findNodeWithKey(ReplacementID)) {
           return FAILURE;
       }
       int original_size  = 0, replace_size = 0;
       group original = groups.findNodeWithKey(GroupID)->getValue();
       group replace = groups.findNodeWithKey(ReplacementID)->getValue();
       player* original_arr, *rp_arr = nullptr, *merged_arr = nullptr;
       if (original->getSize() == 0) {
           groups.remove(GroupID);
           non_empty_groups.remove(GroupID);
           return SUCCESS;
       }
       original_arr = original->AVLToSortedArray(original_size);
       for (int i = 0; i < original_size; i++) {
           original_arr[i]->setGroupID(ReplacementID);
       }
       merged_arr = original_arr;
       if (replace->getSize() != 0) {
           rp_arr = replace->AVLToSortedArray(replace_size);
           merged_arr = mergePlayerArrays(original_arr, rp_arr, original_size, replace_size);
       }
       delete[] original_arr;
       delete[] rp_arr;
       //make remove do nothing if not in tree
       groups.remove(GroupID);
       groups.remove(ReplacementID);
       non_empty_groups.remove(GroupID);
       non_empty_groups.remove(ReplacementID);
       Player_Key* arr_key = new Player_Key[original_size + replace_size];
       for (int i = 0; i < original_size + replace_size; i++){
           arr_key[i] = Player_Key(merged_arr[i]->getID(), merged_arr[i]->getLevel());
       }
       group merged_group(new players_tree(arr_key, merged_arr, 0, original_size + replace_size));
       groups.insert(ReplacementID, merged_group);
       non_empty_groups.insert(ReplacementID, merged_group);
       return SUCCESS;

  }
  catch (std::bad_alloc&) {
      return ALLOCATION_ERROR;
  }
}

StatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **Players) {
    if (numOfGroups < 1 or not Players) {
        return INVALID_INPUT;
    }
    if (numOfGroups > non_empty_groups.getSize()) {
        return FAILURE;
    }
    try {

        int limit = numOfGroups, i = 0;
        group* gr_array = new group[numOfGroups];
        non_empty_groups.scanInorder(gr_array, i, non_empty_groups.getRoot(),numOfGroups);
        *Players = (int*) malloc(numOfGroups * sizeof(int));
        for (i = 0; i < numOfGroups; i++) {
            *(*Players + i) = (int) gr_array[i]->getRightMost()->getValue()->getID();
        }
        delete[] gr_array;
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}







