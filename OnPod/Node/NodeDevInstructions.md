# Node dev instructions

## General Arduino
- Install arduino ide
- Install samd20 and samd21 packages (Tools -> Boards -> Board Manager)
- Replace YOUR_PATH\Arduino\portable\packages\arduino\hardware\samd\1.6.15\variants\mkr1000, 
    variant.cpp and .h with BoardPackageFiles version // custom for the Nodes

## Eclipse + sloeber
- Install latest c++ eclipse (2018-09 confirmed, neon did not work)
- Install sloeber plugin from eclipse marketplace (one with highest stars) // Eclipse arduino and MCU plugins also reccomended
- Place files from AdditionalLibraries in YOUR_PATH\Arduino\libraries\, or download maunually
