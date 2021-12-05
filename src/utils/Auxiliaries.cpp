#include "Auxiliaries.h"

Player::Player(uint32_t id, std::shared_ptr<Group> group, uint32_t level): _id(id), _group(group), _level(level) {}

uint32_t Player::getID() const {
    return _id;
}

void Player::setID(uint32_t id) {
    _id = id;
}

uint32_t Player::getLevel() const{
    return _level;
}

void Player::setLevel(uint32_t level) {
    _level = level;
}

std::shared_ptr<Group>& Player::getGroup(){
    return _group;
}

Group::Group(uint32_t id, uint32_t numOfPlayers): _id(id), _num_of_players(numOfPlayers) {}

uint32_t Group::getID() const {
    return _id;
}

void Group::setID(uint32_t id) {
    _id = id;
}

uint32_t Group::getNumOfPlayers() const {
    return _num_of_players;
}

AVL_Tree<Player_Key, std::shared_ptr<Player>>& Group::getGroupPlayers() {
    return _group_players;
}

Player_Key::Player_Key(uint32_t id, uint32_t level): _id(id),  _level(level){}

bool Player_Key::operator<(const Player_Key &other) const {
    if (_level < other._level) {
        return true;
    }
    else if (_level == other._level) {
        if (_id < other._id) {
            return true;
        }
    }
    return false;
}

bool Player_Key::operator==(const Player_Key &other) const {
    return not((*this < other) or (*this > other));
}

bool Player_Key::operator>(const Player_Key &other) const {
    return not((*this < other) or (*this == other));
}

uint32_t Player_Key::getId() const {
    return _id;
}

uint32_t Player_Key::getLevel() const {
    return _level;
}

