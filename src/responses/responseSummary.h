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
        int total_pieces_;
        int standard_size_pieces_;
        std::vector<OversizedPiece> oversized_pieces_;
        int standard_box_count_;
        int large_box_count_;
        int custom_piece_count_;
        int standard_pallet_count_;
        int oversized_pallet_count_;
        int crate_count_;
        int total_artwork_weight_;
        int total_packaging_weight_;
        int final_shipment_weight_;

    public:
        ResponseSummary(Response& response);

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResponseSummary,
            total_pieces_,
            standard_size_pieces_,
            oversized_pieces_,
            standard_box_count_,
            large_box_count_,
            custom_piece_count_,
            standard_pallet_count_,
            oversized_pallet_count_,
            crate_count_,
            total_artwork_weight_,
            total_packaging_weight_,
            final_shipment_weight_
        );
};
        