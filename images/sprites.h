/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites --bpp=8 --for_bitmap --transparent=FFFFFF sprites rdPiece.png xPiece.png dlPiece.png sink.png hPiece.png source.png urPiece.png luPiece.png vPiece.png cursor.png 
 * Time-stamp: Wednesday 11/06/2019, 15:22:03
 * 
 * Image Information
 * -----------------
 * rdPiece.png 16@16
 * xPiece.png 16@16
 * dlPiece.png 16@16
 * sink.png 16@16
 * hPiece.png 16@16
 * source.png 16@16
 * urPiece.png 16@16
 * luPiece.png 16@16
 * vPiece.png 16@16
 * cursor.png 16@16
 * Transparent color: (255, 255, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITES_H
#define SPRITES_H

#define SPRITES_PALETTE_TYPE (1 << 13)
#define SPRITES_DIMENSION_TYPE (1 << 6)

extern const unsigned short sprites_palette[7];
#define SPRITES_PALETTE_SIZE 14
#define SPRITES_PALETTE_LENGTH 7

extern const unsigned short sprites[1280];
#define SPRITES_SIZE 2560
#define SPRITES_LENGTH 1280

#define RDPIECE_PALETTE_ID (0 << 12)
#define RDPIECE_SPRITE_SHAPE (0 << 14)
#define RDPIECE_SPRITE_SIZE (1 << 14)
#define RDPIECE_ID 512

#define XPIECE_PALETTE_ID (0 << 12)
#define XPIECE_SPRITE_SHAPE (0 << 14)
#define XPIECE_SPRITE_SIZE (1 << 14)
#define XPIECE_ID 520

#define DLPIECE_PALETTE_ID (0 << 12)
#define DLPIECE_SPRITE_SHAPE (0 << 14)
#define DLPIECE_SPRITE_SIZE (1 << 14)
#define DLPIECE_ID 528

#define SINK_PALETTE_ID (0 << 12)
#define SINK_SPRITE_SHAPE (0 << 14)
#define SINK_SPRITE_SIZE (1 << 14)
#define SINK_ID 536

#define HPIECE_PALETTE_ID (0 << 12)
#define HPIECE_SPRITE_SHAPE (0 << 14)
#define HPIECE_SPRITE_SIZE (1 << 14)
#define HPIECE_ID 544

#define SOURCE_PALETTE_ID (0 << 12)
#define SOURCE_SPRITE_SHAPE (0 << 14)
#define SOURCE_SPRITE_SIZE (1 << 14)
#define SOURCE_ID 552

#define URPIECE_PALETTE_ID (0 << 12)
#define URPIECE_SPRITE_SHAPE (0 << 14)
#define URPIECE_SPRITE_SIZE (1 << 14)
#define URPIECE_ID 560

#define LUPIECE_PALETTE_ID (0 << 12)
#define LUPIECE_SPRITE_SHAPE (0 << 14)
#define LUPIECE_SPRITE_SIZE (1 << 14)
#define LUPIECE_ID 568

#define VPIECE_PALETTE_ID (0 << 12)
#define VPIECE_SPRITE_SHAPE (0 << 14)
#define VPIECE_SPRITE_SIZE (1 << 14)
#define VPIECE_ID 576

#define CURSOR_PALETTE_ID (0 << 12)
#define CURSOR_SPRITE_SHAPE (0 << 14)
#define CURSOR_SPRITE_SIZE (1 << 14)
#define CURSOR_ID 584

#endif

