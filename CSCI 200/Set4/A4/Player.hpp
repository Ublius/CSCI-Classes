#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
  public:
    /**
    * @brief The default constructor of the player
    */
    Player();
    /**
    * @brief The overloaded constructor of the player to set the value of the player
    */
    Player(int playerVal);
    /**
    * @brief overloaded equality operator
    * @param OTHER object to compare to the left side value
    */
    Player& operator=(const Player& OTHER);
    /**
    * @brief Getter for the player number
    */
    int getPlayer();
    /**
    * @brief Getter for the chip number
    */
    int getChips();
    /**
    * @brief
    * @param
    */
    void giveChipTo(Player*& target);
    /**
    * @brief
    * @param
    */
    void giveChipToCenter(int& centerChips);
    /**
    * @brief
    */
    void recieveChip();
    private:
    /**
    * @brief Player number data member
    */
    int mPlayerNum;
    /**
    * @brief Player chip data member
    */
    int mChipNum;
};

Player::Player(){
    mPlayerNum = 0;
    mChipNum = 0;
}


Player::Player(int playerVal){
    mPlayerNum = playerVal;
    mChipNum = 3;
}

int Player::getPlayer(){
    return mPlayerNum;
}

int Player::getChips(){
    return mChipNum;
}

Player& Player::operator=(const Player& OTHER){
  mChipNum = OTHER.mChipNum;
  mPlayerNum = OTHER.mPlayerNum;
  return *this;
}

void Player::giveChipTo(Player& target)
{
    mChipNum--;
    target.recieveChip();
}

void Player::giveChipToCenter(int& centerChips)
{
    mChipNum--;
    centerChips++;    
}

void Player::recieveChip()
{
    mChipNum++;
}

#endif