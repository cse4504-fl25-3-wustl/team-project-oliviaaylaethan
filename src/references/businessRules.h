#pragma once

// ⚠️ CRITICAL OVERSIZED DETECTION RULES
// Your code MUST implement these rules:

// Standard Box Limit: If ANY dimension >36" → requires large box
// Large Box Limit: Up to 43.5" in both directions
// Crate Threshold: >46" → requires custom pallet (won't fit in crate)
// Height Limit: 84" max recommended, 102" absolute LTL limit
// 📊 PIECES PER CONTAINER RULES
// Pallet Shipping (Boxes on Pallets)
// Material Type	Pieces per Box	Boxes per Pallet	Total per Pallet
// Glass/Acrylic Framed	6	4	24
// Glass/Acrylic (Sunrise)	8	4	32
// Canvas (Framed/Gallery)	Note: Excel vs Written Rule Conflict	4	12 or 16?
// Acoustic Panels	4	4	16
// Mirrors	Use crates (best practice)	-	-
// Crate Shipping (Custom Cardboard Boxes)
// Material	If <33" both dimensions	If >33" either dimension
// Glass/Acrylic	25 pieces per crate	18 pieces per crate
// Canvas	18 pieces per crate	12 pieces per crate
// Mirrors	24-25 pieces directly in crate	(no boxes)
// 🚨 IMPORTANT: Canvas Rule Discrepancy
// We found conflicting rules for Canvas packing:

// Excel Calculator: 6 pieces per box → 12 per pallet
// Written Instructions: 4 pieces per box → 16 per pallet
// For now: Use Excel calculator logic (12 per pallet) but flag this for Bri review in Friday meeting.

// 📏 ESTIMATED DEPTH (System Gap Solution)
// Problem: Current systems don't store depth data, but packing needs 3D dimensions.

// Solution: Use 4"+ range for all product types

// Box-Ready Depth Notes:

// Canvases get bubble wrap packaging
// Framed items get cardboard corner guards
// All packaging materials add to the packing depth per piece