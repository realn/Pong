#pragma once

namespace pong {
  class GameInput {
  public:
    enum GAME_INPUT_DEF {
      GAME_QUIT,

      MENU_ITEM_ENTER,
      MENU_ITEM_RETURN,

      PLAYER_ONE_UP,
      PLAYER_ONE_DOWN,
      PLAYER_ONE_LEFT,
      PLAYER_ONE_RIGHT,

      PLAYER_TWO_UP,
      PLAYER_TWO_DOWN,
      PLAYER_TWO_LEFT,
      PLAYER_TWO_RIGHT
    };

    using EventIdT = unsigned;
  };
}