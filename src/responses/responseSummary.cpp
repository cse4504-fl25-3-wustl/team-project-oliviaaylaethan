#include "responseSummary.h"

ResponseSummary::ResponseSummary(Response& response) {
    BoxInfo boxInfo = response.getBoxInfo();
    ArtInfo artInfo = response.getArtInfo();
    CrateInfo crateInfo = response.getCrateInfo();
    PalletInfo palletInfo = response.getPalletInfo();
    HardwareInfo hardwareInfo = response.getHardwareInfo();
    Requirements requirements = response.getRequirements();

    this->total_pieces = artInfo.getTotalCount();
    this->standard_size_pieces = artInfo.getStandardCount();
    this->oversized_pieces = std::vector<DetailedPiece>();
    for (Art art : artInfo.getOversizedItems()) {
        DetailedPiece piece;
        piece.side1 = art.getOuterHeight();
        piece.side2 = art.getOuterWidth();
        piece.quantity = artInfo.getQuantity(art.getUniqueID());
        this->oversized_pieces.push_back(piece);
    }

    this->custom_pieces = std::vector<DetailedPiece>();
    for (Art art : artInfo.getCustomItems()) {
        DetailedPiece piece;
        piece.side1 = art.getOuterHeight();
        piece.side2 = art.getOuterWidth();
        piece.quantity = artInfo.getQuantity(art.getUniqueID());
        this->custom_pieces.push_back(piece);
    }
    this->standard_box_count = boxInfo.getStandardBoxCount();
    this->large_box_count = boxInfo.getLargeBoxCount();
    this->custom_piece_count = artInfo.getCustomCount();
    this->standard_pallet_count = palletInfo.getStandardPalletCount();
    this->oversized_pallet_count = palletInfo.getOversizePalletCount();
    this->crate_count = crateInfo.getTotalCrateCount();
    this->total_artwork_weight = artInfo.getTotalWeight();
    this->total_packaging_weight = palletInfo.getTotalTareWeight() + crateInfo.getTotalTareWeight();
    this->final_shipment_weight = this->total_artwork_weight + this->total_packaging_weight;
}