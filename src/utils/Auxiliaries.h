
#ifndef SQUID_GAMES_234218_AUXILIARIES_H
#define SQUID_GAMES_234218_AUXILIARIES_H

#include "../Tree/Tree.hpp"
#include "../../lib/library1.h"

class Player_Key{
private:
    uint32_t _id;
    uint32_t _level;

public:
    Player_Key(uint32_t id, uint32_t level);
    ~Player_Key() = default;
    Player_Key(const Player_Key& other) = delete;
    Player_Key& operator=(const Player_Key& other) = delete;
    uint32_t getId() const;
    uint32_t getLevel() const;

    bool operator<(Player_Key& other) const;
    bool operator==(Player_Key& other);
    bool operator>(Player_Key& other);
};

class Group{
private:
    uint32_t _id;
    uint32_t _num_of_players;

    class Player{
    private:
        uint32_t _id;
        std::shared_ptr<Group> _group;
        uint32_t _level;
        friend class Group;

    public:
        /****** Default class functions ******/
        Player(uint32_t id, std::shared_ptr<Group> group, uint32_t level = 0);
        ~Player() = default;
        Player& operator=(const Player& other) = default;
        Player(const Player& other) = default;

        /****** Type functions ******/
        uint32_t getID() const;
        void setID(uint32_t id);

        uint32_t getLevel() const;
        void setLevel(uint32_t level);

        std::shared_ptr<Group>& getGroup();
    };
    AVL_Tree<Player_Key, std::shared_ptr<Group::Player>> _group_players;
public:

    /****** Default class functions ******/
    explicit Group(uint32_t id, uint32_t numOfPlayers = 0);
    ~Group() = default;
    Group& operator=(const Group& other) = default;
    Group(const Group& other) = default;

    /****** Type functions ******/
    uint32_t getID() const;
    void setID(uint32_t id);

    uint32_t getNumOfPlayers() const;

    AVL_Tree< Player_Key, std::shared_ptr<Group::Player>> getGroupPlayers();
};

/*************** definitions of typedef ***************/

typedef AVL_Tree<uint32_t , std::shared_ptr<Group>> group_tree;
typedef AVL_Tree<class Player_Key, std::shared_ptr<class Player>> players_tree;
typedef std::shared_ptr<class Player> player;
typedef std::shared_ptr<Group> group;

#endif //SQUID_GAMES_234218_AUXILIARIES_H
