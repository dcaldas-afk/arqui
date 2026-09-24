//
// Poxim-V Java simulator example
// 
// (C) Copyright 2024 Bruno Otavio Piedade Prado
//
// This file is part of Poxim-V.
//
// Poxim-V is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Poxim-V is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Poxim-V.  If not, see <https://www.gnu.org/licenses/>.
//

// How to build and run:
// javac nomesobrenome_123456789012_exemplo.java
// java nomesobrenome_123456789012_exemplo input.hex output.out [terminal.in] [terminal.out]

// I/O library
import java.io.*;

/**
 * nomesobrenome_123456789012_exemplo class
 * 
 * This class name must be equal of file name
 * 
 */
public class nomesobrenome_123456789012_exemplo {
	/**
	 * Main method
	 * @param args	Command line arguments
	 */
	public static void main(String[] args) {
		// Try-catch block to handle file I/O exceptions
		try {
			// Outputting separator
			System.out.println("--------------------------------------------------------------------------------");
			// Iterating over arguments
			for(int i = 0; i < args.length; i++) {
				// Outputting argument
				System.out.println("args[" + i + "] = " + args[i]);
			}
			// Opening input and output files using proper permissions
			// BufferedReader input = new BufferedReader(new FileReader(args[0]));
			// BufferedWriter output = new BufferedWriter(new FileWriter(args[1]));
			// BufferedReader terminal_in = new BufferedReader(new FileReader(args[2]));
			// BufferedWriter terminal_out = new BufferedWriter(new FileWriter(args[3]));
			// .
			// .
			// .
			// Closing input and output files
			// input.close();
			// output.close();
			// terminal_in.close();
			// terminal_out.close();
			// Outputting separator
			System.out.println("--------------------------------------------------------------------------------");
		}
		// Exception handling
		catch(Exception e) {
			// Printing stack trace
			e.printStackTrace();
		}
	}
}