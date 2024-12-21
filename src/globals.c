
#include "../includes/interface.h"

int _screen_width = 700;
int _screen_height = 700;
const char* _engine_name = "Trading engine";

const char* _add_new_trader = "insert into users (email, password) values (?, ?)";
const char* _get_if_trader_exists = "select * from users where email = ?";
const char* _get_all_traders = "select * from users";