
#ifndef SQUID_GAMES_234218_AUXILIARIES_H
#define SQUID_GAMES_234218_AUXILIARIES_H

#include "../Tree/Tree.hpp"
#include "../../lib/library1.h"

class Player;

class Player_Key{
private:
    uint32_t _id;
    uint32_t _level;

public:
    Player_Key() = default;
    Player_Key(uint32_t id, uint32_t level);
    ~Player_Key() = default;
    Player_Key(const Player_Key& other) = default;
    Player_Key& operator=(const Player_Key& other) = default;
    uint32_t getId() const;
    uint32_t getLevel() const;

    bool operator<(const Player_Key& other) const;
    bool operator==(const Player_Key& other) const;
    bool operator>(const Player_Key& other) const;
};

/*class Group{
private:
    uint32_t _id;
    uint32_t _num_of_players;
    AVL_Tree<Player_Key, std::shared_ptr<Player>> _group_players;
public:*/

    /****** Default class functions ******/
    /*explicit Group(uint32_t id, uint32_t numOfPlayers = 0);
    ~Group() = default;
    Group& operator=(const Group& other) = default;
    Group(const Group& other) = default;*/

    /****** Type functions ******/
    /*uint32_t getID() const;
    void setID(uint32_t id);

    uint32_t getNumOfPlayers() const;

    AVL_Tree< Player_Key, std::shared_ptr<Player>>& getGroupPlayers();
};*/


class Player{
private:
    uint32_t _id;
    std::shared_ptr<AVL_Tree<Player_Key, std::shared_ptr<Player>>> _group;
    uint32_t _level;
    uint32_t _group_id;
    friend class Group;

public:
    /****** Default class functions ******/
    Player(uint32_t id, std::shared_ptr<AVL_Tree<Player_Key, std::shared_ptr<Player>>> group, uint32_t level = 0);
    ~Player() = default;
    Player& operator=(const Player& other) = default;
    Player(const Player& other) = default;

    /****** Type functions ******/
    uint32_t getID() const;
    void setID(uint32_t id);
    uint32_t getGroupID() const;
    void setGroupID(uint32_t groupID);
    uint32_t getLevel() const;
    void setLevel(uint32_t level);

    std::shared_ptr<AVL_Tree<Player_Key, std::shared_ptr<Player>>>& getGroup();
};

/*************** definitions of typedef ***************/
typedef std::shared_ptr<Player> player;

typedef AVL_Tree<Player_Key, player> players_tree;
typedef AVL_Tree<uint32_t , player> players_by_id_tree;

typedef std::shared_ptr<players_tree> group;
typedef AVL_Tree<uint32_t , group> group_tree;




#endif //SQUID_GAMES_234218_AUXILIARIES_H