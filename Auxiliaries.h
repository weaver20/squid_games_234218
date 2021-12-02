
#ifndef SQUID_GAMES_234218_AUXILIARIES_H
#define SQUID_GAMES_234218_AUXILIARIES_H

#include "Tree.hpp"

/*************** definitions of typedef ***************/

typedef AVL_Tree<uint32_t , std::shared_ptr<Group>> groups;
typedef AVL_Tree<uint32_t, std::shared_ptr<Group>> non_empty_groups;
typedef AVL_Tree<uint32_t, std::shared_ptr<Player>> players;
typedef AVL_Tree<uint32_t, std::shared_ptr<Player>> players_by_id;

class Player{
private:
    uint32_t _id;
    std::shared_ptr<Group> _group;
    uint32_t _level;

public:

    /****** Default class functions ******/
    Player(uint32_t id, std::shared_ptr<Group> group, uint32_t level = 0);
    ~Player() = default;
    Player& operator=(const Player& other) = default;
    Player(const Player& other) = default;

    /****** Type functions ******/
    uint32_t getID();
    void setID(uint32_t id);

    uint32_t getLevel();
    void setLevel(uint32_t level);

    std::shared_ptr<Group>& getGroup;
};

class Group{
private:
int _id;
int _num_of_players;
players _group_players;

public:
    /****** Default class functions ******/
    explicit Group(uint32_t id, uint32_t numOfPlayers = 0);
    ~Group() = default;
    Group& operator=(const Group& other);
    Group(const Group& other) = default;

    /****** Type functions ******/
    uint32_t getID();
    void setID(uint32_t id);

    uint32_t getNumOfPlayers();

    players getGroupPlayers();
};

#endif //SQUID_GAMES_234218_AUXILIARIES_H
