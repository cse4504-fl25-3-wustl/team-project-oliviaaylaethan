#include "responseSummary.h"

ResponseSummary::ResponseSummary(Response& response) {
    BoxInfo boxInfo = response.getBoxInfo();
    ArtInfo artInfo = response.getArtInfo();
    CrateInfo crateInfo = response.getCrateInfo();
    PalletInfo palletInfo = response.getPalletInfo();
    HardwareInfo hardwareInfo = response.getHardwareInfo();
    Requirements requirements = response.getRequirements();

    this->total_pieces_ = artInfo.getTotalCount();
    this->standard_size_pieces_ = artInfo.getStandardCount();
    this->oversized_pieces_ = std::vector<OversizedPiece>();
    for (Art art : artInfo.getOversizedItems()) {
        OversizedPiece piece;
        piece.side1 = art.getOuterHeight();
        piece.side2 = art.getOuterWidth();
        piece.quantity = artInfo.getQuantity(art.getLineNumber());
        this->oversized_pieces_.push_back(piece);
    }
    this->standard_box_count_ = boxInfo.getStandardBoxCount();
    this->large_box_count_ = boxInfo.getLargeBoxCount();
    this->custom_piece_count_ = artInfo.getCustomCount();
    this->standard_pallet_count_ = palletInfo.getStandardPalletCount();
    this->oversized_pallet_count_ = palletInfo.getOversizePalletCount();
    this->crate_count_ = crateInfo.getTotalCrateCount();
    this->total_artwork_weight_ = artInfo.getTotalWeight();
    this->total_packaging_weight_ = palletInfo.getTotalTareWeight() + crateInfo.getTotalTareWeight();
    this->final_shipment_weight_ = this->total_artwork_weight_ + this->total_packaging_weight_;
}