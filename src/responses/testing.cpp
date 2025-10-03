#include "response.h"

int main() {
    Art art1 = Art(1, 1, "1", GLASS, 31.3750, 45.3750, GLAZING_GLASS, "475130-B", PT_SEC_4);
    Art art2 = Art(2, 1, "2", CANVAS_GALLERY, 27.0000, 27.0000, GLAZING_NONE, "313-301-BX	", PT_SEC_3);
    std::vector<Art> pieces = { art1, art2 };

    Box box1 = Box::makeStandardBox();
    box1.addArt(art2);
    Box box2 = Box::makeLargeBox();
    box2.addArt(art1);
    std::vector<Box> boxes = { box1, box2 };

    Pallet pallet1 = Pallet::makeStandardPallet();
    pallet1.addBox(box1);
    Pallet pallet2 = Pallet::makeOversizePallet();
    pallet2.addBox(box2);
    std::vector<Pallet> pallets = { pallet1, pallet2 };

    std::vector<Crate> crates;

    Response response = Response(boxes, pallets, crates);

    response.printWeightSummary();
    response.printPackingSummary();
    response.printBusinessIntelSummary();
    response.printEmailFormat();
}
