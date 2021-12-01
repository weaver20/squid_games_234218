#include "Auxiliaries.h"

Group::Player::Player(uint32_t id, std::shared_ptr<Group> group, uint32_t level): _id(id), _group(group), _level(level) {}

uint32_t Group::Player::getID() {
    return _id;
}

void Group::Player::setID(uint32_t id) {
    _id = id;
}

uint32_t Group::Player::getLevel(){
    return _level;
}

void Group::Player::setLevel(uint32_t level) {
    _level = level;
}

std::shared_ptr<Group>& Group::Player::getGroup(){
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

players Group::getGroupPlayers() {
    return _group_players;
}




