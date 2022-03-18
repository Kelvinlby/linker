/* ********************************************************************************
 *                        Copyright (C) 2022 Kelvin_LBY
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 *     License: https://github.com/Kelvinlby/linker/LICENSE.md
 * 
 * ********************************************************************************/

#include <iostream>
#include "./inc/color.h"

// defination
#define version "1.0.0"

// usings
using namespace std;

// declaration
void about();

int main( int argv, char* args[] )
{
    cout << "\nReceived arguments: \n";

    // print arguments
    for( int i = 0; i < argv; i++ )
    {
        cout << "  No." << i << "  " << args[i] << "\n";
    }

    cout << "\n";

    // ---------- code here
    about();

    // ---------- code finish
    cout << "\n";
}

/* @brief  about linker
*/
void about()
{
    cout << RESET << BOLDCYAN << "   _ " << BOLDRED << " _" << BOLDCYAN << "         _" << endl;
    cout << RESET << BOLDCYAN << "  | |" << BOLDRED << "|_|" << BOLDCYAN << "       | | __" << RESET << CYAN << "           -+-+-+-------------------+-+-+-" << endl;
    cout << RESET << BOLDCYAN << "  | | _  _ ___ | |/ / __  _ __" << endl;
    cout << RESET << BOLDCYAN << "  | || || '_  || | // __ \\ ' _] " << RESET << CYAN << " COPYRIGHT (C) 2022 Kelvin_LBY" << endl;
    cout << RESET << BOLDCYAN << "  | || || | | ||   \\\\  __/| |   " << RESET << CYAN << " Version: " << version << endl;
    cout << RESET << BOLDCYAN << "  |_||_||_| |_||_|\\_\\\\___||_|  " << RESET << CYAN << "  License: GPL-v3.0\n" << endl;
    cout << RESET << MAGENTA << "  Github: https://github.com/Kelvinlby/linker" << endl;
    cout << RESET;
}
