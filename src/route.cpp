#include "hold.h"
#include "route.h"

Route::Route(std::vector<Hold> holds){
    for(int i = 0; i < holds.size(); i++){
        holds_route.push_back(holds[i]);
    }
}
