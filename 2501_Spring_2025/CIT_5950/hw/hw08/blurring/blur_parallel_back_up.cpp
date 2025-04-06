#include <cstdlib> 
#include <iostream>
#include <vector>
#include <thread>
#include "qdbmp.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;


bool valid_idx(UINT a, UINT b) {
	if (a < 0 || a >= b) {
		return false;
	}
	return true;
}


struct avg_RGB_args {
	BitMap* from_image; 
	BitMap* to_image;
	UINT mod;
        UINT height; 
        UINT width; 
        UINT block_size;
	UINT thrd_num;
};


void* get_batch_avg_RGB(void* arg) {
	avg_RGB_args* args = static_cast<avg_RGB_args*>(arg);
	UINT block_size = args->block_size;
	UINT height = args->height;
	UINT width = args->width;

	UINT thrd_num = args->thrd_num;
	unsigned long long* pxl_cnt = new unsigned long long(0);
	//fprintf(stdout, "thrd %lu called\n", args->mod);

	UINT x = args->mod; 
	UINT y = 0;

	while (y < height) {
		int cnt = 0;
		int red = 0; 
		int green = 0; 
		int blue = 0;

		UINT yy = y - block_size;
		int num_iter = static_cast<int>(block_size) * 2 + 1;
		for (int idx = 0; idx < num_iter; idx++) {
			if (!valid_idx(yy, height)) {
				yy++;
		        	continue;
			}	       

			UINT xx = x - block_size;
			for (int jdx = 0; jdx < num_iter; jdx++) {
				if (!valid_idx(xx, width)) {
					xx++;
					continue;
				}
			
				RGB from_pixel = args->from_image->get_pixel(xx, yy);
				red += static_cast<int>(from_pixel.red);
				green += static_cast<int>(from_pixel.green);
				blue += static_cast<int>(from_pixel.blue);

				xx++;
				cnt++;
			}
			yy++;
		}

		if (cnt > 0) {
			red /= cnt;
			green /= cnt;
			blue /= cnt;
		}

		RGB avg_rgb = RGB{
			static_cast<UCHAR>(red),
			static_cast<UCHAR>(green),
			static_cast<UCHAR>(blue)
		};

		args->to_image->set_pixel(x, y, avg_rgb);
		++(*pxl_cnt);

		x += thrd_num;
		y += x / width;
		x %= width;
	}

	delete args; 
	return static_cast<void*>(pxl_cnt);
}



/**
 * This program takes a .bmp image and generates its blurred 
 * image to a new .bmp file on disk.
 */
int main(int argc, char* argv[]) {
  // Check input commands
  if (argc != 5) {
    cerr << "Usage: " << argv[0] << " <input file> <output file> <block_size> <thread num>" << endl;
    return EXIT_FAILURE;
  }

  string input_fname{argv[1]};
  string output_fname{argv[2]};
  int int_block_size = std::stoi(std::string(argv[3]));
  int thrd_num = std::stoi(std::string(argv[4]));

  if (int_block_size <= 0) {
	  cerr << "Invalid block size : " << int_block_size << endl;
	  return EXIT_FAILURE;
  }

  UINT block_size = static_cast<UINT>(int_block_size); 

  if (thrd_num <= 0) {
	  cerr << "Invalid thread number : " << thrd_num << endl;
	  return EXIT_FAILURE;
  }

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
  if (blurry.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  // MK 3) N threads parallel using vector
  vector<pthread_t> thrds {};
  thrds.reserve(thrd_num);
  unsigned long long total_pxl = 0;
  
  for (int idx = 0; idx < thrd_num; ++idx) {
	 pthread_t thrd = 0;

	 avg_RGB_args* args = new avg_RGB_args();
	 args->from_image = &image;
	 args->to_image = &blurry;
	 args->mod = idx;
	 args->height = height;
	 args->width = width;
	 args->block_size = block_size;
	 args->thrd_num = static_cast<UINT>(thrd_num);

	 if (pthread_create(&thrd, nullptr, get_batch_avg_RGB, static_cast<void*>(args)) != 0) { 
		 //fprintf(stderr, "pthread_create failed.\n");
		 perror("ERROR: pthread_create failed..");
		 return EXIT_FAILURE;
	 }
	 thrds.push_back(thrd);
  }

  for (auto& thrd: thrds) {
	 void* res = nullptr;
	 if (pthread_join(thrd, &res) != 0) { 
		 //fprintf(stderr, "pthread_join failed.\n");
		 perror("ERROR: pthread_join failed..");
		 return EXIT_FAILURE;
	 }

	 unsigned long long* llu_res = static_cast<unsigned long long*>(res);
	 total_pxl += *llu_res;
	 delete llu_res;
  }

  // Output the averaged image to disk
  cout << "Total pixels created : " << total_pxl << endl;
  cout << "height * width       : " << static_cast<unsigned long long>(height) * static_cast<unsigned long long>(width) << endl;
  blurry.write_file(output_fname);

  if (blurry.check_error() != BMP_OK) {
    perror("ERROR: Failed to open BMP file.");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
