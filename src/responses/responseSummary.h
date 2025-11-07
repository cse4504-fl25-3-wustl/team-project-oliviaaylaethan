#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include "response.h"

struct OversizedPiece {
    float side1;
    float side2;
    int quantity;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OversizedPiece, side1, side2, quantity);
};

class ResponseSummary {
    private:
        int total_pieces;
        int standard_size_pieces;
        std::vector<OversizedPiece> oversized_pieces;
        int standard_box_count;
        int large_box_count;
        int custom_piece_count;
        int standard_pallet_count;
        int oversized_pallet_count;
        int crate_count;
        int total_artwork_weight;
        int total_packaging_weight;
        int final_shipment_weight;

    public:
        ResponseSummary(Response& response);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResponseSummary,
            total_pieces,
            standard_size_pieces,
            oversized_pieces,
            standard_box_count,
            large_box_count,
            custom_piece_count,
            standard_pallet_count,
            oversized_pallet_count,
            crate_count,
            total_artwork_weight,
            total_packaging_weight,
            final_shipment_weight
        );
};
        