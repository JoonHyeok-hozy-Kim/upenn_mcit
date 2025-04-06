#include <cstdlib> 
#include <iostream>
#include <array>
#include "qdbmp.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::array;

bool valid_idx(size_t a, UINT b) {
	if (a < 0 || a >= b) {
		return false;
	}
	return true;
}


RGB get_average_RGB(BitMap& curr_image, size_t x, size_t y, UINT height, UINT width, size_t block_size) {
	array<int, 3> rgb_list{0,0,0};
	int cnt = 0;

	size_t yy = y - block_size;
	int num_iter = static_cast<int>(block_size) * 2 + 1;
	for (int idx = 0; idx < num_iter; idx++) {
		if (!valid_idx(yy, height)) {
			yy++;
		        continue;
		}	       

		size_t xx = x - block_size;
		for (int jdx = 0; jdx < num_iter; jdx++) {
			if (!valid_idx(xx, width)) {
				xx++;
				continue;
			}
			
			RGB curr_pixel = curr_image.get_pixel(xx, yy);
			rgb_list[0] += static_cast<int>(curr_pixel.red);
			rgb_list[1] += static_cast<int>(curr_pixel.green);
			rgb_list[2] += static_cast<int>(curr_pixel.blue);

			xx++;
			cnt++;
		}
		yy++;
	}

	for (int& val: rgb_list) {
		if (cnt > 0) {
			val /= cnt;
		}
	}

	RGB res{
		static_cast<UCHAR>(rgb_list[0]), 
		static_cast<UCHAR>(rgb_list[1]), 
		static_cast<UCHAR>(rgb_list[2])
	};
	return res;
}


/**
 * This program takes a .bmp image and generates its blurred 
 * image to a new .bmp file on disk.
 */
int main(int argc, char* argv[]) {
  // Check input commands
  if (argc != 4) {
    cerr << "Usage: " << argv[0] << " <input file> <output file> <block_size>" << endl;
    return EXIT_FAILURE;
  }

  string input_fname{argv[1]};
  string output_fname{argv[2]};
  int int_block_size = std::stoi(std::string(argv[3]));

  if (int_block_size <= 0) {
	  cerr << "Invalid block size : " << int_block_size << endl;
	  return EXIT_FAILURE;
  }

  UINT block_size = static_cast<UINT>(int_block_size); 

  // Construct a BitMap object using the input file specified
  BitMap image(input_fname);

  // Check the command above succeed
  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  // Create a new BitMap for output the negative image
  const unsigned int height = image.height();
  const unsigned int width = image.width();
  BitMap blurry(width, height);

  // Check the command above succeed
  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  // Loop through each pixel and turn into negative
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      // Set the average color
      blurry.set_pixel(x, y, get_average_RGB(image, x, y, height, width, block_size));
    }
  }

  // Output the averaged image to disk
  blurry.write_file(output_fname);

  if (image.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
