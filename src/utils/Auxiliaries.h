
#ifndef SQUID_GAMES_234218_AUXILIARIES_H
#define SQUID_GAMES_234218_AUXILIARIES_H

#include "../Tree/Tree.hpp"
#include "../../lib/library1.h"

class Group{


private:
uint32_t _id;
uint32_t _num_of_players;
AVL_Tree<uint32_t, std::shared_ptr<Player>> _group_players;

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
        uint32_t getID();
        void setID(uint32_t id);

        uint32_t getLevel();
        void setLevel(uint32_t level);

        std::shared_ptr<Group>& getGroup();
    };

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

    AVL_Tree<uint32_t, std::shared_ptr<Player>> getGroupPlayers();
};

/*************** definitions of typedef ***************/

typedef AVL_Tree<uint32_t , std::shared_ptr<Group>> group_tree;
typedef AVL_Tree<uint32_t, std::shared_ptr<class Player>> players_tree;

#endif //SQUID_GAMES_234218_AUXILIARIES_H
