#include "engine/engine.h"
#include <cstdlib>
#include <algorithm>

// void Engine::minimax(int position, int depth, bool maximizingPlayer) {
//     if (depth = 0) {
//         return staticEval();
//     }

//     if (maximizingPlayer) {
//         for (auto from : moveList) {
//             for (auto to : moveList) {
//                 eval = minimax(int position, int depth, bool maximizingPlayer)
//                 maxEval = std::max(inf_, eval)
//                 return maxEval
//             }
//         }
//     } else {
//         for (auto from : moveList) {
//             for (auto to : moveList) {
//                 eval = minimax(int position, int depth, bool maximizingPlayer)
//                 minEval = std::min(-inf_, eval)
//                 return minEval
//             }
//         }
//     }
// }