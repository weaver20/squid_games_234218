#include "PlayersManager.h"

PlayersManager::PlayersManager(): groups(group_tree()),
                                  non_empty_groups(group_tree()), players_by_level(players_tree()) ,
                                  players_by_id(players_by_id_tree()){}


StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level) {
    if(PlayerID <= 0 or GroupID <= 0 or Level < 0) {
        return INVALID_INPUT;
    }
    try{
        if (not groups.findNodeWithKey(GroupID) or players_by_id.findNodeWithKey(PlayerID)) {
            return FAILURE;
        }
        group gr = groups.findNodeWithKey(GroupID)->getValue();
        player p(new Player(PlayerID, gr, GroupID, Level));
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
        if(GroupID <= 0)
        {
            return INVALID_INPUT;
        }
        if(not groups.isEmpty() and groups.findNodeWithKey(GroupID) != nullptr)
        {
            return FAILURE;
        }
        group new_group(new players_tree());
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
        if(players_by_id.findNodeWithKey(PlayerID) == nullptr)
        {
            return FAILURE;
        }
        player current_player = players_by_id.findNodeWithKey(PlayerID)->getValue();
        if(current_player == nullptr)
        {
            return FAILURE;
        }
        Player_Key current_key = Player_Key(current_player->getID() ,current_player->getLevel());
        group current_group = non_empty_groups.findNodeWithKey(current_player->getGroupID())->getValue();// current_player->getGroup();
        current_group->remove(current_key);

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
        if(players_by_id.findNodeWithKey(PlayerID) == nullptr)
        {
            return FAILURE;
        }
        player current_player = players_by_id.findNodeWithKey(PlayerID)->getValue();

        uint32_t current_level = current_player->getLevel();
        uint32_t new_level = current_level+LevelIncrease;
        current_player->setLevel(current_level + LevelIncrease);
        group current_group = non_empty_groups.findNodeWithKey(current_player->getGroupID())->getValue();

        players_by_level.remove(Player_Key( PlayerID, current_level));
        players_by_level.insert(Player_Key(PlayerID , new_level) , current_player);
        players_by_id.remove(PlayerID);
        players_by_id.insert(PlayerID, current_player);
        current_group->remove(Player_Key( PlayerID, current_level));
        current_group->insert(Player_Key(PlayerID , new_level) , current_player);
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType PlayersManager::GetHighestLevel(int GroupID, int *PlayerID)
{
    try{
        if(GroupID == 0 or not PlayerID) {
            return INVALID_INPUT;
        }

        if(GroupID > 0)
        {
            if(not groups.findNodeWithKey(GroupID)) {
                return FAILURE;
            }
            group current_group (groups.findNodeWithKey(GroupID)->getValue());
            if(current_group->getRightMost() == nullptr) {
                *PlayerID = -1;
                return SUCCESS;
            }
            *PlayerID = (int)current_group->getRightMost()->getValue()->getID();
        }
        else {
            //group current_group(&players_by_level);
            if(players_by_level.getRightMost() == nullptr) {
                *PlayerID = -1;
                return SUCCESS;
            }
            *PlayerID = (int) players_by_level.getRightMost()->getValue()->getID();
        }
        return SUCCESS;
    }
    catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    }
}

StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers) {
    try {
        if(not Players or not numOfPlayers or GroupID == 0) {
            return INVALID_INPUT;
        }
        player* player_arr = nullptr;
        int i = 0;
        //group current_group(&players_by_level);
        if(GroupID > 0) {
            if(groups.findNodeWithKey(GroupID) == nullptr)
            {
                return FAILURE;
            }
            group current_group = groups.findNodeWithKey(GroupID)->getValue();
            if(current_group == nullptr) {
                return FAILURE;
            }
            player_arr = current_group->AVLToSortedArray(i);
        }
        else
        {
            player_arr = players_by_level.AVLToSortedArray(i);
        }
        //int i = 0;
        //player* player_arr = nullptr;
        //player_arr = current_group->AVLToSortedArray(i);
        if(!player_arr) {
            if(i == -1)//no players in group
            {
                return ALLOCATION_ERROR;
            }
            *Players = NULL;
            *numOfPlayers = 0;
            return SUCCESS;
        }
        *numOfPlayers = i;
        int* id_array =(int*)malloc(i * sizeof (int));
        if(not id_array) {
            return ALLOCATION_ERROR;
        }
        for(int j = 0 ; j<i ; j++) {
            id_array[j] = (int)player_arr[i-j-1]->getID();
        }
        delete[] player_arr;
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
           Player_Key* original_key_arr = new Player_Key[original_size];
           Player_Key* replace_key_arr = new Player_Key[replace_size];
           for (int i = 0; i < original_size; i++){
               original_key_arr[i] = Player_Key(original_arr[i]->getID(), original_arr[i]->getLevel());
           }
           for (int i = 0; i < replace_size; i++){
               replace_key_arr[i] = Player_Key(rp_arr[i]->getID(), rp_arr[i]->getLevel());
           }
           merged_arr = mergePlayerArrays(original_key_arr , replace_key_arr ,original_arr, rp_arr, original_size, replace_size);
           delete[] original_key_arr;
           delete[] replace_key_arr;
       }

       //make remove do nothing if not in tree
       groups.remove(GroupID);
       groups.remove(ReplacementID);
       non_empty_groups.remove(GroupID);
       non_empty_groups.remove(ReplacementID);
       Player_Key* arr_key = new Player_Key[original_size + replace_size];
       for (int i = 0; i < original_size + replace_size; i++){
           arr_key[i] = Player_Key(merged_arr[i]->getID(), merged_arr[i]->getLevel());
       }
       group merged_group(new players_tree(arr_key, merged_arr, 0, original_size + replace_size - 1));
       groups.insert(ReplacementID, merged_group);
       non_empty_groups.insert(ReplacementID, merged_group);
       if(merged_arr != original_arr)
       {
           delete[] merged_arr;
       }
       delete[] original_arr;
       delete[] rp_arr;
       delete[] arr_key;
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
        non_empty_groups.scanInorder(gr_array, i, non_empty_groups.getRoot(),limit);
        *Players = (int*) malloc(numOfGroups * sizeof(int));
        if (Players == nullptr) {
            delete[] gr_array;
            return ALLOCATION_ERROR;
        }
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








