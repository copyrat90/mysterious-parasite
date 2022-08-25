# How to export

You need [Aseprite v1.3 beta](https://www.aseprite.org/beta/) to export these graphics.\
Aseprite v1.2.x might work for others, but **in order to work with `dungeon_tileset`, v1.3 or higher is a must.**

Exported `*.bmp` files should go to the parent directory, which is `/graphics`.


## `bg_minimap.aseprite`

* File > Export > Resize: 200%

## `bg_dungeon_tileset_*.aseprite`

1. File > Import Sprite Sheet
    * Type: By Rows
    * X: 0, Y: 0, Width: 8, Height: 8
2. File > Export Sprite Sheet
    * Layout
        + Sheet Type: By Rows
        + Constraints: Fixed Width, 256px
        + **Uncheck** both Merge Duplicates & Ignore Empty
    * **Don't save** `bg_dungeon_tileset_*.aseprite` and close the file. 
3. Open the exported `bg_dungeon_tileset_*.bmp`
    1. Set background color to Idx-0
        + Right click on the top-left palette item
    2. Sprite > Canvas Size
        + Set the center anchor to top row
        + Width: 256px, Height: 256px
    3. **Save** and quit.

## Others

* File > Export Sprite Sheet
    * Layout
        + Sheet Type: Vertical Strip
        + **Uncheck** both Merge Duplicates & Ignore Empty

# How to create your own tileset & export

1. Copy the file `template_dungeon_tileset.aseprite`, and modify it to create your own version.
    * Note that **tile palette index order should not be changed, and must start from 0, and end at 48.**
        + **Use Manual mode (Space+1) to avoid accidentally creating a new tile.**
2. Clear the board, and change the canvas size to 24 x 1176.
3. Starting from empty tile(0) to the end(48), put them one by one on the canvas, from top to bottom.
    * See the examples by opening any `bg_dungeon_tileset_*.aseprite` file.
4. Save it as `bg_dungeon_tileset_<your_tileset_name>.aseprite`.
5. Follow the upper export guide on `bg_dungeon_tileset_*.aseprite`.
