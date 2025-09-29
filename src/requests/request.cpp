#include "request.h"

Request::Request(std::vector<Art> artPieces) {
    artPieces_ = artPieces;
}
Request::Request() {
    artPieces_ = std::vector<Art>();
}